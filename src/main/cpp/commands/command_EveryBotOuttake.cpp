// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "commands/command_EveryBotOuttake.h"

command_EveryBotOuttake::command_EveryBotOuttake(subsystem_EveryBotIntake* EveryBotIntake): m_EveryBotIntake{EveryBotIntake} {
  // Use addRequirements() here to declare subsystem dependencies.
  AddRequirements({m_EveryBotIntake});
}

// Called when the command is initially scheduled.
void command_EveryBotOuttake::Initialize() {
  m_EveryBotIntake->SetIntake();
}

// Called repeatedly when this Command is scheduled to run
void command_EveryBotOuttake::Execute() {}

// Called once the command ends or is interrupted.
void command_EveryBotOuttake::End(bool interrupted) {
  m_EveryBotIntake->SetOff();
}

// Returns true when the command should end.
bool command_EveryBotOuttake::IsFinished() {
  return false;
  
}
