#include "vex.h"

using namespace vex;

brain Brain;
controller Controller = controller(primary);

motor LF = motor(PORT7 , ratio6_1 , true);
motor LMF = motor(PORT8 , ratio6_1 , false);
motor LMB = motor(PORT9 , ratio6_1 , false);
motor LB = motor(PORT10 , ratio6_1 , true);
motor RF = motor(PORT4 , ratio6_1 , false);
motor RMF = motor(PORT3 , ratio6_1 , true);
motor RMB = motor(PORT2 , ratio6_1 , true);
motor RB = motor(PORT1 , ratio6_1 , false);

inertial Inertial_1 = inertial(PORT5);
inertial Inertial_2 = inertial(PORT6);

encoder encoderX = encoder(Brain.ThreeWirePort.A);
encoder encoderY = encoder(Brain.ThreeWirePort.C);

void initialize_robot(){
    Inertial_1.calibrate();
    waitUntil(!Inertial_1.isCalibrating());
    Inertial_2.calibrate();
    waitUntil(!Inertial_2.isCalibrating());
    LF.resetPosition();
    RF.resetPosition();
    encoderX.resetRotation();
    encoderY.resetRotation();
}