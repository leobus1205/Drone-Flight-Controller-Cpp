#ifndef FLIGHTCONTROLLER_SRC_MOTORCONTROL_HPP_
#define FLIGHTCONTROLLER_SRC_MOTORCONTROL_HPP_

#include "config_loader.hpp"

#include <pigpio.h>
#include <pigpiod_if2.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

class Motors : private LoadConfigFileClass
{
public:
    int pi_;
    int motors_num_ = 4;
    std::vector<int> motor_gpios_ = std::vector<int>(motors_num_, 0);
    // ONESHOT125 defoults
    int max_pulse_ = 2000, min_pulse_ = 1000;
    int motor_arm_pulse_ = 1200;
    int frequency_ = 2000;
    int pwm_range_ = 4000;
    int sleep_time_ = 2000000;

    Motors(bool flag);
    ~Motors();
    void SetUpPwm();
    void CalibrateEsc();
    void WakeUpEsc();
    void Arming();
    void ChangePwmDuty(const std::vector<double> &);
    void DisArming();
};

#endif // FLIGHTCONTROLLER_SRC_MOTORCONTROL_HPP_