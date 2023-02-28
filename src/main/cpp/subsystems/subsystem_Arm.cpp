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
                                 m_BottomFollowerPID{m_BottomFollower.GetPIDController()},
                                 m_TopArmPID{m_TopArmMotor.GetPIDController()},
                                 m_TopFollowerPID{m_TopFollower.GetPIDController()},
                                 m_IntakeTiltPID{m_IntakeTiltMotor.GetPIDController()},
                                 m_BottomRelEncoder{m_BottomArmMotor.GetEncoder()},
                                 m_TopRelEncoder{m_TopArmMotor.GetEncoder()},
                                 m_BottomRelFollowerEncoder{m_BottomFollower.GetEncoder()},
                                 m_TopRelFollowerEncoder{m_TopFollower.GetEncoder()},
                                 m_BackLimit{m_BottomArmMotor.GetReverseLimitSwitch(rev::CANDigitalInput::LimitSwitchPolarity::kNormallyOpen)},
                                 m_FrontLimit{m_BottomArmMotor.GetForwardLimitSwitch(rev::CANDigitalInput::LimitSwitchPolarity::kNormallyOpen)},
                                 m_TopSolenoid{frc::PneumaticsModuleType::CTREPCM, ArmConstants::TopBrake1, ArmConstants::TopBrake2},
                                 m_BottomSolenoid{frc::PneumaticsModuleType::CTREPCM, ArmConstants::BottomBrake1, ArmConstants::BottomBrake2}
{
    m_BottomArmMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_TopArmMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_BottomFollower.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_TopFollower.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_IntakeTiltMotor.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);

    m_BottomArmMotor.SetSmartCurrentLimit(ArmConstants::ArmCurrentLimit);
    m_BottomFollower.SetSmartCurrentLimit(ArmConstants::ArmCurrentLimit);

    m_TopArmMotor.SetSmartCurrentLimit(ArmConstants::ArmCurrentLimit);
    m_TopFollower.SetSmartCurrentLimit(ArmConstants::ArmCurrentLimit);

    m_BottomFollower.Follow(m_BottomArmMotor);
    m_TopFollower.Follow(m_TopArmMotor);
    m_TopArmMotor.SetInverted(true);
    // m_BottomArmMotor.SetInverted(true);

    m_BottomArmPID.SetP(ArmConstants::kBottomGoingUpP, 0);
    m_BottomArmPID.SetD(ArmConstants::kBottomGoingUpD, 0);
    // m_BottomArmPID.SetFF(ArmConstants::kBottomGoingUpF, 0);
    m_TopArmPID.SetP(ArmConstants::kTopGoingUpP, 0);
    m_TopArmPID.SetD(ArmConstants::kTopGoingUpD, 0);
    // m_TopArmPID.SetFF(ArmConstants::kTopGoingUpF, 0);

    // m_BottomArmPID.SetP(ArmConstants::kBottomGoingDownP, 1);
    // m_BottomArmPID.SetD(ArmConstants::kBottomGoingDownD, 1);
    // m_BottomArmPID.SetFF(ArmConstants::kBottomGoingDownF, 1);
    // m_TopArmPID.SetP(ArmConstants::kTopGoingDownP, 1);
    // m_TopArmPID.SetD(ArmConstants::kTopGoingDownD, 1);
    // m_TopArmPID.SetFF(ArmConstants::kTopGoingDownF, 1);

    // m_BottomArmPID.SetSmartMotionMaxAccel(ArmConstants::kBottomMaxA);
    // m_BottomArmPID.SetSmartMotionMaxVelocity(ArmConstants::kBottomMaxV);
    // m_BottomArmPID.SetSmartMotionMinOutputVelocity(ArmConstants::kBottomMinV);

    // m_TopArmPID.SetSmartMotionMaxAccel(ArmConstants::kTopMaxA);
    // m_TopArmPID.SetSmartMotionMaxVelocity(ArmConstants::kTopMaxV);
    // m_TopArmPID.SetSmartMotionMinOutputVelocity(ArmConstants::kTopMinV);

    // frc::SmartDashboard::PutNumber("Bottom Arm Moving Down P", 0);
    // frc::SmartDashboard::PutNumber("Bottom Arm Moving Down D", 0);
    // frc::SmartDashboard::PutNumber("Bottom Arm Moving Down FF", 0);

    // topStartPos = m_TopAbsEncoder.GetPosition();
    // bottomStartPos = m_BottomAbsEncoder.GetPosition();
}

double subsystem_Arm::CalculateBottomArmAngle(double x, double y)
{
    double topAngle = CalculateTopArmAngle(x, y);
    return (atan(y / x) - atan((ArmConstants::topJointLength * sin(topAngle)) / (ArmConstants::bottomJointLength + (ArmConstants::topJointLength * cos(topAngle)))));
}

double subsystem_Arm::CalculateTopArmAngle(double x, double y)
{
    return (-acos((pow(x, 2) + pow(y, 2) - pow(ArmConstants::topJointLength, 2) - pow(ArmConstants::bottomJointLength, 2)) / (2 * ArmConstants::topJointLength * ArmConstants::bottomJointLength)));
}

void subsystem_Arm::MoveArm(double x, double y)
{
    armX = x;
    armY = y;
    adjustedX = x - ArmConstants::xOriginAdjustment;
    adjustedY = y - ArmConstants::yOriginAdjustment;

    SetArmPIDByDirection(x, y);

    if (adjustedX <= ArmConstants::totalArmLength && adjustedY <= ArmConstants::totalArmLength)
    {
        double bottom = CalculateBottomArmAngle(adjustedX, adjustedY) - 180.0;
        double top = (bottom - CalculateTopArmAngle(adjustedX, adjustedY));
        convertedBottom = bottom * ArmConstants::DegreesToRotations;
        convertedTop = top * ArmConstants::DegreesToRotations;
        frc::SmartDashboard::PutString("ARM MOVING", "ayush");
        frc::SmartDashboard::PutNumber("Desired Bottom Arm", convertedBottom);
        m_BottomArmPID.SetReference(convertedBottom, rev::ControlType::kSmartMotion);
        m_TopArmPID.SetReference(convertedTop, rev::ControlType::kSmartMotion);
    }
    else
    {
        printf("INVALID X OR Y INPUT");
    }
}

void subsystem_Arm::RunBottomArmTest()
{
    m_BottomArmPID.SetReference(-14.0, rev::ControlType::kSmartMotion);
}

void subsystem_Arm::MoveArmManually(double leftAxis, double rightAxis)
{
    manualX = armX + (leftAxis * ArmConstants::JoystickToArm);
    manualY = armY + (rightAxis * ArmConstants::JoystickToArm);
    MoveArm(manualX, manualY);
}
 
void subsystem_Arm::ManualMacroSwitch()
{
    m_IsManual = !m_IsManual;
}

bool subsystem_Arm::IsManual()
{
    return m_IsManual;
}

void subsystem_Arm::SetIntakeAngle(double angle)
{
    double adjustedAngle = angle - intakeAngleOffset;
    m_IntakeTiltPID.SetReference(adjustedAngle, rev::ControlType::kPosition, 0);
}

void subsystem_Arm::SetArmPIDByDirection(double x, double y)
{
    if (IsBottomArmDirectionGoingUp(x, y))
    {
        m_BottomArmSlot = 0;
    }
    else
    {
        m_BottomArmSlot = 1;
    }
    if (IsTopArmDirectionGoingUp(x, y))
    {
        m_TopArmSlot = 0;
    }
    else
    {
        m_TopArmSlot = 1;
    }
}

// void subsystem_Arm::UnlockArm()
// {
//     m_TopSolenoid.Set(frc::DoubleSolenoid::kReverse);
//     m_BottomSolenoid.Set(frc::DoubleSolenoid::kReverse);
// }

// void subsystem_Arm::LockArm()
// {
//     m_TopSolenoid.Set(frc::DoubleSolenoid::kForward);
//     m_BottomSolenoid.Set(frc::DoubleSolenoid::kForward);
// }

bool subsystem_Arm::IsTopArmDirectionGoingUp(double x, double y)
{
    double adjustedX = x + ArmConstants::xOriginAdjustment;
    double adjustedY = y + ArmConstants::yOriginAdjustment;

    if (topPosition < (CalculateTopArmAngle(adjustedX, adjustedY) * ArmConstants::DegreesToRotations))
    {
        return true;
    }
    else if (topPosition > (CalculateTopArmAngle(adjustedX, adjustedY) * ArmConstants::DegreesToRotations))
    {
        return false;
    }
    return true;
}

bool subsystem_Arm::IsBottomArmDirectionGoingUp(double x, double y)
{
    double adjustedX = x + ArmConstants::xOriginAdjustment;
    double adjustedY = y + ArmConstants::yOriginAdjustment;

    if (bottomPosition < (CalculateBottomArmAngle(adjustedX, adjustedY) * ArmConstants::DegreesToRotations)) // Positive direction
    {
        return true;
    }
    else if (bottomPosition > (CalculateTopArmAngle(adjustedX, adjustedY) * ArmConstants::DegreesToRotations)) // Negative Direction
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


double subsystem_Arm::EncoderToDegrees(double ticks)
{
    return (ticks - ArmConstants::TicksOffset) * ArmConstants::TicksToDegrees;
}

void subsystem_Arm::Periodic()
{

    bottomPosition = m_BottomRelEncoder.GetPosition() /*+ (bottomStartPos * ArmConstants::AbsToRel)*/;
    topPosition = m_TopRelEncoder.GetPosition() /* + (topStartPos * ArmConstants::AbsToRel)*/;
    // intakeAngleOffset = topPosition * ArmConstants::intakeAngleConversion; /* - some constant bleh */

    // frc::SmartDashboard::PutNumber("Bottom Pos:", m_BottomRelEncoder.GetPosition());
    // frc::SmartDashboard::PutNumber("Top Pos", m_TopRelEncoder.GetPosition());

    // if (!m_BackLimitSwitch.Get())
    // {
    //     m_BottomArmPID.SetReference(0, rev::ControlType::kVoltage);
    //     m_BottomArmPID.SetReference(ArmConstants::ArmBackLimit, rev::ControlType::kPosition);
    // }
    // if (!m_FrontLimitSwitch.Get())
    // {
    //     m_BottomArmPID.SetReference(0, rev::ControlType::kVoltage);
    //     m_BottomArmPID.SetReference(ArmConstants::ArmFrontLimit, rev::ControlType::kPosition);
    // }

    frc::SmartDashboard::PutNumber("Bottom Arm Encoder", bottomPosition);
    frc::SmartDashboard::PutNumber("Top Arm Encoder", topPosition);

    // frc::SmartDashboard::PutNumber("Bottom Arm Moving Up P", 0);
    // frc::SmartDashboard::PutNumber("Bottom Arm Moving Up D", 0);
    // frc::SmartDashboard::PutNumber("Bottom Arm Moving Up FF", 0);
    // m_BottomArmPID.SetP(frc::SmartDashboard::GetNumber("Bottom Arm Moving Up P", 0), 0);
    // m_BottomArmPID.SetD(frc::SmartDashboard::GetNumber("Bottom Arm Moving Up D", 0), 0);
    // m_BottomArmPID.SetFF(frc::SmartDashboard::GetNumber("Bottom Arm Moving Up FF", 0), 0);

    // m_BottomArmPID.SetP(frc::SmartDashboard::GetNumber("Bottom Arm Moving Down P", 0), 1);
    // m_BottomArmPID.SetD(frc::SmartDashboard::GetNumber("Bottom Arm Moving Down D", 0), 1);
    // m_BottomArmPID.SetFF(frc::SmartDashboard::GetNumber("Bottom Arm Moving Down FF", 0), 1);

    // frc::SmartDashboard::PutNumber("Top Arm Moving Up P", 0);
    // frc::SmartDashboard::PutNumber("Top Arm Moving Up D", 0);
    // frc::SmartDashboard::PutNumber("Top Arm Moving Up FF", 0);
    // m_TopArmPID.SetP(frc::SmartDashboard::GetNumber("Top Arm Moving Up P", 0), 0);
    // m_TopArmPID.SetD(frc::SmartDashboard::GetNumber("Top Arm Moving Up D", 0), 0);
    // m_TopArmPID.SetFF(frc::SmartDashboard::GetNumber("Top Arm Moving Up FF", 0), 0);

    //     frc::SmartDashboard::PutNumber("Top Arm Moving Down P", 0);
    // frc::SmartDashboard::PutNumber("Top Arm Moving Down D", 0);
    // frc::SmartDashboard::PutNumber("Top Arm Moving Down FF", 0);
    // m_TopArmPID.SetP(frc::SmartDashboard::GetNumber("Top Arm Moving Down P", 0), 1);
    // m_TopArmPID.SetD(frc::SmartDashboard::GetNumber("Top Arm Moving Down D", 0), 1);
    // m_TopArmPID.SetFF(frc::SmartDashboard::GetNumber("Top Arm Moving Down FF", 0), 1);
}