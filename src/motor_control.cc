#include "motor_control.hpp"

Motors::Motors(bool flag)
{
    std::string file_name = "../conf/esc_parameters.conf";

    std::list<double> parameters;

    if (!this->ReadConfigFileDouble(file_name, parameters))
    {
        std::cout << "Failed to read" << file_name << std::endl;
    }

    auto itr_param = parameters.begin();
    max_pulse_width_ = *itr_param;
    min_pulse_width_ = *(++itr_param);
    frequency_ = *(++itr_param);
    pwm_range_ = *(++itr_param);
    motor_arm_duty_ = *(++itr_param);
    sleep_time_ = *(++itr_param);
    ++itr_param;
    motors_num_ = *(++itr_param);

    for (int i = 0; i < motors_num_; i++)
    {
        motor_gpios_[i] = *(++itr_param);
    }

    pi_ = gpioInitialise();

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
        gpioSetPWMrange(motor_gpios_[i], pwm_range_);
    }
}
void Motors::CalibrateEsc()
{
    std::string s;

    std::cout << "Start to Calibrate ESC" << std::endl;
    std::cout << "Send Max Pulse Width" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioServo(motor_gpios_[i], pwm_range_);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
    std::cout << "Wait to hear Beep and press any key" << std::endl;
    std::cin >> s;

    std::cout << "Send Min Pulse Width" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioServo(motor_gpios_[i], min_pulse_width_);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
    std::cout << "Wait to hear Beep and press any key" << std::endl;
    std::cin >> s;

    std::cout << "Stop to Send Pulse" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioServo(motor_gpios_[i], 0);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));

    std::cout << "Send Max Pulse Width" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioServo(motor_gpios_[i], pwm_range_);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
    std::cout << "Send Min Pulse Width" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioServo(motor_gpios_[i], min_pulse_width_);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
}
void Motors::WakeUpEsc()
{
    std::cout << "Send Max Pulse Width" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioServo(motor_gpios_[i], pwm_range_);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));

    std::cout << "Send Min Pulse Width" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioServo(motor_gpios_[i], min_pulse_width_);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
}
void Motors::Arming()
{
    std::cout << "Arming Motros" << std::endl;
    motor_arm_duty_ = (int)((motor_arm_duty_ * (max_pulse_width_ - min_pulse_width_)) / (max_pulse_width_ - min_pulse_width_));
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioServo(motor_gpios_[i], motor_arm_duty_ * max_pulse_width_);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
}
void Motors::ChangePwmDuty()
{
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioServo(motor_gpios_[i], motor_control_duties_[i] * max_pulse_width_);
}
void Motors::DisArming()
{
    std::cout << "Disarming Motros" << std::endl;
    for (int i = 0; i < motor_gpios_.size(); i++)
        gpioServo(motor_gpios_[i], 0);
    std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_));
}