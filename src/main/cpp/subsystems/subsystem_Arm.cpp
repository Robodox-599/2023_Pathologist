// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystems/subsystem_Arm.h"

subsystem_Arm::subsystem_Arm() : m_BottomArmMotor{ArmConstants::bottomArmMotorID, rev::CANSparkMax::MotorType::kBrushless},
                                 m_BottomFollower{ArmConstants::bottomFollowerID, rev::CANSparkMax::MotorType::kBrushless},
                                 m_TopArmMotor{ArmConstants::topArmMotorID, rev::CANSparkMax::MotorType::kBrushless},
                                 m_TopFollower{ArmConstants::topFollowerID, rev::CANSparkMax::MotorType::kBrushless},
                                 m_IntakeTiltMotor{ArmConstants::intakeTiltMotorID, rev::CANSparkMax::MotorType::kBrushless},
                                 m_BottomArmPID{m_BottomArmMotor.GetPIDController()},
                                 m_TopArmPID{m_TopArmMotor.GetPIDController()},
                                 m_IntakeTiltPID{m_IntakeTiltMotor.GetPIDController()},
                                 m_TopAbsEncoder{m_TopArmMotor.GetAbsoluteEncoder(rev::SparkMaxAbsoluteEncoder::Type::kDutyCycle)},
                                 m_BottomAbsEncoder{m_BottomArmMotor.GetAbsoluteEncoder(rev::SparkMaxAbsoluteEncoder::Type::kDutyCycle)},
                                 m_BottomRelEncoder{m_BottomArmMotor.GetEncoder()},
                                 m_TopRelEncoder{m_TopArmMotor.GetEncoder()},
                                 m_TopSolenoid{frc::PneumaticsModuleType::CTREPCM, IntakeConstants::IntakePistonLA, IntakeConstants::IntakePistonLB},
                                 m_BottomSolenoid{frc::PneumaticsModuleType::CTREPCM, IntakeConstants::IntakePistonRA, IntakeConstants::IntakePistonRB}
{
    m_BottomArmMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_TopArmMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);

    m_BottomFollower.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_TopFollower.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);

    m_BottomFollower.Follow(m_BottomArmMotor);
    m_TopFollower.Follow(m_TopArmMotor);

    m_BottomArmPID.SetP(ArmConstants::kBottomGoingUpP);
    m_BottomArmPID.SetD(ArmConstants::kBottomGoingUpD);
    m_BottomArmPID.SetFF(ArmConstants::kBottomGoingUpF);
    m_TopArmPID.SetP(ArmConstants::kTopGoingUpP);
    m_TopArmPID.SetD(ArmConstants::kTopGoingUpD);
    m_TopArmPID.SetFF(ArmConstants::kTopGoingUpF);

    m_BottomArmPID.SetSmartMotionMaxAccel(ArmConstants::kBottomMaxA);
    m_BottomArmPID.SetSmartMotionMaxVelocity(ArmConstants::kBottomMaxV);
    m_BottomArmPID.SetSmartMotionMinOutputVelocity(ArmConstants::kBottomMinV);

    m_TopArmPID.SetSmartMotionMaxAccel(ArmConstants::kTopMaxA);
    m_TopArmPID.SetSmartMotionMaxVelocity(ArmConstants::kTopMaxV);
    m_TopArmPID.SetSmartMotionMinOutputVelocity(ArmConstants::kTopMinV);

    topStartPos = m_TopAbsEncoder.GetPosition();
    bottomStartPos = m_BottomAbsEncoder.GetPosition();
}

double subsystem_Arm::CalculateBottomArmAngle(double x, double y)
{
    double topAngle = CalculateTopArmAngle(x, y);
    return (atan(y / x) - atan((ArmConstants::topJointLength * sin(topAngle)) 
            / (ArmConstants::bottomJointLength + (ArmConstants::topJointLength * cos(topAngle)))));
}
double subsystem_Arm::CalculateTopArmAngle(double x, double y)
{
    return (-acos((pow(x, 2) + pow(y, 2) - pow(ArmConstants::topJointLength, 2) - pow(ArmConstants::bottomJointLength, 2)) 
            / (2 * ArmConstants::topJointLength * ArmConstants::bottomJointLength)));
}

void subsystem_Arm::MoveArm(double x, double y)
{
    armX = x;
    armY = y; 
    adjustedX = x + ArmConstants::xOriginAdjustment;
    adjustedY = y + ArmConstants::yOriginAdjustment;

    SetArmPIDByDirection(x, y);

    if (adjustedX <= ArmConstants::totalArmLength && adjustedY <= ArmConstants::totalArmLength)
    {
        double bottom = CalculateBottomArmAngle(adjustedX, adjustedY);
        convertedTop = (CalculateTopArmAngle(adjustedX, adjustedY)) * (180 / 3.14159265358979) /* ArmConstants::radiansToEncoder*/;
        convertedBottom = bottom * (180 / 3.14159265358979) /* ArmConstants::radiansToEncoder*/;

        m_BottomArmPID.SetReference(convertedBottom, rev::ControlType::kSmartMotion, 0);
        m_TopArmPID.SetReference(convertedTop, rev::ControlType::kSmartMotion, 0);
    }
    else
    {
        printf("INVALID X OR Y INPUT");
    }
    frc::SmartDashboard::PutString("Move arm Method", "moving arm");
}

void subsystem_Arm::MoveArmManually(double leftAxis, double rightAxis)
{
    manualX = armX + (leftAxis * ArmConstants::JoystickToArm);
    manualY = armY + (rightAxis * ArmConstants::JoystickToArm);
    MoveArm(manualX, manualY);
}

void subsystem_Arm::SetToZero()
{
    m_BottomArmPID.SetReference(0, rev::ControlType::kSmartMotion);
    m_TopArmPID.SetReference(0, rev::ControlType::kSmartMotion);
}

void subsystem_Arm::SetIntakeAngle(double angle)
{
    double adjustedAngle = angle - intakeAngleOffset;
    m_IntakeTiltPID.SetReference(adjustedAngle, rev::ControlType::kPosition, 0);
}

void subsystem_Arm::SetArmPIDByDirection(double x, double y){
    if (IsBottomArmDirectionGoingUp(x, y))
    {
        m_BottomArmPID.SetP(ArmConstants::kBottomGoingUpP);
        m_BottomArmPID.SetD(ArmConstants::kBottomGoingUpD);
        m_BottomArmPID.SetD(ArmConstants::kBottomGoingUpF);
    }
    else
    {
        m_BottomArmPID.SetP(ArmConstants::kBottomGoingDownP);
        m_BottomArmPID.SetD(ArmConstants::kBottomGoingDownD);
        m_BottomArmPID.SetD(ArmConstants::kBottomGoingDownF);
    }
    if (IsTopArmDirectionGoingUp(x, y))
    {
        m_TopArmPID.SetP(ArmConstants::kTopGoingUpP);
        m_TopArmPID.SetD(ArmConstants::kTopGoingUpD);
        m_TopArmPID.SetD(ArmConstants::kTopGoingUpF);
    }
    else
    {
        m_TopArmPID.SetP(ArmConstants::kTopGoingDownP);
        m_TopArmPID.SetD(ArmConstants::kTopGoingDownD);
        m_TopArmPID.SetD(ArmConstants::kTopGoingDownF);
    }
}

void subsystem_Arm::UnlockArm(){
    m_TopSolenoid.Set(frc::DoubleSolenoid::kReverse);
    m_BottomSolenoid.Set(frc::DoubleSolenoid::kReverse);
}

void subsystem_Arm::LockArm(){
    m_TopSolenoid.Set(frc::DoubleSolenoid::kForward);
    m_BottomSolenoid.Set(frc::DoubleSolenoid::kForward);
}

bool subsystem_Arm::IsTopArmDirectionGoingUp(double x, double y)
{
    double adjustedX = x + ArmConstants::xOriginAdjustment;
    double adjustedY = y + ArmConstants::yOriginAdjustment;

    if (m_TopAbsEncoder.GetPosition() < (CalculateTopArmAngle(adjustedX, adjustedY) * ArmConstants::radiansToEncoder))
    {
        return true;
    }
    else if (m_TopAbsEncoder.GetPosition() > (CalculateTopArmAngle(adjustedX, adjustedY) * ArmConstants::radiansToEncoder))
    {
        return false;
    }
    return true;
}

bool subsystem_Arm::IsBottomArmDirectionGoingUp(double x, double y)
{
    double adjustedX = x + ArmConstants::xOriginAdjustment;
    double adjustedY = y + ArmConstants::yOriginAdjustment;

    if (m_BottomAbsEncoder.GetPosition() < (CalculateBottomArmAngle(adjustedX, adjustedY) * ArmConstants::radiansToEncoder)) // Positive direction
    {
        return true;
    }
    else if (m_BottomAbsEncoder.GetPosition() > (CalculateTopArmAngle(adjustedX, adjustedY) * ArmConstants::radiansToEncoder)) // Negative Direction
    {
        return false;
    }
    return true; 
}

bool subsystem_Arm::IsAtDesiredPosition()
{
    if (fabs(bottomPosition - convertedBottom) < ArmConstants::bufferZone && fabs(topPosition - convertedTop) < ArmConstants::bufferZone)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double subsystem_Arm::DegreesToSmartMotion(double degrees)
{
    return degrees * ArmConstants::DegreesToSmartMotion;
}

double subsystem_Arm::EncoderToDegrees(double ticks)
{
    return (ticks - ArmConstants::TicksOffset) * ArmConstants::TicksToDegrees;
}

void subsystem_Arm::Periodic()
{

    bottomPosition = m_BottomRelEncoder.GetPosition() + (bottomStartPos * ArmConstants::AbsToRel);
    topPosition = m_TopRelEncoder.GetPosition() + (topStartPos * ArmConstants::AbsToRel);
    intakeAngleOffset = topPosition * ArmConstants::intakeAngleConversion; /* - some constant bleh */

    frc::SmartDashboard::PutNumber("Bottom Pos:", m_BottomRelEncoder.GetPosition());
    frc::SmartDashboard::PutNumber("Top Pos", m_TopRelEncoder.GetPosition());
}