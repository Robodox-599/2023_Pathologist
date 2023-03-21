#include "commands/cGroup_Arm.h"
#include <frc2/command/Commands.h>

frc2::CommandPtr ArmMovements::ToHighCone(subsystem_Arm *Arm) {
  return frc2::cmd::Sequence(command_MoveElbow(Arm, [=]{return 26;}, [=]{return true;}).ToPtr(),
                             command_MoveShoulder(Arm, [=]{return ArmConstants::HighConeShoulder;}, [=]{return false;}).ToPtr(),
                             command_MoveElbow(Arm, [=]{return ArmConstants::HighConeElbow;}, [=]{return false;}).ToPtr(),
                             command_MoveWrist(Arm, [=]{return ArmConstants::HighConeTilt;}, [=]{return false;}).ToPtr());
}

frc2::CommandPtr ArmMovements::ToMidCone(subsystem_Arm *Arm){
  return frc2::cmd::Sequence(command_MoveElbow(Arm, [=]{return ArmConstants::MidConeElbow;}, [=]{return true;}).ToPtr(),
                             command_MoveShoulder(Arm, [=]{return ArmConstants::MidConeShoulder;}, [=]{return false;}).ToPtr(),
                             command_MoveWrist(Arm, [=]{return ArmConstants::MidConeTilt;}, [=]{return false;}).ToPtr());

}
frc2::CommandPtr ArmMovements::ToHighCube(subsystem_Arm *Arm){
  return frc2::cmd::Sequence(command_MoveElbow(Arm, [=]{return ArmConstants::HighCubeElbow;}, [=]{return true;}).ToPtr(),
                             command_MoveShoulder(Arm, [=]{return ArmConstants::HighCubeShoulder;}, [=]{return false;}).ToPtr(),
                             command_MoveWrist(Arm, [=]{return ArmConstants::HighCubeTilt;}, [=]{return false;}).ToPtr());

}
frc2::CommandPtr ArmMovements::ToMidCube(subsystem_Arm *Arm){
  return frc2::cmd::Sequence(command_MoveElbow(Arm, [=]{return ArmConstants::MidCubeElbow;}, [=]{return true;}).ToPtr(),
                             command_MoveShoulder(Arm, [=]{return ArmConstants::MidCubeShoulder;}, [=]{return false;}).ToPtr(),
                             command_MoveWrist(Arm, [=]{return ArmConstants::MidCubeTilt;}, [=]{return false;}).ToPtr());

}
frc2::CommandPtr ArmMovements::ToSubstation(subsystem_Arm *Arm){
  return frc2::cmd::Sequence(command_MoveElbow(Arm, [=]{return ArmConstants::SubstationElbow;}, [=]{return true;}).ToPtr(),
                             command_MoveShoulder(Arm, [=]{return ArmConstants::SubstationShoulder;}, [=]{return true;}).ToPtr(),
                             command_MoveWrist(Arm, [=]{return ArmConstants::SubstationTilt;}, [=]{return false;}).ToPtr());
}

frc2::CommandPtr ArmMovements::ToStow(subsystem_Arm *Arm){
  return frc2::cmd::Sequence(
                             command_MoveShoulder(Arm, [=]{return ArmConstants::StowShoulder;}, [=]{return true;}).ToPtr(),
                             command_MoveWrist(Arm, [=]{return ArmConstants::StowTilt;}, [=]{return false;}).ToPtr(),
                             command_MoveElbow(Arm, [=]{return ArmConstants::StowElbow;}, [=]{return false;}).ToPtr());
}

frc2::CommandPtr ArmMovements::ToGround(subsystem_Arm *Arm){
  return frc2::cmd::Sequence(
                             command_MoveElbow(Arm, [=]{return ArmConstants::TempElbow;}, [=]{return true;}).ToPtr(),
                             command_MoveShoulder(Arm, [=]{return ArmConstants::GroundShoulder;}, [=]{return false;}).ToPtr(),
                             command_MoveWrist(Arm, [=]{return ArmConstants::GroundTilt;}, [=]{return false;}).ToPtr(),
                             command_MoveElbow(Arm, [=]{return ArmConstants::GroundElbow;}, [=]{return false;}).ToPtr());
    
}

frc2::CommandPtr ArmMovements::ToFloorCube(subsystem_Arm *Arm){
  return frc2::cmd::Sequence(
                             command_MoveShoulder(Arm, [=]{return ArmConstants::StowShoulder;}, [=]{return false;}).ToPtr(),
                             command_MoveElbow(Arm, [=]{return ArmConstants::StowElbow;}, [=]{return false;}).ToPtr(),
                             command_MoveWrist(Arm, [=]{return ArmConstants::floorCubeTilt;}, [=]{return false;}).ToPtr());
}

frc2::CommandPtr ArmMovements::ScoreAndStow(subsystem_Arm *Arm, subsystem_Intake *Intake){
    return frc2::cmd::Sequence(
                              command_MoveWrist(Arm, [=]{return ArmConstants::ScoreTilt;}, [=]{return true;}).ToPtr(),
                              command_OuttakeObject(Intake).ToPtr(),
                              ArmMovements::ToStow(Arm));
}


