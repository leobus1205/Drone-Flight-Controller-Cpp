#include "../src/mpu9250.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>

int main()
{
    MPU9250 NineAxisSensor;

    unsigned char data[1];
    NineAxisSensor.ReadDatafromSpiDevice(MPU9250_WHO_AM_I, data, 1);

    //  Unsigned char型に16進数int型のデータがそのまま格納されているので、16進数表示
    //  データそのものは型によらない。
    int convert_data = data[0];
    std::cout << "Who Am I :\t0x" << std::hex << convert_data << std::endl;

    for (int i = 0; i < 1000; i++)
    {
        NineAxisSensor.GetVelocitoesandAccelerations();
        NineAxisSensor.GetGeomagnetisms();
        NineAxisSensor.GetEulerRadAngles();

        std::cout
            << i
            << "\t\nGyro:\t"
            << NineAxisSensor.raw_gyro_values_[0]
            << " "
            << NineAxisSensor.raw_gyro_values_[1]
            << " "
            << NineAxisSensor.raw_gyro_values_[2]
            << "\t\nAccel:\t"
            << NineAxisSensor.raw_accel_values_[0]
            << " "
            << NineAxisSensor.raw_accel_values_[1]
            << " "
            << NineAxisSensor.raw_accel_values_[2]
            << "\t\nMag:\t"
            << NineAxisSensor.raw_mag_values_[0]
            << " "
            << NineAxisSensor.raw_mag_values_[1]
            << " "
            << NineAxisSensor.raw_mag_values_[2]
            << "\t\nRadAngle:\t"
            << NineAxisSensor.raw_rad_angles_[0]
            << " "
            << NineAxisSensor.raw_rad_angles_[1]
            << " "
            << NineAxisSensor.raw_rad_angles_[2]
            << std::endl;

        NineAxisSensor.GetVelocitoesandAccelerations();
        NineAxisSensor.GetEulerDegAngles();
        std::cout
            << "DegAngle:\t"
            << NineAxisSensor.raw_deg_angles_[0]
            << " "
            << NineAxisSensor.raw_deg_angles_[1]
            << " "
            << NineAxisSensor.raw_deg_angles_[2]
            << "\n"
            << std::endl;
    }

    return 0;
}