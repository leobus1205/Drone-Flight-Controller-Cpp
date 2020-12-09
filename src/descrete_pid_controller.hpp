#ifndef FLIGHTCONTROLLER_SRC_DISCRETEPIDCONTROLLER_HPP_
#define FLIGHTCONTROLLER_SRC_DISCRETEPIDCONTROLLER_HPP_

#include <vector>
#include <cmath>

class PidController
{

public:
    double initializer = 0.0;
    std::vector<double> u_pre_ = std::vector<double>(3, initializer);
    std::vector<double> e_pre_ = std::vector<double>(3, initializer);
    std::vector<double> e_pre_2_ = std::vector<double>(3, initializer);

    std::vector<double> K_c_ = std::vector<double>(3, initializer);
    std::vector<double> T_i_transform_ = std::vector<double>(3, initializer);
    std::vector<double> T_d_transform_ = std::vector<double>(3, initializer);

    std::vector<double> e_ = std::vector<double>(3, initializer);
    std::vector<double> u_ = std::vector<double>(3, initializer);

    PidController(std::vector<double> &, std::vector<double> &, std::vector<double> &);

    void DescretePidController(std::vector<double>, std::vector<double>, double);
};

#endif // FLIGHTCONTROLLER_SRC_DISCRETEPIDCONTROLLER_HPP_