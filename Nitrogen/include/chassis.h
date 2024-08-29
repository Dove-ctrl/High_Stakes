#pragma once

#include "vex.h"
#include "PID.h"

class chassis{
private:
    chassis();
    chassis(const chassis &_chassis) = delete;
    const chassis &operator=(const chassis &_chassis) = delete;

    int liner_velocity , angular_velocity;
    int left_voltage , right_voltage;
    int yaw , rotation;
    int temperature , max_temperature; int temperature_list[8];
    int connection_state_list[10]; bool connection_state;
    brakeType chassis_braketype;

public:
    static chassis& GetInstance();

    void yaw_filter();
    void temperature_monitoring();
    void connection_monitoring();

    void chassis_move();
    void chassis_brake();
    void set_braketype(brakeType chassis_braketype);
    void set_velocity(int lv , int av);
    void set_voltage(int left_v , int right_v);

    int get_yaw_limit();
    int get_yaw_no_limit();
    int get_temperature();
    int get_max_temperature();
    bool get_connection_state();
};

void chassis_drive_thread();