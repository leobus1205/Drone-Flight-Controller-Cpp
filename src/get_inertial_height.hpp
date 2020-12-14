#ifndef FLIGHTCONTROLLER_SRC_GETINERTIALHEIGHT_HPP_
#define FLIGHTCONTROLLER_SRC_GETINERTIALHEIGHT_HPP_

#include "config_loader.hpp"

#include <vector>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES
#include <cmath>

class InertialHeight : public LoadConfigFileClass
{
public:
    double height_ = 0.0;
    double alpha_ = 0.0;

    std::vector<double> low_freq_data_ = std::vector<double>(3, 0.0);
    std::vector<double> high_freq_data_ = std::vector<double>(3, 0.0);

    double GRAVITY;

    InertialHeight();

    void low_pass_filter(std::vector<double> &, std::vector<double> &);

    void high_pass_filter(std::vector<double> &, std::vector<double> &, std::vector<double> &);

    void initialize_gravity(double &);

    void get_inertial_height(std::vector<double> &, double);
};

#endif // FLIGHTCONTROLLER_SRC_GETINERTIALHEIGHT_HPP_