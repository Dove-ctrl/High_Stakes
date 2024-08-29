#ifndef PID_H_
#define PID_H_

#include <math.h>
#include "v5.h"
#include "v5_vcs.h"

class PID{
private:
    float error_curt , error_past;
    float P,I,D;
    bool first_time;
    bool arrived;
    float kp,ki,kd;
    float target , error_tol;
    float error_range_low , error_range_high;
    float output;
    int sign_start , sign_now;
public:
    enum reset_type{
        _first_time , _I , _arrived , _all
    };
    PID();
    void reset(reset_type);
    void set_coefficient(float _kp,float _ki,float _kd);
    void set_target(float _target);
    void set_error_range(float _error_range_low,float _error_range_high);
    void set_error_tol(float _error_tol);
    float get_output();
    bool target_arrived();
    void update(float input);

    static void PIDwait();
};

#endif