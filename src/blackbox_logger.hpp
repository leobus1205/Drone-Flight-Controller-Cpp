#ifndef FLIGHTCONTROLLER_SRC_BLACKBOXLOGGER_HPP_
#define FLIGHTCONTROLLER_SRC_BLACKBOXLOGGER_HPP_

#include "convert_output2duty.hpp"
#include "descrete_pid_controller.hpp"
#include "kalman_filter.hpp"
#include "motor_control.hpp"
#include "mpu9250.hpp"

#include <vector>
#include <iostream>
#include <string>

class BlackboxLogger
{
public:
    std::ofstream writing_file_;

    int i_ = 0;

    const double &t_usec_;
    // const ConvertOutput2Duty &Converter_;
    // const PidController &OuterController_, InnerController_;
    // const KalmanFilter &Filter_;
    const MPU9250 &AttitudeSensor_;
    const std::vector<double> &target_angles_;

    double time_ = 0.0;

    //BlackboxLogger(double &, ConvertOutput2Duty &, PidController &, PidController &, KalmanFilter &, MPU9250 &, std::vector<double> &);
    BlackboxLogger(double &, MPU9250 &, std::vector<double> &);
    ~BlackboxLogger();

    void Logging();
};

#endif // FLIGHTCONTROLLER_SRC_BLACKBOXLOGGER_HPP_