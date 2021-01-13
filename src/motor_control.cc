#include "motor_control.hpp"

Motors::Motors(bool flag)
{
    std::string file_name = "../conf/esc_parameters.conf";

    std::list<double> parameters;

    if (!this->ReadConfigFileDouble(file_name, parameters))
    {
        std::cout << "Failed to read" << file_name << "\n" << std::endl;
    }

    auto itr_param = parameters.begin();
    max_pulse_ = *itr_param;
    min_pulse_ = *(++itr_param);
    frequency_ = *(++itr_param);
    pwm_range_ = *(++itr_param);
    motor_arm_pulse_ = *(++itr_param);
    sleep_time_ = *(++itr_param);
    ++itr_param;
    motors_num_ = *(++itr_param);

    for (int i = 0; i < motors_num_; i++)
    {
        motor_gpios_[i] = *(++itr_param);
    }

    if (gpioInitialise() < 0)
    {
        std::cout << "Failed to initialize pigpio.\n" << std::endl;
        exit(1);
    }

    this->SetUpPwm();

    if (flag != false)
        this->CalibrateEsc();
    else
        this->WakeUpEsc();
}

Motors::~Motors()
{
    gpioTerminate();
}

void Motors::SetUpPwm()
{
    for (int i = 0; i < motor_gpios_.size(); i++)
    {
        gpioSetPWMfrequency(motor_gpios_[i], frequency_);
        gpioSetPWMrange(motor_gpios_[i], (unsigned int)pwm_range_);
    }
}

void Motors::CalibrateEsc()
{
    std::string s;

    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioPWM(motor_gpios_[i], 0);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));

    std::cout << "\nStart to Calibrate ESC" << std::endl;
    std::cout << "\nCalibration ESC is usb power only. If raspberrypi use power from bat, cnnect power cable to it." << std::endl;

    std::cout << "\nSend Max Pulse Width" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioPWM(motor_gpios_[i], (unsigned int)(max_pulse_));
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
    std::cout << "Please connect battery." << std::endl;
    std::cout << "Wait to hear Beep and press any key" << std::endl;
    std::cin >> s;

    std::cout << "\nSend Min Pulse Width" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioPWM(motor_gpios_[i], (unsigned int)(min_pulse_));
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
    std::cout << "Wait to hear Beep and press any key" << std::endl;
    std::cin >> s;
}

void Motors::WakeUpEsc()
{
    std::cout << "\nSend Min Pulse Width\n" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioPWM(motor_gpios_[i], (unsigned int)(min_pulse_));
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
}

void Motors::Arming()
{
    std::cout << "\nArming Motros" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioPWM(motor_gpios_[i], (unsigned int)(motor_arm_pulse_));
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
}

void Motors::ChangePwmDuty(const std::vector<double> &motor_control_pulses)
{
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioPWM(motor_gpios_[i], (unsigned int)(motor_control_pulses[i]));

    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
}

void Motors::DisArming()
{
    std::cout << "Disarming Motros\n" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioPWM(motor_gpios_[i], 0);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
}