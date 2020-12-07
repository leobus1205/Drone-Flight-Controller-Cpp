#include "descrete_pid_controller.hpp"

PidController::PidController(std::vector<double> &K_c, std::vector<double> &T_i_transform, std::vector<double> &T_d_transform)
{
    for (int i = 0; i < 3; i++)
    {
        K_c_[i] = K_c[i];
        T_s_msec_[i] = 0.0;
        T_i_transform_[i] = T_i_transform[i];
        T_d_transform_[i] = T_d_transform[i];
    }
}

void PidController::DescretePidController(std::vector<double> &target)
{
    std::vector<double> u_delta = std::vector<double>(3, 0.0);

    double T_s_sec = 0.0;

    for (int i = 0; i < 3; i++)
    {
        e_[i] = target[i] - u_pre_[i];
        T_s_sec = T_s_msec_[i] / 1000.0;

        u_delta[i] = K_c_[i] * (e_[i] - e_pre_[i]) + T_i_transform_[i] * T_s_sec * e_[i] + T_d_transform_[i] / T_s_sec * (e[i] - 2.0 * e_pre_[i] + e_pre_2_[i]);

        u_[i] = u_pre_[i] + u_delta[i];

        u_pre_[i] = u_[i];
        e_pre_2_[i] = e_pre_[i];
        e_pre_[i] = e_[i];
    }
}