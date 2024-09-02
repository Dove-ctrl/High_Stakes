#pragma once
using namespace vex;

//auto_control
const int MAX_MOVE_TIME = 1500;
const int MIN_DRIVE_VOLTAGE = 200;
const float HALF_WHEEL_DISTANCE = 17;
const int MAX_FILTER_ERROR = 5;

//op_control
const int K_AXIS_3 = 130;
const int K_AXIS_1 = 80;

//elementOS
const int TIMEOUT_WRRNING = 30;
const int TEMPERATURE_WRRNING = 50;
const int DEVICE_NUMMER = 10;

//---------------------------------------------------------------//
extern brain Brain;
extern controller Controller;

extern motor* LF;
extern motor* LMF;
extern motor* LMB;
extern motor* LB;

extern motor* RF;
extern motor* RMF;
extern motor* RMB;
extern motor* RB;

extern inertial* Inertial_1;
extern inertial* Inertial_2;

extern encoder* encoderX;
extern encoder* encoderY;

void initialize_robot();