#include "../src/convert_output2duty.hpp"

#include <vector>
#include <iostream>

int main()
{
    ConvertOutput2Duty Converter;
    std::vector<double> test_vector = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> test_outputs = std::vector<double>(3, 1.0);

    std::cout
        << "Show Parameters.\n"
        << Converter.translate_force2angular_ << "\n"
        << Converter.max_pulse_width_ << "\n"
        << Converter.min_pulse_width_ << "\n"
        << Converter.frequency_ << "\n"
        << Converter.pwm_range_ << "\n"
        << Converter.max_thrust_ << "\n"
        << std::endl;

    for(int i =0; i <Converter.translate_matrix_size_; i++){
        for(int j =0; j <Converter.translate_matrix_size_; j++){
            std::cout << Converter.translate_matrix_.at(i).at(j) << std::endl;
        }
    }

    std::cout
        << "Vector culculation test. Answer = 30\n"
        << Converter.vector_culculator(test_vector, test_vector)
        << std::endl;

    Converter.outputs2thrusts_converter(test_outputs);
    std::cout
        << "Outputs to thrusts convertion test. Answer = {}\n"
        << Converter.thrusts_[0] << " "
        << Converter.thrusts_[1] << " "
        << Converter.thrusts_[2] << " "
        << Converter.thrusts_[3]
        << std::endl;

    Converter.thrusts2duties_converter();
    std::cout
        << "Thrusts to duties convertion test. Answer = {}\n"
        << Converter.duties_[0] << " "
        << Converter.duties_[1] << " "
        << Converter.duties_[2] << " "
        << Converter.duties_[3]
        << std::endl;

    return 0;
}