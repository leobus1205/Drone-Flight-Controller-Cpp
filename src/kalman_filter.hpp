#ifndef FLIGHTCONTROLLER_SRC_KALMANFILTER_HPP_
#define FLIGHTCONTROLLER_SRC_KALMANFILTER_HPP_

#include "config_loader.hpp"

#include <vector>

class KalmanFilter : public LoadConfigFileClass
{
private:
    /* data */
public:
    int sqare_matrix_size_;
    int axis_dimention_;
    std::vector<double> q_thetas_;
    std::vector<double> q_biases_;
    std::vector<double> rs_;

    std::vector<std::vector<double>> matrixes_state_estimate_;
    std::vector<std::vector<std::vector<double>>> matrixes_error_covariance_;
    KalmanFilter();

    void Filtering(std::vector<double> &, std::vector<double>, double);
};

#endif // FLIGHTCONTROLLER_SRC_KALMANFILTER_HPP_