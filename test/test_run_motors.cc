#include "../src/motor_control.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    Motors Motors(true);

    std::vector<int> motor_control_pwms = std::vector<int>(4, 1500);

    Motors.Arming();
    std::this_thread::sleep_for(std::chrono::microseconds(Motors.sleep_time_));

    Motors.ChangePwmDuty(motor_control_pwms);

    return 0;
}