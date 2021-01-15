#include "blackbox_logger.hpp"

BlackboxLogger::BlackboxLogger(double &t_usec, MPU9250 &AttitudeSensor, std::vector<double> &target_angles)
    // double &t_usec,
    // ConvertOutput2Duty &Converter,
    // PidController &OuterController,
    // PidController &InnerController,
    // KalmanFilter &Filter,
    // MPU9250 &AttitudeSensor,
    // std::vector<double> &target_angles)
    : t_usec_(t_usec), AttitudeSensor_(AttitudeSensor), target_angles_(target_angles)
//:t_usec_(t_usec), Converter_(Converter), OuterController_(OuterController), InnerController_(InnerController), Filter_(Filter), AttitudeSensor_(AttitudeSensor)
{
    std::string filename = "../data/blackbox.csv";

    std::cout << "Start to write in  " << filename << "..." << std::endl;
    writing_file_.open(filename, std::ios::trunc);

    writing_file_
        << "TIME"
        << ",";

    //  Converter
    //     for (int i = 0; i < Converter_.thrusts_.size(); i++)
    // {
    //     writing_file_
    //         << "THRUST_"
    //         << i
    //         << ",";
    // }
    // for (int i = 0; i < Converter_.pulses_.size(); i++)
    // {
    //     writing_file_
    //         << "PULSES_"
    //         << i
    //         << ",";
    // }

    // //  Attitude Sensor
    // for (int i = 0; i < Filter_.matrixes_state_estimate_.size(); i++)
    // {
    //     writing_file_
    //         << "GYRO_FILTERED_"
    //         << i
    //         << ",";
    // }
    // for (int i = 0; i < AttitudeSensor_.raw_gyro_values_.size(); i++)
    // {
    //     writing_file_
    //         << "GYRO_"
    //         << i
    //         << ",";
    // }
    // for (int i = 0; i < AttitudeSensor_.raw_accel_values_.size(); i++)
    // {
    //     writing_file_
    //         << "ACCEL_"
    //         << i
    //         << ",";
    // }
    // for (int i = 0; i < AttitudeSensor_.raw_mag_values_.size(); i++)
    // {
    //     writing_file_
    //         << "MAG_"
    //         << i
    //         << ",";
    // }
    for (int i = 0; i < AttitudeSensor_.raw_rad_angles_.size(); i++)
    {
        writing_file_
            << "ANGLE_RAD_"
            << i
            << ",";
    }
    for (int i = 0; i < target_angles.size() - 1; i++)
    {
        writing_file_
            << "TARGET_ANGLE_"
            << i
            << ",";
    }
    writing_file_
        << "TARGET_ANGLE_"
        << 2
        << std::endl;

    // //  OuterController
    // for (int i = 0; i < OuterController_.e_.size(); i++)
    // {
    //     writing_file_
    //         << "OUTER_E_"
    //         << i
    //         << ",";
    // }
    // for (int i = 0; i < OuterController_.u_.size(); i++)
    // {
    //     writing_file_
    //         << "OUTER_U_"
    //         << i
    //         << ",";
    // }

    // //  InnerController
    // for (int i = 0; i < InnerController_.e_.size(); i++)
    // {
    //     writing_file_
    //         << "INNER_E_"
    //         << i
    //         << ",";
    // }
    // for (int i = 0; i < InnerController_.u_.size() - 1; i++)
    // {
    //     writing_file_
    //         << "INNER_U_"
    //         << i
    //         << ",";
    // }
    // writing_file_
    //         << "INNER_U_"
    //         << 2
    //         << std::endl;
}

BlackboxLogger::~BlackboxLogger()
{
    writing_file_.close();
}

void BlackboxLogger::Logging()
{
    time_ += t_usec_ / 1000.0 /1000.0;
    writing_file_
        << time_
        << ",";

    //     //  Converter
    //     for (int i = 0; i < Converter_.thrusts_.size(); i++)
    // {
    //     writing_file_
    //     << Converter_.thrusts_[i]
    //     << ",";
    // }
    // for (int i = 0; i < Converter_.pulses_.size(); i++)
    // {
    //     writing_file_
    //     << Converter_.pulses_[i]
    //     << ",";
    // }

    // //  Attitude Sensor
    // for (int i = 0; i < Filter_.matrixes_state_estimate_.size(); i++)
    // {
    //     writing_file_
    //     << Filter_.matrixes_state_estimate_.at(i).at(1)
    //     << ",";
    // }
    // for (int i = 0; i < AttitudeSensor_.raw_gyro_values_.size(); i++)
    // {
    //     writing_file_
    //         << AttitudeSensor_.raw_gyro_values_[i]
    //         << ",";
    // }
    // for (int i = 0; i < AttitudeSensor_.raw_accel_values_.size(); i++)
    // {
    //     writing_file_
    //         << AttitudeSensor_.raw_accel_values_[i]
    //         << ",";
    // }
    // for (int i = 0; i < AttitudeSensor_.raw_mag_values_.size(); i++)
    // {
    //     writing_file_
    //         << AttitudeSensor_.raw_mag_values_[i]
    //         << ",";
    // }
    for (int i = 0; i < AttitudeSensor_.raw_rad_angles_.size(); i++)
    {
        writing_file_
            << AttitudeSensor_.raw_rad_angles_[i]
            << ",";
    }
    for (int i = 0; i < target_angles_.size() - 1; i++)
    {
        writing_file_
            << target_angles_[i]
            << ",";
    }
    writing_file_
        << target_angles_[2]
        << std::endl;

    // //  OuterController
    // for (int i = 0; i < OuterController_.e_.size(); i++)
    // {
    //     writing_file_
    //         << OuterController_.e_[i]
    //         << ",";
    // }
    // for (int i = 0; i < OuterController_.u_.size(); i++)
    // {
    //     writing_file_
    //         << OuterController_.u_[i]
    //         << ",";
    // }

    // //  InnerController
    // for (int i = 0; i < InnerController_.e_.size(); i++)
    // {
    //     writing_file_
    //         << InnerController_.e_[i]
    //         << ",";
    // }
    // for (int i = 0; i < InnerController_.u_.size()-1; i++)
    // {
    //     writing_file_
    //         << InnerController_.u_[i]
    //         << ",";
    // }
    // writing_file_
    //         << InnerController_.u_[2]
    //         << std::endl;
}
