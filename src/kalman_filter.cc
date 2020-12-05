#include "kalman_filter.hpp"

#include <stdio.h>

KalmanFilter::KalmanFilter()
{
    //  Read Config Flie
    std::string file_name = "../conf/kalman_filter.conf";

    std::list<double> parameters;

    if (!this->ReadConfigFileDouble(file_name, parameters))
    {
        std::cout << "Failed to read" << file_name << std::endl;
    }

    std::cout << "Start to read kalman filter config files\n"
              << std::endl;

    auto itr = parameters.begin();
    sqare_matrix_size_ = static_cast<int>(*itr);
    axis_dimention_ = *(++itr);
    q_thetas_.resize(axis_dimention_);
    for (int i = 0; i < q_thetas_.size(); i++)
    {
        q_thetas_[i] = *(++itr);
    }
    q_biases_.resize(axis_dimention_);
    for (int i = 0; i < q_biases_.size(); i++)
    {
        q_biases_[i] = *(++itr);
    }
    rs_.resize(axis_dimention_);
    for (int i = 0; i < rs_.size(); i++)
    {
        rs_[i] = *(++itr);
    }

    matrixes_state_estimate_.resize(axis_dimention_, std::vector<double>(sqare_matrix_size_));
    matrixes_error_covariance_.resize(axis_dimention_);
    for (int i = 0; i < matrixes_error_covariance_.size(); i++)
    {
        matrixes_error_covariance_[i].resize(sqare_matrix_size_, std::vector<double>(sqare_matrix_size_));
    }

    std::cout << "Success to read kalman filter config files\n"
              << std::endl;
}

void KalmanFilter::Filtering(std::vector<double> &angles, std::vector<double> gyros, double dt_msec)
{
    double dt_sec = dt_msec / 1000.0;
    std::vector<double> matrix_pre_state_estimate = std::vector<double>(sqare_matrix_size_);
    std::vector<double> matrix_gain = std::vector<double>(sqare_matrix_size_);

    std::vector<std::vector<double>> matrix_pre_error_covariance = std::vector<std::vector<double>>(sqare_matrix_size_, std::vector<double>(sqare_matrix_size_));

    for (int i = 0; i < axis_dimention_; i++)
    {
        // Create reference last matrix.
        const std::vector<double> &matrix_last_state_estimate = matrixes_state_estimate_[i];
        const std::vector<std::vector<double>> &matrix_last_pre_error_covariance = matrixes_error_covariance_[i];

        // Calculate pre-error covariance matrix
        //Pre_state_estimate = {{ angle, bias }...}
        matrix_pre_state_estimate[0] = matrix_last_state_estimate[0] - dt_sec * matrix_last_state_estimate[1] + dt_sec * gyros[i];
        matrix_pre_state_estimate[1] = matrix_last_state_estimate[1];

        //Calculate pre-error covariance matrix
        matrix_pre_error_covariance[0][0] = (matrix_last_pre_error_covariance[0][0] - dt_sec * matrix_last_pre_error_covariance[1][0]) - dt_sec * (matrix_last_pre_error_covariance[0][1] - dt_sec * matrix_last_pre_error_covariance[1][1]) + dt_sec * q_thetas_[i];
        matrix_pre_error_covariance[0][1] = matrix_last_pre_error_covariance[0][1] - dt_sec * matrix_last_pre_error_covariance[1][1];
        matrix_pre_error_covariance[1][0] = matrix_last_pre_error_covariance[1][0] - dt_sec * matrix_last_pre_error_covariance[1][1];
        matrix_pre_error_covariance[1][1] = matrix_last_pre_error_covariance[1][1] + dt_sec * q_biases_[i];

        //Calculate kalman gain
        for (int j = 0; j < matrix_gain.size(); j++)
        {
            matrix_gain[j] = (1 / (matrix_pre_error_covariance[0][0] + rs_[i])) * matrix_pre_error_covariance[j][0];
        }

        //Calculate estimate angle
        for (int j = 0; j < matrixes_state_estimate_.at(0).size(); j++)
        {
            matrixes_state_estimate_[i][j] = matrix_pre_state_estimate[j] + matrix_gain[j] * (angles[i] - matrix_pre_state_estimate[0]);
            angles[i] = matrixes_state_estimate_[i][0];
        }

        //Calculate estimate error_covariance
        for (int j = 0; j < matrixes_error_covariance_.at(0).at(0).size(); j++)
        {
            matrixes_error_covariance_[i][0][j] = (1.0 - matrix_gain[0]) * matrix_pre_error_covariance[0][j];
            matrixes_error_covariance_[i][1][j] = matrix_pre_error_covariance[1][j] - matrix_gain[1] * matrix_pre_error_covariance[0][j];
        }
    }
}