#include "convert_output2duty.hpp"
#include "descrete_pid_controller.hpp"
#include "kalman_filter.hpp"
#include "motor_control.hpp"
#include "mpu9250.hpp"

#include <iostream>
#include <string>
#include <stdio.h>

int emergency_shutdown(Motors &Motors)
{
    while (1)
        if ('\r' == _getch())
            Motors.DisArming();

    //////////////
    exit(1);

    return -1;
}

int main(char *argv[])
{
    ConvertOutput2Duty Converter;
    PidController OuterController, InnerController;
    KalmanFilter Fillter;
    MPU9250 AtitudeSensor;
    
    std::vector<double> target_angles = std::vector<double>(3, 0.0);

    std::cout << "Calibrate Magnimeter." << std::endl;
    AtitudeSensor.CalibrateMagnimeter(0.001, 1000, 1000);

    // emergency thread open

    Motors Motors(std::stoi(argv[1]));
    Motors.CalibrateEsc();
    Motors.Arming();

    std::chrono::system_clock::time_point start, end;
    double dt_usec = 0.0;

    // Control proccess
    while ()
    {
        start = std::chrono::system_clock::now();

        AtitudeSensor.GetVelocitoesandAccelerations();
        AtitudeSensor.GetEulerRadAngles();
        Fillter.Filtering(AtitudeSensor.angles_rad_offsets_, dt_usec);

        // set target

        OuterController.DescretePidController(target_angles, AtitudeSensor.angles_rad_offsets_, dt_usec);

        for (int i = 0; i < 3; i++)
            AtitudeSensor.raw_gyro_values_ = AtitudeSensor.raw_gyro_values_[i] - Fillter.at(i).at(1);
        InnerController.DescretePidController(OuterController.u_, , dt_usec);

        Converter.outputs2thrusts_converter(InnerController.u_);
        Converter.thrusts2duties_converter();

        end = std::chrono::system_clock::now();
        dt_usec = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    }

    Motors.DisArming();

    return 0;
}