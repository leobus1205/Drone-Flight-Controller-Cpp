#include "../src/get_inertial_height.hpp"
#include "../src/mpu9250.hpp"

#include <iostream>

int main()
{
    InertialHeight GetHeight;
    MPU9250 NineAxisSensor;

    unsigned char data[1];
    NineAxisSensor.ReadDatafromSpiDevice(MPU9250_WHO_AM_I, data, 1);

    int convert_data = data[0];
    std::cout << "Who Am I :\t0x" << std::hex << convert_data << std::endl;

    std::chrono::system_clock::time_point start, end;
    double dt_usec = 0.0;

    for (int i = 0; i < 1000; i++)
    {
        start = std::chrono::system_clock::now();

        std::this_thread::sleep_for(std::chrono::microseconds(100000));

        NineAxisSensor.GetVelocitoesandAccelerations();
        NineAxisSensor.GetGeomagnetisms();
        NineAxisSensor.GetEulerRadAngles(dt_usec);

        GetHeight.get_inertial_height(
            NineAxisSensor.raw_accel_values_,
            dt_usec);

        std::cout
            << i << std::dec
            << "\t\nHeight:\t"
            << GetHeight.height_ * 100
            << std::endl;

        end = std::chrono::system_clock::now();
        dt_usec = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    }

    return 0;
}