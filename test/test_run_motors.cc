#include "../src/motor_control.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    Motors Motors(true);

    Motors.Arming();
    std::this_thread::sleep_for(std::chrono::microseconds(Motors.sleep_time_));
    for (int i = 0; i < Motors.motor_gpios_.size(); i++)
        Motors.motor_control_duties_[i] = 50;
    void ChangePwmDuty();

    return 0;
}