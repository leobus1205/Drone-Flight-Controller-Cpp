#include "descrete_pid_controller.hpp"

#include <iostream>

PidController::PidController(std::string filename)
{
    std::string file_name_descrete_pid = filename;

    std::list<double> descrete_pid_parameters;

    if (!this->ReadConfigFileDouble(file_name_descrete_pid, descrete_pid_parameters))
    {
        std::cout << "Failed to read" << file_name_descrete_pid << std::endl;
    }

    auto itr_descrete_pid_parameters = descrete_pid_parameters.begin();
    for (int i = 0; i < 3; i++)
    {
        K_c_[i] = *itr_descrete_pid_parameters;
        T_i_transform_[i] = K_c_[i] / *(++itr_descrete_pid_parameters);
        T_d_transform_[i] = K_c_[i] * *(++itr_descrete_pid_parameters);
    }
}

void PidController::DescretePidController(std::vector<double> target, std::vector<double> value, double T_s_usec)
{
    std::vector<double> u_delta = std::vector<double>(3, 0.0);

    double T_s_sec = T_s_usec / 1000.0 / 1000.0;
    double T_s_msec = T_s_usec / 1000.0;

    double P = 0.0;
    double I = 0.0;
    double D = 0.0;

    //std::cout << T_s_sec << std::endl;

    for (int i = 0; i < 3; i++)
    {
        P = 0.0;
        I = 0.0;
        D = 0.0;

        e_[i] = target[i] - value[i];

        if (K_c_[i] > 0.0)
            P = K_c_[i] * (e_[i] - e_pre_[i]);
        if (T_i_transform_[i] > 0.0)
            I = T_i_transform_[i] * T_s_sec * e_[i];
        if (T_d_transform_[i] > 0.0)
            D = (e_[i] - 2.0 * e_pre_[i] + e_pre_2_[i]) * T_d_transform_[i] / T_s_sec;

        if (!std::isfinite(D))
            D = 0.0;

        u_delta[i] = P + I + D;

        u_[i] = u_pre_[i] + u_delta[i];

        u_pre_[i] = u_[i];
        e_pre_2_[i] = e_pre_[i];
        e_pre_[i] = e_[i];
    }
}