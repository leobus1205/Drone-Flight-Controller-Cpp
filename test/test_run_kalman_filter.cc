#include "../src/mpu9250.hpp"
#include "../src/kalman_filter.hpp"
#include <iostream>
#include <chrono>

int main()
{
    MPU9250 NineAxisSensor;
    KalmanFilter Filter;

    unsigned char data[1];
    NineAxisSensor.ReadDatafromSpiDevice(MPU9250_WHO_AM_I, data, 1);

    //  Unsigned char型に16進数int型のデータがそのまま格納されているので、16進数表示
    //  データそのものは型によらない。
    int convert_data = data[0];
    std::cout << "Who Am I :\t0x" << std::hex << convert_data << "\ns" << std::endl;

    double raw_gyro_values[3] = {0.0}, raw_accel_values[3] = {0.0}, raw_mag_values[3] = {0.0};
    double dt_msec = 0.0;
    std::chrono::system_clock::time_point start, end;

    std::cout << "DegOffsets:\t\t"
              << NineAxisSensor.angles_rad_offsets_[0] * NineAxisSensor.RAD2DEG
              << " "
              << NineAxisSensor.angles_rad_offsets_[1] * NineAxisSensor.RAD2DEG
              << " "
              << NineAxisSensor.angles_rad_offsets_[2] * NineAxisSensor.RAD2DEG
              << "\n"
              << std::endl;
    for (int i = 0; i < 100; i++)
    {
        start = std::chrono::system_clock::now();
        NineAxisSensor.GetVelocitoesandAccelerations();
        NineAxisSensor.GetGeomagnetisms();
        NineAxisSensor.GetEulerRadAngles(0.0);
        std::cout
            << "RawDegAngle:\t\t"
            << NineAxisSensor.raw_rad_angles_[0] * NineAxisSensor.RAD2DEG
            << " "
            << NineAxisSensor.raw_rad_angles_[1] * NineAxisSensor.RAD2DEG
            << " "
            << NineAxisSensor.raw_rad_angles_[2] * NineAxisSensor.RAD2DEG
            << std::endl;
        Filter.Filtering(NineAxisSensor.raw_rad_angles_, NineAxisSensor.raw_gyro_values_, dt_msec);
        std::cout
            << "FilteredDegAngle:\t"
            << NineAxisSensor.raw_rad_angles_[0] * NineAxisSensor.RAD2DEG
            << " "
            << NineAxisSensor.raw_rad_angles_[1] * NineAxisSensor.RAD2DEG
            << " "
            << NineAxisSensor.raw_rad_angles_[2] * NineAxisSensor.RAD2DEG
            << "\n"
            << std::endl;
        end = std::chrono::system_clock::now();
        dt_msec = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    }

    return 0;
}