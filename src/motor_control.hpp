#ifndef FLIGHTCONTROLLER_SRC_MOTORCONTROL_HPP_
#define FLIGHTCONTROLLER_SRC_MOTORCONTROL_HPP_

#include <pigpio.h>
#include <pigpiod_if2.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

class Motors
{
public:
    int pi_;
    int max_pulse_width_ = 2000, min_pulse_width_ = 1000;
    std::vector<int> motor_gpios_ = std::vector<int>(4, 0);
    int frequency_ = 400, pwm_range_ = 2500;
    int motor_arm_duty_ = 50;
    std::vector<int> motor_control_duties_ = std::vector<int>(4, 0);
    int sleep_time_ = 100000;

    Motors(bool flag);
    ~Motors();
    void SetUpPwm();
    void CalibrateEsc();
    void WakeUpEsc();
    void Arming();
    void ChangePwmDuty();
    void DisArming();
};

#endif // FLIGHTCONTROLLER_SRC_MOTORCONTROL_HPP_