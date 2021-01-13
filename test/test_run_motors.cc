#include "../src/motor_control.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    Motors Motors(true);

    std::vector<double> motor_control_pwms = std::vector<double>(4, 1400);

    Motors.Arming();

    std::this_thread::sleep_for(std::chrono::microseconds(2000000));

    Motors.ChangePwmDuty(motor_control_pwms);

    // for (int i = 0; i < Motors.motor_gpios_.size(); i++)
    //     gpioPWM(Motors.motor_gpios_[i], 1100);

    std::this_thread::sleep_for(std::chrono::microseconds(5000000));

    return 0;
}