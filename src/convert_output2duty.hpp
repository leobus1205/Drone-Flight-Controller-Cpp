#ifndef FLIGHTCONTROLLER_SRC_CONVERTOUTPUT2DUTY_HPP_
#define FLIGHTCONTROLLER_SRC_CONVERTOUTPUT2DUTY_HPP_

#include "config_loader.hpp"

#include <vector>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES
#include <cmath>

class ConvertOutput2Duty : public LoadConfigFileClass
{
public:
    // std::vector<double> translate_matrix_1_ = std::vector<double>(4, 0.0);
    // std::vector<double> translate_matrix_2_ = std::vector<double>(4, 0.0);
    // std::vector<double> translate_matrix_3_ = std::vector<double>(4, 0.0);
    // std::vector<double> translate_matrix_4_ = std::vector<double>(4, 0.0);
    std::vector<std::vector<double>> translate_matrix_;
    int translate_matrix_size_ = 4;
    double translate_force2angular_ = 0.0;

    std::vector<double> thrusts_ = std::vector<double>(translate_matrix_size_, 0.0);
    std::vector<double> duties_ = std::vector<double>(translate_matrix_size_, 0.0);

    int max_pulse_width_ = 0;
    int min_pulse_width_ = 0;
    int frequency_ = 0;
    int pwm_range_ = 0;
    double max_thrust_ = 0.0;
    int motor_num_ = 4;

    ConvertOutput2Duty();

    double vector_culculator(std::vector<double> &, std::vector<double> &);

    void outputs2thrusts_converter(std::vector<double> &);

    // void thrusts2angularvelocities_converter(std::vector<double> &, std::vector<double> &);

    // void angularvelocities2duties_converter(std::vector<double> &, std::vector<double> &);

    void thrusts2duties_converter();
};

#endif // FLIGHTCONTROLLER_SRC_CONVERTOUTPUT2DUTY_HPP_