// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/subsystem_DriveTrain.h"
#include "subsystems/subsystem_PoseTracker.h"

#include <frc/controller/PIDController.h>
#include <units/length.h>
#include <units/angle.h>


/**
 * An example command.
 *
 * <p>Note that this extends CommandHelper, rather extending CommandBase
 * directly; this is crucially important, or else the decorator functions in
 * Command will *not* work!
 */
class command_AlignToDesired
    : public frc2::CommandHelper<frc2::CommandBase, command_AlignToDesired> {
 public:
  command_AlignToDesired(subsystem_DriveTrain* DriveTrain, subsystem_PoseTracker* PoseTracker, std::function<double()> XDesired, std::function<double()> YDesired, std::function<double()> ThetaDesired);

  void Initialize() override;

  void Execute() override;

  void End(bool interrupted) override;

  bool IsFinished() override;

  private:
  subsystem_DriveTrain* m_DriveTrain;
  subsystem_PoseTracker* m_PoseTracker;
  std::function<double()>  m_X;
  std::function<double()>  m_Y;
  std::function<double()>  m_Theta;

  
  frc::PIDController XPID {0.5, 0, 0};
  frc::PIDController YPID {0.5, 0, 0};
  frc::PIDController ThetaPID {0.5, 0, 0};
};
