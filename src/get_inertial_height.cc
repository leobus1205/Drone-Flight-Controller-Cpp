#include "get_inertial_height.hpp"
#include <iostream>

InertialHeight::InertialHeight()
{
    alpha_ = 0.7;

    std::string file_name_physical = "../conf/physical_parameters.conf";
    std::list<double> physical_parameters;

    if (!this->ReadConfigFileDouble(file_name_physical, physical_parameters))
    {
        std::cout << "Failed to read" << file_name_physical << std::endl;
    }

    auto itr_physical_parameters = physical_parameters.begin();
    GRAVITY = *itr_physical_parameters;
}

void InertialHeight::low_pass_filter(std::vector<double> &low_freq_data_, std::vector<double> &raw_data)
{
    for (int i = 0; i < 3; i++)
    {
        low_freq_data_[i] = alpha_ * low_freq_data_[i] + (1 - alpha_) * raw_data[i];
    }
}

void InertialHeight::high_pass_filter(std::vector<double> &high_freq_data_, std::vector<double> &low_freq_data_, std::vector<double> &raw_data)
{
    for (int i = 0; i < 3; i++)
    {
        high_freq_data_[i] = raw_data[i] - low_freq_data_[i];
    }
}

void InertialHeight::initialize_gravity(double &static_z_axis_accel)
{
    GRAVITY = static_z_axis_accel;
}

void InertialHeight::get_inertial_height(std::vector<double> &raw_accel_values, double dt_usec)
{
    //double g = 0.0;
    double ah = 0.0;
    double dt_sec = dt_usec / 1000.0 / 1000.0;

    low_pass_filter(low_freq_data_, raw_accel_values);
    //high_pass_filter(high_freq_data_, low_freq_data_, raw_accel_values);

    //g = low_freq_data_[2];

    ah = low_freq_data_[2];

    //ah = g * (1.0 - cos(low_freq_data_[0]) * cos(low_freq_data_[1]));
    //ah = GRAVITY - g;

    // std::cout
    //     << ah
    //     << std::endl;

    height_ += 1.0/2.0 * ah * dt_sec * dt_sec;
}