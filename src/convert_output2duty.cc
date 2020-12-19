#include "convert_output2duty.hpp"

ConvertOutput2Duty::ConvertOutput2Duty()
{
    std::string file_name_vihecle = "../conf/vihecle_parameters.conf";

    std::list<double> parameters_vihecle;

    if (!this->ReadConfigFileDouble(file_name_vihecle, parameters_vihecle))
    {
        std::cout << "Failed to read" << file_name_vihecle << std::endl;
    }

    translate_matrix_.resize(translate_matrix_size_, std::vector<double>(translate_matrix_size_));
    auto itr_param_vihecle = parameters_vihecle.begin();

    for (int i = 0; i < translate_matrix_size_; i++)
    {
        for (auto itr = translate_matrix_.at(i).begin(); itr != translate_matrix_.at(i).end(); ++itr)
        {
            *itr = *itr_param_vihecle;
            ++itr_param_vihecle;
        }
    }

    translate_force2angular_ = *(++itr_param_vihecle);

    std::string file_name_esc = "../conf/esc_parameters.conf";

    std::list<double> parameters_esc;

    if (!this->ReadConfigFileDouble(file_name_esc, parameters_esc))
    {
        std::cout << "Failed to read" << file_name_esc << std::endl;
    }

    auto itr_param_esc = parameters_esc.begin();
    max_pulse_width_ = *itr_param_esc;
    min_pulse_width_ = *(++itr_param_esc);
    frequency_ = *(++itr_param_esc);
    pwm_range_ = *(++itr_param_esc);

    ++itr_param_esc;
    ++itr_param_esc;
    max_thrust_ = *(++itr_param_esc) * max_pulse_width_ / pwm_range_;

    // for(int i = 0; i < translate_matrix_1_.size();i++){
    //     translate_matrix_1_[i] = *itr_param;
    //     ++itr_param;
    // }
    // for(int i = 0; i < translate_matrix_1_.size();i++){
    //     translate_matrix_2_[i] = *itr_param;
    //     ++itr_param;
    // }
    // for(int i = 0; i < translate_matrix_1_.size();i++){
    //     translate_matrix_3_[i] = *itr_param;
    //     ++itr_param;
    // }
    // for(int i = 0; i < translate_matrix_1_.size();i++){
    //     translate_matrix_4_[i] = *itr_param;
    //     ++itr_param;
    // }
}

double ConvertOutput2Duty::vector_culculator(std::vector<double> &horizontal_vector, std::vector<double> &vertical_vector)
{
    double result = 0.0;

    for (int i = 0; i < translate_matrix_size_; i++)
    {
        result += horizontal_vector[i] * vertical_vector[i];
    }

    return result;
}

void ConvertOutput2Duty::outputs2thrusts_converter(std::vector<double> &outputs)
{
    for (int i = 0; i < translate_matrix_size_; i++)
    {
        thrusts_[i] = vector_culculator(translate_matrix_.at(i), outputs);
    }
}

// void ConvertOutput2Duty::thrusts2angularvelocities_converter(std::vector<double> &thrusts, std::vector<double> &angular_velocities)
// {
//     double angular_square = 0.0;

//     for(int i = 0; i < translate_matrix_size_; i++)
//     {
//         angular_square = translate_force2angular_ * vector_culculator(translate_matrix_.at(i), thrusts);
//         angular_velocities[i] = sqrt(angular_square);
//     }
// }

// void ConvertOutput2Duty::angularvelocities2duties_converter(std::vector<double> &angular_velocities, std::vector<double> &duties)
// {
//     for (int i = 0; i < translate_matrix_size_; i++) {
//         duties = angular_velocities;
//     }

//     for (int i = 0; i < translate_matrix_size_; i++) {
//         if (duties[i] > 100) {
//             duties[i] = 100;
//         }
//     }
// }

void ConvertOutput2Duty::thrusts2duties_converter()
{
    for (int i = 0; i < translate_matrix_size_; i++)
    {
        duties[i] = thrusts_[i] / max_thrust_;
        if (duties_[i] > 100)
        {
            duties_[i] = 100;
        }
    }
}