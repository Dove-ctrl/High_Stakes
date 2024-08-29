#include "vex.h"
#include "robot-config.h"

using namespace vex;

vex::triport::port null_tport;
uint32_t null_port;

brain Brain;
controller Controller = controller(primary);

motor null_motor = motor(null_port);
inertial null_inertial = inertial(null_port);
encoder null_encoder = encoder(null_tport);

motor* LF = &null_motor;
motor* LMF = &null_motor;
motor* LMB = &null_motor;
motor* LB = &null_motor;
motor* RF = &null_motor;
motor* RMF = &null_motor;
motor* RMB = &null_motor;
motor* RB = &null_motor;

inertial* Inertial_1 = &null_inertial;
inertial* Inertial_2 = &null_inertial;

encoder* encoderX = &null_encoder;
encoder* encoderY = &null_encoder;

void initialize_robot(){
    Inertial_1->calibrate();
    waitUntil(!Inertial_1->isCalibrating());
    Inertial_2->calibrate();
    waitUntil(!Inertial_2->isCalibrating());
    LF->resetPosition();
    RF->resetPosition();
    encoderX->resetRotation();
    encoderY->resetRotation();
}