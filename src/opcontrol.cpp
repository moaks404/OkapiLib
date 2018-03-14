#include "api.h"

#include "okapi/chassis/chassisController.hpp"
#include "okapi/chassis/chassisControllerIntegrated.hpp"
#include "okapi/chassis/chassisModel.hpp"
#include "okapi/chassis/odomChassisController.hpp"
#include "okapi/chassis/skidSteerModel.hpp"
#include "okapi/chassis/xDriveModel.hpp"

#include "okapi/control/motorController.hpp"
#include "okapi/control/pidController.hpp"
#include "okapi/control/velMath.hpp"
#include "okapi/control/velPidController.hpp"

#include "okapi/device/adiButton.hpp"
#include "okapi/device/adiEncoder.hpp"
#include "okapi/device/controllerButton.hpp"
#include "okapi/device/motor.hpp"
#include "okapi/device/motorGroup.hpp"

#include "okapi/filter/averageFilter.hpp"
#include "okapi/filter/demaFilter.hpp"
#include "okapi/filter/emaFilter.hpp"

#include "okapi/odometry/odomMath.hpp"
#include "okapi/odometry/odometry.hpp"

#include "okapi/util/timer.hpp"

void opcontrol() {
  while (true) {
    okapi::ADIButton btn(2);
    btn.isPressed();
    btn.edge();
    btn.risingEdge();
    btn.fallingEdge();

    okapi::ControllerButton btn2(E_CONTROLLER_MASTER, E_CONTROLLER_DIGITAL_A);

    {
      using namespace okapi;

      okapi::Motor mtr = 1_m;
      Motor r_mtr = 2_rm;

      SkidSteerModel model1(MotorGroup<2>({1_m, 2_m}), MotorGroup<2>({3_m, 4_m}),
                            ADIEncoder(1, 2, true), ADIEncoder(3, 4));

      ChassisControllerIntegrated int1(1_m, 2_m);
      ChassisControllerIntegrated int2(MotorGroup<3>({1_m, 2_m, 3_m}), MotorGroup<2>({4_m, 5_m}));

      XDriveModel model2(MotorGroup<1>({1_m}), // {2_m, 3_m, 4_m, 5_m}
                         MotorGroup<1>({2_m}), MotorGroup<1>({3_m}), MotorGroup<1>({4_m}),
                         ADIEncoder(1, 2, true), ADIEncoder(3, 4));

      ChassisControllerPID controller1(
        SkidSteerModelParams(MotorGroup<2>({1_m, 2_m}), MotorGroup<2>({3_m, 4_m}),
                             ADIEncoder(1, 2, true), ADIEncoder(3, 4)),
        PIDControllerParams(0, 0, 0), PIDControllerParams(0, 0, 0));

      ChassisControllerPID controller2(
        XDriveModelParams(MotorGroup<1>({1_m}), // {2_m, 3_m, 4_m, 5_m}
                          MotorGroup<1>({2_m}), MotorGroup<1>({3_m}), MotorGroup<1>({4_m}),
                          ADIEncoder(1, 2, true), ADIEncoder(3, 4)),
        PIDControllerParams(0, 0, 0), PIDControllerParams(0, 0, 0));

      OdomChassisControllerPID controller3(
        OdometryParams(SkidSteerModelParams(MotorGroup<2>({1_m, 2_m}), MotorGroup<2>({3_m, 4_m}),
                                            ADIEncoder(1, 2, true), ADIEncoder(3, 4)),
                       0, 0),
        PIDControllerParams(0, 0, 0), PIDControllerParams(0, 0, 0));

      OdomChassisControllerPID controller4(
        OdometryParams(XDriveModelParams(MotorGroup<1>({1_m}), // {2_m, 3_m, 4_m, 5_m}
                                         MotorGroup<1>({2_m}), MotorGroup<1>({3_m}),
                                         MotorGroup<1>({4_m}), ADIEncoder(1, 2, true),
                                         ADIEncoder(3, 4)),
                       0, 0),
        PIDControllerParams(0, 0, 0), PIDControllerParams(0, 0, 0));

      PIDController pid1(0, 0, 0);
      MotorController mgController(MotorGroup<2>({1_m, 2_m}), pid1);

      PIDController pid2(0, 0, 0);
      PIDController pid3(0, 0, 0, 0);
      PIDController pid4(PIDControllerParams(0, 0, 0));
      PIDController pid5(PIDControllerParams(0, 0, 0, 0));

      VelMath velMath1(0);
      VelMath velMath2(0, 0);
      VelMath velMath3(0, 0, 0);

      VelPIDController velPid1(0, 0);
      VelPIDController velPid2(VelPIDControllerParams(0, 0));

      okapi::ADIEncoder quad1(0, 0);
      okapi::ADIEncoder quad2(0, 0, true);

      // MotorGroup<2> mg1({1_m, 2_m});

      AverageFilter<1> avgFilt1;
      avgFilt1.filter(0);
      avgFilt1.getOutput();

      DemaFilter demaFilt1(0, 0);

      EmaFilter emaFilt1(0);

      Odometry odom1(SkidSteerModelParams(MotorGroup<2>({1_m, 2_m}), MotorGroup<2>({3_m, 4_m}),
                                          ADIEncoder(1, 2, true), ADIEncoder(3, 4)),
                     0, 0);

      Timer timer1();
    }
  }
}
