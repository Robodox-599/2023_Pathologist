// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/Autos.h"

#include <frc2/command/Commands.h>


frc2::CommandPtr autos::ThreeScoreAuto(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker, subsystem_Arm* Arm, subsystem_Intake* Intake){
  return frc2::cmd::Sequence( ArmMovements::HighConeScoreAndStow(Arm, Intake), 
                            frc2::cmd::Parallel(command_DriveAuton(DriveTrain, PoseTracker, "Yes^3 First Option Pt 1", true).ToPtr(), 
                                                frc2::cmd::Sequence(frc2::WaitCommand(1.0_s), 
                                                                    Intake->UnclampCommand())/* Intake Cube and Stop Wheels */),
                                                Intake->ClampCommand() /* Outtake Cube */,
                                                frc2::cmd::Parallel(command_DriveAuton(DriveTrain, PoseTracker, "Yes^3 First Option Pt 2", true).ToPtr(), 
                                                                    frc2::cmd::Sequence(frc2::WaitCommand(1.0_s), 
                                                                                        Intake->UnclampCommand())/* Intake Cube and Stop Wheels */),
                            command_AutoClamp(Intake).ToPtr() /* Outtake Cube */); 
}

frc2::CommandPtr autos::ThreeScoreAuto2(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker, subsystem_Arm* Arm, subsystem_Intake* Intake){
  return frc2::cmd::Sequence( ArmMovements::HighConeScoreAndStow(Arm, Intake), 
                            frc2::cmd::Parallel(command_DriveAuton(DriveTrain, PoseTracker, "Yes^3 Second Option Pt 1", true).ToPtr(), 
                                                frc2::cmd::Sequence(frc2::WaitCommand(1.0_s), 
                                                                    Intake->UnclampCommand())/* Intake Cube and Stop Wheels */),
                                                Intake->ClampCommand() /* Outtake Cube */,
                                                frc2::cmd::Parallel(command_DriveAuton(DriveTrain, PoseTracker, "Yes^3 Second Option Pt 2", true).ToPtr(), 
                                                                    frc2::cmd::Sequence(frc2::WaitCommand(1.0_s), 
                                                                                        Intake->UnclampCommand())/* Intake Cube and Stop Wheels */),
                            command_AutoClamp(Intake).ToPtr() /* Outtake Cube */); 
}

frc2::CommandPtr autos::TestAuto(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker){
  return command_DriveAuton(DriveTrain, PoseTracker, "Test",  true).ToPtr();
}

/*
frc2::CommandPtr autos::Niemann(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker){
  return frc2::cmd::Sequence( command_DriveAuton(DriveTrain, PoseTracker, "Niemann (Yes^2)", true).ToPtr(), 
                              command_DriveAuton(DriveTrain, PoseTracker, "Niemann 2 (Yes^2)", false).ToPtr(),
                              command_DriveAuton(DriveTrain, PoseTracker, "Niemann 3 (Yes^2)", false).ToPtr());
}
*/
frc2::CommandPtr autos::Kasparov(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker){
  return frc2::cmd::Sequence( command_DriveAuton(DriveTrain, PoseTracker, "Kasparov (4T)", true).ToPtr() );
}


// frc2::CommandPtr ArmMovements::HighCubeScoreAndStow(subsystem_Intake* Intake, subsystem_Arm* Arm){
//   return frc2::cmd::Sequence(Intake->ClampCommand(),
//                             ArmMovements::ToHighCube(Arm),
//                             command_TimeOut([=]{return 1.0;}).ToPtr(),
//                             Intake->UnclampCommand(),
//                             ArmMovements::ToStow(Arm, Intake));
// }

// frc2::CommandPtr autos::ScoreMidCube(subsystem_Intake* Intake, subsystem_Arm* Arm){
//   return frc2::cmd::Sequence(Intake->ClampCommand(),
//                             ArmMovements::ToMidCube(Arm),
//                             command_TimeOut([=]{return 1.0;}).ToPtr(),
//                             Intake->UnclampCommand(),
//                             ArmMovements::ToStow(Arm, Intake));
// }

// frc2::CommandPtr autos::ScoreHighCone(subsystem_Intake* Intake, subsystem_Arm* Arm){
//   return frc2::cmd::Sequence(Intake->ClampCommand(),
//                             ArmMovements::ToHighCone(Arm),
//                             command_TimeOut([=]{return 1.0;}).ToPtr(),
//                             Intake->UnclampCommand(),
//                             ArmMovements::ToStow(Arm, Intake));
// }

// frc2::CommandPtr autos::ScoreMidCone(subsystem_Intake* Intake, subsystem_Arm* Arm){
//   return frc2::cmd::Sequence(Intake->ClampCommand(),
//                             ArmMovements::ToHighCube(Arm),
//                             command_TimeOut([=]{return 1.0;}).ToPtr(),
//                             Intake->UnclampCommand(),
//                             ArmMovements::ToStow(Arm, Intake));
// }


frc2::CommandPtr autos::OneScoreAndTaxi(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker, subsystem_Intake* Intake, subsystem_Arm* Arm){
    return frc2::cmd::Sequence(ArmMovements::HighCubeScoreAndStow(Arm, Intake), command_DriveAuton(DriveTrain, PoseTracker, "1ST", true).ToPtr());
}
frc2::CommandPtr autos::TwoScoreAndTaxi(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker, subsystem_Intake* Intake, subsystem_Arm* Arm){
    return frc2::cmd::Sequence(ArmMovements::HighCubeScoreAndStow(Arm, Intake), command_DriveAuton(DriveTrain, PoseTracker, "2ST", true).ToPtr());
}
frc2::CommandPtr autos::ThreeScoreAndTaxi(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker, subsystem_Intake* Intake, subsystem_Arm* Arm){
    return frc2::cmd::Sequence(ArmMovements::HighCubeScoreAndStow(Arm, Intake), command_DriveAuton(DriveTrain, PoseTracker, "3ST", true).ToPtr());
}
frc2::CommandPtr autos::ThreeTaxiAndBalance(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker){
    return frc2::cmd::Sequence(command_DriveAuton(DriveTrain, PoseTracker, "4TC", true).ToPtr(), command_Balance(DriveTrain).ToPtr());
    // return frc2::cmd::Sequence(command_DriveAuton(DriveTrain, PoseTracker, "4TC", true).ToPtr(), );

}
frc2::CommandPtr autos::TwoTaxiAndBalance(subsystem_DriveTrain*DriveTrain, subsystem_PoseTracker* PoseTracker){
    return frc2::cmd::Sequence(command_DriveAuton(DriveTrain, PoseTracker, "2TC", true).ToPtr(), command_Balance(DriveTrain).ToPtr());
}

frc2::CommandPtr autos::One_ScoreIntakeScore(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker, subsystem_Intake* Intake, subsystem_Arm* Arm){
  return frc2::cmd::Sequence(ArmMovements::HighCubeScoreAndStow(Arm, Intake), 
                            command_DriveAuton(DriveTrain, PoseTracker, "OneToTopPiece", true).ToPtr(), 
                            ArmMovements::ToGround(Arm, Intake),
                            command_TimeOut([=]{return 2.0;}).ToPtr(),
                            Intake->ClampCommand(),
                            ArmMovements::ToStow(Arm, Intake), 
                            command_DriveAuton(DriveTrain, PoseTracker, "TopPieceToOne", false).ToPtr(),
                            ArmMovements::MidCubeScoreAndStow(Arm, Intake));
}



frc2::CommandPtr autos::Two_ScoreTaxiAndBalance(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker, subsystem_Intake* Intake, subsystem_Arm* Arm){
  return frc2::cmd::Sequence(ArmMovements::HighCubeScoreAndStow(Arm, Intake), 
                              command_DriveAuton(DriveTrain, PoseTracker, "2STC", true).ToPtr(), 
                              command_Balance(DriveTrain).ToPtr());
}

frc2::CommandPtr autos::TestPickUp(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker, subsystem_Intake* Intake, subsystem_Arm* Arm){
  return frc2::cmd::Sequence( command_DriveAuton(DriveTrain, PoseTracker, "LinePath", true).ToPtr(),
                              Intake->UnclampCommand(),
                             ArmMovements::ToGround(Arm, Intake),
                             command_TimeOut([=]{return 1.5;}).ToPtr(),
                             command_AutoClamp(Intake).ToPtr(),
                             ArmMovements::ToPortal(Arm));
}

