#include "chassis.h"

chassis& chassis::GetInstance(){
    static chassis Chassis;
    return Chassis;
}

chassis::chassis(){
    liner_velocity = angular_velocity = 0;
    left_voltage = right_voltage = 0;
    yaw = rotation = (Inertial_1.rotation() + Inertial_2.rotation())/2;
    temperature = max_temperature = 0;
}

void chassis::chassis_brake(){
    LF.stop(chassis_braketype);
    LMF.stop(chassis_braketype);
    LMB.stop(chassis_braketype);
    LB.stop(chassis_braketype);
    RF.stop(chassis_braketype);
    RMF.stop(chassis_braketype);
    RMB.stop(chassis_braketype);
    RB.stop(chassis_braketype);
}

void chassis::chassis_move(){
    LF.spin(fwd , left_voltage , voltageUnits::mV);
    LMF.spin(fwd , left_voltage , voltageUnits::mV);
    LMB.spin(fwd , left_voltage , voltageUnits::mV);
    LB.spin(fwd , left_voltage , voltageUnits::mV);

    RF.spin(fwd , right_voltage , voltageUnits::mV);
    RMF.spin(fwd , right_voltage , voltageUnits::mV);
    RMB.spin(fwd , right_voltage , voltageUnits::mV);
    RB.spin(fwd , right_voltage , voltageUnits::mV);

    if(abs(left_voltage) <= MIN_DRIVE_VOLTAGE && abs(right_voltage) <= MIN_DRIVE_VOLTAGE){
        chassis_brake();
    }
}

void chassis::yaw_filter(){
    if(Inertial_1.rotation() - rotation <= MAX_FILTER_ERROR && Inertial_2.rotation() - rotation <= MAX_FILTER_ERROR){
        rotation = (Inertial_1.rotation() + Inertial_2.rotation())/2;
    }
}

void chassis::set_braketype(brakeType braketype){
    chassis_braketype = braketype;
}

void chassis::set_velocity(int lv , int av){
    liner_velocity = lv; angular_velocity = av;
}

void chassis::set_voltage(int left_v , int right_v){
    left_voltage = left_v; right_voltage = right_v;
}

int chassis::get_yaw_limit(){
    if(rotation > 0){yaw = rotation % 360;}
    else if(rotation < 0){yaw = 360 - (abs(rotation) % 360);}
    else {yaw = 0;}
    return yaw;
}

int chassis::get_yaw_no_limit(){
    return rotation;
}

int chassis::get_temperature(){
    return temperature;
}

int chassis::get_max_temperature(){
    return max_temperature;
}

bool chassis::get_connection_state(){
    return connection_state;
}

void chassis::temperature_monitoring(){
    temperature = 0;
    temperature_list[0] = LF.temperature(celsius);
    temperature_list[1] = LMF.temperature(celsius);
    temperature_list[2] = LMB.temperature(celsius);
    temperature_list[3] = LB.temperature(celsius);
    temperature_list[4] = RF.temperature(celsius);
    temperature_list[5] = RMF.temperature(celsius);
    temperature_list[6] = RMB.temperature(celsius);
    temperature_list[7] = RB.temperature(celsius);

    for(int i=0 ; i<8 ; i++){
        temperature += temperature_list[i];
        max_temperature = (
            temperature_list[i] > max_temperature ? 
            temperature_list[i] :
            max_temperature
        );
    }
    temperature = temperature/8;
}

void chassis::connection_monitoring(){
    connection_state_list[0] = LF.installed();
    connection_state_list[1] = LMF.installed();
    connection_state_list[2] = LMB.installed();
    connection_state_list[3] = LB.installed();
    connection_state_list[4] = RF.installed();
    connection_state_list[5] = RMF.installed();
    connection_state_list[6] = RMB.installed();
    connection_state_list[7] = RB.installed();
    connection_state_list[8] = Inertial_1.installed();
    connection_state_list[9] = Inertial_2.installed();

    for(int i=0 ; i<10 ; i++){
        if(connection_state_list[i] == 0){connection_state = false; break;}
        if(connection_state_list[9] == 1){connection_state = true;}
    }
    
}

void chassis_drive_thread(){
    while(true){
        chassis::GetInstance().chassis_move();
        chassis::GetInstance().yaw_filter();
        chassis::GetInstance().temperature_monitoring();
        chassis::GetInstance().connection_monitoring();
        wait(5,msec);
    }
}