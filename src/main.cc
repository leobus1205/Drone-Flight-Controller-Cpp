#include "convert_output2duty.hpp"
#include "descrete_pid_controller.hpp"
#include "kalman_filter.hpp"
#include "motor_control.hpp"
#include "mpu9250.hpp"

#include <iostream>
#include <string>

int control_key_input(bool &flag_loopbreak, Motors &Motors, std::vector<double> &target_anfles)
{
    char command_input;
    double value_input;

    while (1)
    {
        std::cin << command_input << value_input << std::endl;
        if (std::cin.fail())
            continue;

        switch (command_input)
        {
        case '/r':
            std::cout << "Emergency Motors Disarming." << std::endl;
            Motors.DisArming();
            flag_loopbreak = true;
            break;

        case 'q':
            std::cout << "Quite." << std::endl;
            flag_loopbreak = true;
            break;

        case 'x':
            target_anfles[0] = value_input;
            break;

        case 'y':
            target_anfles[1] = value_input;
            break;

        case 'z':
            target_anfles[2] = value_input;
            break;
        }
    }

    return 0;
}

void logger(ConvertOutput2Duty &Converter, PidController &OuterController, PidController &InnerController, MPU9250 &AtitudeSensor, Motors &Motors)
{
    //////////////////////
}

//  argv = {motor_status, target_x, target_y, target_z}
int main(char *argv[])
{
    ConvertOutput2Duty Converter;
    PidController OuterController, InnerController;
    KalmanFilter Fillter;
    MPU9250 AtitudeSensor;

    std::vector<double> target_angles = std::vector<double>(3, 0.0);
    for (int i = 0; i < 3; i++)
        target_angles[i] = std::stod(argv[i + 1]);

    std::cout << "Calibrate Magnimeter." << std::endl;
    AtitudeSensor.CalibrateMagnimeter(0.001, 1000, 1000);

    bool flag_loopbreak = false;
    control_key_input(flag_loopbreak, Motors, target_anfles)

    Motors Motors(std::stoi(argv[1]));
    Motors.CalibrateEsc();
    Motors.Arming();

    std::chrono::system_clock::time_point start, end;
    double dt_usec = 0.0;

    // Control proccess
    while (1)
    {
        start = std::chrono::system_clock::now();

        AtitudeSensor.GetVelocitoesandAccelerations();
        AtitudeSensor.GetEulerRadAngles();
        Fillter.Filtering(AtitudeSensor.angles_rad_offsets_, AtitudeSensor.raw_gyro_values_, dt_usec);

        // set target

        OuterController.DescretePidController(target_angles, AtitudeSensor.angles_rad_offsets_, dt_usec);

        for (int i = 0; i < 3; i++)
            AtitudeSensor.raw_gyro_values_[i] = AtitudeSensor.raw_gyro_values_[i] - Fillter.matrixes_state_estimate_.at(i).at(1);
        //InnerController.DescretePidController(OuterController.u_, , dt_usec);

        Converter.outputs2thrusts_converter(InnerController.u_);
        Converter.thrusts2duties_converter();

        auto itr_motors = Motors.motor_control_duties_.begin();
        for (auto itr_converter = Converter.duties_.begin(); itr_converter != Converter.duties_.end(); ++itr_converter && ++itr_motors)
            *itr_motors = *itr_converter;
        Motors.ChangePwmDuty();

        end = std::chrono::system_clock::now();
        dt_usec = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    }

    Motors.DisArming();

    return 0;
}