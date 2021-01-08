#include "../src/descrete_pid_controller.hpp"
#include <chrono>
#include <iostream>
#include <fstream>
#include <random>
#include <thread>

int main()
{
    PidController Controller("../conf/outer_descrete_pid_parameters.conf");

    std::cout
        << Controller.K_c_[0] << ","
        << Controller.T_i_transform_[0] << ","
        << Controller.T_d_transform_[0]
        << std::endl;

    std::string filename = "../data/test_run_descrete_pid.csv";
    std::ofstream writing_file;
    writing_file.open(filename, std::ios::trunc);

    writing_file
        << "TARGET_X,"
        << "OUT_X,"
        << "UPRE_X,"
        << "EPRE_X,"
        << "EPRE2_X,"
        << "E_X,"
        << "U_X,"
        << "KC_X,"
        << "TI_X,"
        << "TD_X,"
        << std::endl;

    int step = 100;

    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    constexpr std::size_t bits = std::numeric_limits<double>::digits;
    double result = 0.0;

    std::vector<double> target_zero(3, 0.0);
    std::vector<double> target_step(3, 20.0);
    std::vector<double> target(3, 0.0);

    std::vector<double> value = std::vector<double>(3, 0.0);

    std::chrono::system_clock::time_point start, end;
    double dt_usec = 0.0;

    for (int i = 0; i < step; i++)
    {
        start = std::chrono::system_clock::now();

        result = std::generate_canonical<double, bits>(engine);
        for (int j = 0; j < 3; j++)
            value[j] += 1000 * result;

        if (i < step / 3)
            for (int j = 0; j < 3; j++)
                target[j] = target_zero[j];
        else if (i >= step / 3 && i <= step * 2 / 3)
            for (int j = 0; j < 3; j++)
                target[j] = target_step[j];
        else
            for (int j = 0; j < 3; j++)
                target[j] = target_zero[j];

        Controller.DescretePidController(target, value, dt_usec);

        for (int j = 0; j < 3; j++)
            value[j] += Controller.u_[j];

        writing_file
            << target[0] << ","
            << value[0] << ","
            << Controller.u_pre_[0] << ","
            << Controller.e_pre_[0] << ","
            << Controller.e_pre_2_[0] << ","
            << Controller.e_[0] << ","
            << Controller.u_[0] << ","
            << Controller.K_c_[0] << ","
            << Controller.T_i_transform_[0] << ","
            << Controller.T_d_transform_[0] << ","
            << std::endl;

        end = std::chrono::system_clock::now();
        dt_usec = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    }

    return 0;
}