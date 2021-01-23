#include "../src/mpu9250.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    //  lr stop step raw_step
    if (argc <= 3)
    {
        std::cout << "コマンドライン引数が不足しています\n"
                  << std::endl;

        return 0;
    }

    MPU9250 NineAxisSensor;

    unsigned char data[1];
    NineAxisSensor.ReadDatafromSpiDevice(MPU9250_WHO_AM_I, data, 1);

    std::string filename_before = "../data/before_raw_mag_data.csv";

    std::ofstream writing_file_before;
    writing_file_before.open(filename_before, std::ios::trunc);

    std::cout << "\nwriting " << filename_before << "..." << std::endl;

    writing_file_before
        << "RAW_MAG_X,"
        << "RAW_MAG_Y,"
        << "RAW_MAG_Z"
        << std::endl;

    for (int i = 0; i < std::stoi(argv[4]); i++)
    {
        std::cout << "\r"
                  << "Loop:" << std::dec << i
                  << std::string(20, ' ');
        NineAxisSensor.GetGeomagnetisms();
        NineAxisSensor.GetEulerRadAngles(0.0);

        writing_file_before
            << NineAxisSensor.raw_mag_values_[0] << ","
            << NineAxisSensor.raw_mag_values_[1] << ","
            << NineAxisSensor.raw_mag_values_[2]
            << std::endl;
    }
    std::cout << std::endl;

    NineAxisSensor.CalibrateMagnimeter(std::stod(argv[1]), std::stod(argv[2]), std::stoi(argv[3]));

    std::string filename_after = "../data/after_raw_mag_data.csv";

    std::ofstream writing_file_after;
    writing_file_after.open(filename_after, std::ios::trunc);

    std::cout << "\nwriting " << filename_after << "..." << std::endl;

    writing_file_after
        << "RAW_MAG_X,"
        << "RAW_MAG_Y,"
        << "RAW_MAG_Z"
        << std::endl;

    for (int i = 0; i < std::stoi(argv[4]); i++)
    {
        std::cout << "\r"
                  << "Loop:" << std::dec << i
                  << std::string(20, ' ');
        NineAxisSensor.GetGeomagnetisms();
        NineAxisSensor.GetEulerRadAngles(0.0);

        writing_file_after
            << NineAxisSensor.raw_mag_values_[0] << ","
            << NineAxisSensor.raw_mag_values_[1] << ","
            << NineAxisSensor.raw_mag_values_[2]
            << std::endl;
    }
    std::cout << std::endl;

    return 0;
}