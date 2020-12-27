#include "blackbox_logger.hpp"

BlackboxLogger::BlackboxLogger(
    double &t_usec,
    ConvertOutput2Duty &Converter,
    PidController &OuterController,
    PidController &InnerController,
    KalmanFilter &Filter,
    MPU9250 &AttitudeSensor)
    :t_usec_(t_usec), Converter_(Converter), OuterController_(OuterController), InnerController_(InnerController), Filter_(Filter), AttitudeSensor_(AttitudeSensor)
{
    std::string filename = "../data/blackbox.csv";

    std::cout << "Start to write in  " << filename << "..." << std::endl;
    writing_file_.open(filename, std::ios::trunc);

    // t_usec_ = t_usec;
    // Converter_ = Converter;
    // OuterController_ = OuterController, InnerController_ = InnerController;
    // Filter_ = Filter;
    // AttitudeSensor_ = AttitudeSensor;

    writing_file_
        << "LOOPTIME_USEC"
        << ",";

        //  Converter
        for (int i = 0; i < Converter_.thrusts_.size(); i++)
    {
        writing_file_
            << "THRUST_"
            << i
            << ",";
    }
    for (int i = 0; i < Converter_.duties_.size(); i++)
    {
        writing_file_
            << "DUTY_"
            << i
            << ",";
    }

    //  Attitude Sensor
    for (int i = 0; i < Filter_.matrixes_state_estimate_.size(); i++)
    {
        writing_file_
            << "GYRO_FILTERED_"
            << i
            << ",";
    }
    for (int i = 0; i < AttitudeSensor_.raw_gyro_values_.size(); i++)
    {
        writing_file_
            << "GYRO_"
            << i
            << ",";
    }
    for (int i = 0; i < AttitudeSensor_.raw_accel_values_.size(); i++)
    {
        writing_file_
            << "ACCEL_"
            << i
            << ",";
    }
    for (int i = 0; i < AttitudeSensor_.raw_mag_values_.size(); i++)
    {
        writing_file_
            << "MAG_"
            << i
            << ",";
    }

    //  OuterController
    for (int i = 0; i < OuterController_.e_.size(); i++)
    {
        writing_file_
            << "OUTER_E_"
            << i
            << ",";
    }
    for (int i = 0; i < OuterController_.u_.size(); i++)
    {
        writing_file_
            << "OUTER_U_"
            << i
            << ",";
    }

    //  InnerController
    for (int i = 0; i < InnerController_.e_.size(); i++)
    {
        writing_file_
            << "INNER_E_"
            << i
            << ",";
    }
    for (int i = 0; i < InnerController_.u_.size(); i++)
    {
        writing_file_
            << "INNER_U_"
            << i
            << std::endl;
    }
}

BlackboxLogger::~BlackboxLogger()
{
    writing_file_.close();
}

void BlackboxLogger::Logging()
{
    writing_file_
        << t_usec_
        << ",";

        //  Converter
        for (int i = 0; i < Converter_.thrusts_.size(); i++)
    {
        writing_file_
        << Converter_.thrusts_[i]
        << ",";
    }
    for (int i = 0; i < Converter_.duties_.size(); i++)
    {
        writing_file_
        << Converter_.duties_[i]
        << ",";
    }

    //  Attitude Sensor
    for (int i = 0; i < Filter_.matrixes_state_estimate_.size(); i++)
    {
        writing_file_
        << Filter_.matrixes_state_estimate_.at(i).at(1)
        << ",";
    }
    for (int i = 0; i < AttitudeSensor_.raw_gyro_values_.size(); i++)
    {
        writing_file_
            << AttitudeSensor_.raw_gyro_values_[i]
            << ",";
    }
    for (int i = 0; i < AttitudeSensor_.raw_accel_values_.size(); i++)
    {
        writing_file_
            << AttitudeSensor_.raw_accel_values_[i]
            << ",";
    }
    for (int i = 0; i < AttitudeSensor_.raw_mag_values_.size(); i++)
    {
        writing_file_
            << AttitudeSensor_.raw_mag_values_[i]
            << ",";
    }

    //  OuterController
    for (int i = 0; i < OuterController_.e_.size(); i++)
    {
        writing_file_
            << OuterController_.e_[i]
            << ",";
    }
    for (int i = 0; i < OuterController_.u_.size(); i++)
    {
        writing_file_
            << OuterController_.u_[i]
            << ",";
    }

    //  InnerController
    for (int i = 0; i < InnerController_.e_.size(); i++)
    {
        writing_file_
            << InnerController_.e_[i]
            << ",";
    }
    for (int i = 0; i < InnerController_.u_.size(); i++)
    {
        writing_file_
            << InnerController_.u_[i]
            << std::endl;
    }
}
