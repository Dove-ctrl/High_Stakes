#include "opfunction.h"

void op_move(void* p){
    competition* cpt = (competition*)p;
    int v1 = 0 , v3 = 0;
    chassis::GetInstance().set_braketype(coast);
    while(cpt->isDriverControl()){
        v1 = Controller.Axis1.position();
        v3 = Controller.Axis3.position();
        chassis::GetInstance().set_voltage(
            v3 * K_AXIS_3 + v1 * K_AXIS_1,
            v3 * K_AXIS_3 - v1 * K_AXIS_1
        );
        wait(5,msec);
    }
}