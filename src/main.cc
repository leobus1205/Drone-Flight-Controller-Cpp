#include "blackbox_logger.hpp"
#include "convert_output2duty.hpp"
#include "descrete_pid_controller.hpp"
#include "kalman_filter.hpp"
#include "motor_control.hpp"
#include "mpu9250.hpp"

#include <iostream>
#include <string>
#include <thread>

int control_key_input(bool &flag_loopbreak, Motors &Motors, std::vector<double> &target_angles)
{
    char command_input;
    double value_input;

    while (1)
    {
        std::cin >> command_input;
        if (std::cin.fail())
            continue;

        switch (command_input)
        {
        case 'q':
            std::cout << "Quite." << std::endl;
            flag_loopbreak = true;
            return -1;

        case 'x':
            std::cin >> value_input;
            target_angles[0] = value_input;
            break;

        case 'y':
            std::cin >> value_input;
            target_angles[1] = value_input;
            break;

        case 'z':
            std::cin >> value_input;
            target_angles[2] = value_input;
            break;

        default: // Press Empty Enterにしたい
            std::cout << "Emergency Motors Disarming." << std::endl;
            Motors.DisArming();
            flag_loopbreak = true;
            return -1;
        }
    }

    return 0;
}

//  argv = {esc_calibration flag, flag_calibratemag, target_y, target_z}
int main(char *argv[])
{
    ConvertOutput2Duty Converter;
    PidController OuterController, InnerController;
    KalmanFilter Fillter;
    MPU9250 AttitudeSensor;

    std::vector<double> target_angles = std::vector<double>(3, 0.0);
    for (int i = 0; i < 3; i++)
        target_angles[i] = 0.0;
    //target_angles[i] = std::stod(argv[i + 1]);

    bool flag_calibratemag = 1;
    if (flag_calibratemag != true)
    {
        std::cout << "Calibrate Magnimeter." << std::endl;
        AttitudeSensor.CalibrateMagnimeter(0.001, 1000, 1000);
    }

    //Motors Motors(std::stoi(argv[1]));
    Motors Motors(true);

    bool flag_loopbreak = false;
    std::thread thread_key_input(control_key_input, std::ref(flag_loopbreak), std::ref(Motors), std::ref(target_angles));

    Motors.Arming();

    std::chrono::system_clock::time_point start, end;
    double dt_usec = 0.0;

    BlackboxLogger Logger(dt_usec, Converter, OuterController, InnerController, Fillter, AttitudeSensor);

    // Control proccess
    while (1)
    {
        // start = std::chrono::system_clock::now();

        if(flag_loopbreak == true)
            break;

        // AttitudeSensor.GetVelocitoesandAccelerations();
        // AttitudeSensor.GetEulerRadAngles();
        // Fillter.Filtering(AttitudeSensor.angles_rad_offsets_, AttitudeSensor.raw_gyro_values_, dt_usec);

        // // set target

        // OuterController.DescretePidController(target_angles, AttitudeSensor.angles_rad_offsets_, dt_usec);

        // for (int i = 0; i < 3; i++)
        //     AttitudeSensor.raw_gyro_values_[i] = AttitudeSensor.raw_gyro_values_[i] - Fillter.matrixes_state_estimate_.at(i).at(1);
        // InnerController.DescretePidController(OuterController.u_, AttitudeSensor.raw_gyro_values_, dt_usec);

        // Converter.outputs2thrusts_converter(InnerController.u_);
        // Converter.thrusts2duties_converter();

        // auto itr_motors = Motors.motor_control_duties_.begin();
        // for (auto itr_converter = Converter.duties_.begin(); itr_converter != Converter.duties_.end(); ++itr_converter)
        // {
        //     *itr_motors = *itr_converter;
        //     ++itr_motors;
        // }
        // Motors.ChangePwmDuty();

        // Logger.Logging();

        // end = std::chrono::system_clock::now();
        // dt_usec = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    }

    Motors.DisArming();

    thread_key_input.join();

    return 0;
}