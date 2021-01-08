#include "mpu9250.hpp"

MPU9250::MPU9250()
{
    //  Read Config Flie and Initialize Parameters
    std::string file_name_physical = "../conf/physical_parameters.conf";
    std::string file_name_mpu9250 = "../conf/mpu9250.conf";

    std::list<double> physical_parameters;
    std::list<double> mpu9250_parameters;

    if (!this->ReadConfigFileDouble(file_name_physical, physical_parameters))
    {
        std::cout << "Failed to read" << file_name_physical << std::endl;
    }

    if (!this->ReadConfigFileDouble(file_name_mpu9250, mpu9250_parameters))
    {
        std::cout << "Failed to read" << file_name_mpu9250 << std::endl;
    }

    auto itr_physical_parameters = physical_parameters.begin();
    GRAVITY = *itr_physical_parameters;

    auto itr_mpu9250 = mpu9250_parameters.begin();
    MAG_X_INITIAL_OFFSET = *itr_mpu9250;
    mag_offsets_[0] = MAG_X_INITIAL_OFFSET;
    MAG_Y_INITIAL_OFFSET = *(++itr_mpu9250);
    mag_offsets_[1] = MAG_Y_INITIAL_OFFSET;
    MAG_Z_INITIAL_OFFSET = *(++itr_mpu9250);
    mag_offsets_[2] = MAG_Z_INITIAL_OFFSET;

    MAG_X_INITIAL_SCALE = *(++itr_mpu9250);
    MAG_Y_INITIAL_SCALE = *(++itr_mpu9250);
    MAG_Z_INITIAL_SCALE = *(++itr_mpu9250);

    MAG_X_INITIAL_SENSE = *(++itr_mpu9250);
    MAG_Y_INITIAL_SENSE = *(++itr_mpu9250);
    MAG_Z_INITIAL_SENSE = *(++itr_mpu9250);

    GYRO_X_INITIAL_OFFSET = *(++itr_mpu9250);
    GYRO_Y_INITIAL_OFFSET = *(++itr_mpu9250);
    GYRO_Z_INITIAL_OFFSET = *(++itr_mpu9250);

    for (auto itr_alignmet = sensor_alignments_.begin(); itr_alignmet != sensor_alignments_.end(); ++itr_alignmet)
    {
        *itr_alignmet = *(++itr_mpu9250);
    }

    std::cout << "Success to read mpu9250 config files\n"
              << std::endl;

    fd_ = this->OpenSpiDevice(SPI_DEVICE_PATH_0, SPI_MODE_0, SPI_BITS_PER_WORD, SPI_SPEED);
    this->InitializeGyroAccelSensor(GYRO_RANGE_1000DPS, ACCEL_RANGE_8G);

    this->SetDlpfBandwidth(GYRO_BANDWIDTH_184HZ, ACCEL_BANDWIDTH_184HZ, 0);

    std::chrono::system_clock::time_point start, end;
    double dt_usec = 0.0;

    for(int i=0; i < 1000; i++){
        start = std::chrono::system_clock::now();

        this->GetVelocitoesandAccelerations();
        this->GetGeomagnetisms();
        this->GetEulerRadAngles(dt_usec);
        this->GetEulerDegAngles(dt_usec);

        end = std::chrono::system_clock::now();
        dt_usec = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    }

    angles_rad_offsets_ = raw_rad_angles_;
    angles_deg_offsets_ = raw_deg_angles_;
    std::cout << "Angle Deg Offsets: "
              << angles_deg_offsets_[0]
              << angles_deg_offsets_[1]
              << angles_deg_offsets_[2]
              << std::endl;
}

MPU9250::~MPU9250()
{
    this->CloseSpiDevice();
}

void MPU9250::SetGyroScale(unsigned char gyro_range)
{
    // Data
    unsigned char data[1];

    // Calculate Gyro Scale
    switch (gyro_range)
    {
    /* +-250 [deg/s] */
    case GYRO_RANGE_250DPS:
        data[0] = 0x00;
        gyro_scale_ = this->DEG2RAD * 250.0 / 32768.0;
        break;
    /* +-500 [deg/s] */
    case GYRO_RANGE_500DPS:
        data[0] = 0x08;
        gyro_scale_ = this->DEG2RAD * 500.0 / 32768.0;
        break;
    /* +-1000 [deg/s] */
    case GYRO_RANGE_1000DPS:
        data[0] = 0x10;
        gyro_scale_ = this->DEG2RAD * 1000.0 / 32768.0;
        break;
    /* +-2000 [deg/s] */
    case GYRO_RANGE_2000DPS:
        data[0] = 0x18;
        gyro_scale_ = this->DEG2RAD * 2000.0 / 32768.0;
        break;
    }

    // Set Gyro Musurement Range
    this->WriteData2SpiDevice(MPU9250_GYRO_CONFIG, data[0]);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    this->ReadDatafromSpiDevice(MPU9250_GYRO_CONFIG, data, sizeof(data));
    int convert_data = data[0];
    std::cout << "MPU9250_GYRO_CONFIG : 0x" << std::hex << convert_data << "\n"
              << std::endl;
}

void MPU9250::SetAccelScale(unsigned char accel_range)
{
    //  Data
    unsigned char data[1];

    //  Calculate Acceleration Scale
    switch (accel_range)
    {
    /* +-2 [G] */
    case ACCEL_RANGE_2G:
        data[0] = 0x00;
        accel_scale_ = GRAVITY * 2.0 / 32768.0;
        break;
    /* +-4 [G] */
    case ACCEL_RANGE_4G:
        data[0] = 0x08;
        accel_scale_ = GRAVITY * 4.0 / 32768.0;
        break;
    /* +-8 [G] */
    case ACCEL_RANGE_8G:
        data[0] = 0x10;
        accel_scale_ = GRAVITY * 8.0 / 32768.0;
        break;
    /* +-16 [G] */
    case ACCEL_RANGE_16G:
        data[0] = 0x18;
        accel_scale_ = GRAVITY * 16.0 / 32768.0;
        break;
    }

    // Set Acceleration Musurement Range
    this->WriteData2SpiDevice(MPU9250_ACCEL_CONFIG, data[0]);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    this->ReadDatafromSpiDevice(MPU9250_ACCEL_CONFIG, data, sizeof(data));
    int convert_data = data[0];
    std::cout << "MPU9250_ACCEL_CONFIG : 0x" << std::hex << convert_data << "\n"
              << std::endl;
}

int MPU9250::InitializeAk8963()
{
    //  Data
    unsigned char data[3];

    //  Check Who I Am Register
    ReadDataFromAk8963(AK8963_WIA, data, 1);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    int convert_data = data[0];
    std::cout << "AK8963_Who_Im_I : 0x" << std::hex << convert_data << "\n"
              << std::endl;
    if (data[0] != 0x48)
    {
        std::cout << "Can't Identify AK8963"
                  << "\n"
                  << std::endl;
        std::exit(1);
    }

    //  Enter Power Down Mode
    WriteData2Ak8963(AK8963_CNTL1, 0x00);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    ReadDataFromAk8963(AK8963_CNTL1, data, 1);
    convert_data = data[0];
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    std::cout << "AK8963_CNTL1 : 0x" << std::hex << convert_data << "\n"
              << std::endl;

    //  Switch To Huse ROM Access Mode
    WriteData2Ak8963(AK8963_CNTL1, 0x0F);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    ReadDataFromAk8963(AK8963_CNTL1, data, 1);
    convert_data = data[0];
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    std::cout << "AK8963_CNTL1 : 0x" << std::hex << convert_data << "\n"
              << std::endl;

    // Set Geomagnification Scale
    ReadDataFromAk8963(AK8963_ASAX, data, 3);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    for (int i = 0; i < 3; i++)
    {
        mag_scales_[i] = (((double)data[i] - 128.0) / 256.0 + 1.0) * 4912.0 / 32760.0;
    }

    // Switch To Power Down Mode
    WriteData2Ak8963(AK8963_CNTL1, 0x00);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    ReadDataFromAk8963(AK8963_CNTL1, data, 1);
    convert_data = data[0];
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    std::cout << "AK8963_CNTL1 : 0x" << std::hex << convert_data << "\n"
              << std::endl;

    // Switch To Continuous Measurement Mode 2
    WriteData2Ak8963(AK8963_CNTL1, 0x16);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    ReadDataFromAk8963(AK8963_CNTL1, data, 1);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    convert_data = data[0];
    std::cout << "AK8963_CNTL1 : 0x" << std::hex << convert_data << "\n"
              << std::endl;

    return 0;
}

int MPU9250::InitializeGyroAccelSensor(unsigned char gyro_range, unsigned char accel_range)
{
    // Data
    unsigned char data[1];

    // Check Who Am I Register
    this->ReadDatafromSpiDevice(MPU9250_WHO_AM_I, data, sizeof(data));
    if (data[0] != MPU9250_I_AM)
    {
        std::cout << "Can't Identify MPU9250"
                  << "\n"
                  << std::endl;
        std::exit(1);
    }
    int convert_data = data[0];
    std::cout << "MPU9250_I_AM : 0x" << std::hex << convert_data << "\n"
              << std::endl;

    // Set Communication Clock
    this->WriteData2SpiDevice(MPU9250_PWR_MGMT_1, 0x01);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    std::cout << "MPU9250_PWR_MGMT_1 : 0x" << std::hex << convert_data << "\n"
              << std::endl;

    // Enable Gyro And Acceleration Sensor
    this->WriteData2SpiDevice(MPU9250_PWR_MGMT_2, 0x00);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    this->ReadDatafromSpiDevice(MPU9250_PWR_MGMT_2, data, sizeof(data));
    std::cout << "MPU9250_PWR_MGMT_2 : 0x" << std::hex << convert_data << "\n"
              << std::endl;

    // Set Gyro Scale
    SetGyroScale(gyro_range);

    // Set Acceleration Scale
    SetAccelScale(accel_range);

    // Enable I2C Master (For Communicating With AK8963)
    this->WriteData2SpiDevice(MPU9250_USER_CTRL, 0x20);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    this->ReadDatafromSpiDevice(MPU9250_USER_CTRL, data, sizeof(data));
    std::cout << "MPU9250_USER_CTRL : 0x" << std::hex << convert_data << "\n"
              << std::endl;

    // Set I2C Speed 400kH
    this->WriteData2SpiDevice(MPU9250_I2C_MST_CTRL, 0x0D);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    this->ReadDatafromSpiDevice(MPU9250_I2C_MST_CTRL, data, sizeof(data));
    std::cout << "MPU9250_I2C_MST_CTRL : 0x" << std::hex << convert_data << "\n"
              << std::endl;

    InitializeAk8963();

    return 0;
}

int MPU9250::CloseGyroAccelSensor()
{
    this->CloseSpiDevice();

    return 0;
}

int MPU9250::SetDlpfBandwidth(
    unsigned char gyro_bandwidth,
    unsigned char accel_bandwidth,
    unsigned char srd)
{
    //  Data
    unsigned char data[1];

    //  Set Accerelatoin DLPF Bandwidth
    switch (gyro_bandwidth)
    {
    //  8800Hz
    case GYRO_BANDWIDTH_8800HZ:
        //  Disable Acceleration DLPF
        this->WriteData2SpiDevice(MPU9250_CONFIG, 0x00);
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        this->ReadDatafromSpiDevice(MPU9250_GYRO_CONFIG, data, sizeof(data));
        data[0] |= 0x03;
        break;
        //  3600Hz
    case GYRO_BANDWIDTH_3600HZ:
        this->WriteData2SpiDevice(MPU9250_CONFIG, 0x07);
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        this->ReadDatafromSpiDevice(MPU9250_GYRO_CONFIG, data, sizeof(data));
        data[0] &= 0xFC;
        break;
        //  250Hz
    case GYRO_BANDWIDTH_250HZ:
        this->WriteData2SpiDevice(MPU9250_CONFIG, 0x00);
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        this->ReadDatafromSpiDevice(MPU9250_GYRO_CONFIG, data, sizeof(data));
        data[0] &= 0xFC;
        break;
        //  184Hz
    case GYRO_BANDWIDTH_184HZ:
        this->WriteData2SpiDevice(MPU9250_CONFIG, 0x01);
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        this->ReadDatafromSpiDevice(MPU9250_GYRO_CONFIG, data, sizeof(data));
        data[0] &= 0xFC;
        break;
        //  92Hz
    case GYRO_BANDWIDTH_92HZ:
        this->WriteData2SpiDevice(MPU9250_CONFIG, 0x02);
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        this->ReadDatafromSpiDevice(MPU9250_GYRO_CONFIG, data, sizeof(data));
        data[0] &= 0xFC;
        break;
        //  41Hz
    case GYRO_BANDWIDTH_41HZ:
        this->WriteData2SpiDevice(MPU9250_CONFIG, 0x03);
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        this->ReadDatafromSpiDevice(MPU9250_GYRO_CONFIG, data, sizeof(data));
        data[0] &= 0xFC;
        break;
        //  20Hz
    case GYRO_BANDWIDTH_20HZ:
        this->WriteData2SpiDevice(MPU9250_CONFIG, 0x04);
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        this->ReadDatafromSpiDevice(MPU9250_GYRO_CONFIG, data, sizeof(data));
        data[0] &= 0xFC;
        break;
        //  10Hz
    case GYRO_BANDWIDTH_10HZ:
        this->WriteData2SpiDevice(MPU9250_CONFIG, 0x05);
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        this->ReadDatafromSpiDevice(MPU9250_GYRO_CONFIG, data, sizeof(data));
        data[0] &= 0xFC;
        break;
        //  5Hz
    case GYRO_BANDWIDTH_5HZ:
        this->WriteData2SpiDevice(MPU9250_CONFIG, 0x06);
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        this->ReadDatafromSpiDevice(MPU9250_GYRO_CONFIG, data, sizeof(data));
        data[0] &= 0xFC;
        break;
    }

    this->WriteData2SpiDevice(MPU9250_GYRO_CONFIG, data[0]);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    this->ReadDatafromSpiDevice(MPU9250_GYRO_CONFIG, data, sizeof(data));
    int convert_data = data[0];
    std::cout << "MPU9250_GYRO_CONFIG : 0x" << std::hex << convert_data << "\n"
              << std::endl;

    //  Set Accerelation DLPF Bandwidth
    switch (accel_bandwidth)
    {
    //  1130Hz
    case ACCEL_BANDWIDTH_1130HZ:
        //  Disable Accerelation DLPF
        data[0] = 0x08;
        break;
        //  460Hz
    case ACCEL_BANDWIDTH_460HZ:
        data[0] = 0x00;
        break;
        //  184Hz
    case ACCEL_BANDWIDTH_184HZ:
        data[0] = 0x01;
        break;
        //  92Hz
    case ACCEL_BANDWIDTH_92HZ:
        data[0] = 0x02;
        break;
        //  41Hz
    case ACCEL_BANDWIDTH_41HZ:
        data[0] = 0x03;
        break;
        //  20Hz
    case ACCEL_BANDWIDTH_20HZ:
        data[0] = 0x04;
        break;
        //  10Hz
    case ACCEL_BANDWIDTH_10HZ:
        data[0] = 0x05;
        break;
        //  5Hz
    case ACCEL_BANDWIDTH_5HZ:
        data[0] = 0x06;
        break;
    }

    this->WriteData2SpiDevice(MPU9250_ACCEL_CONFIG2, data[0]);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    this->ReadDatafromSpiDevice(MPU9250_ACCEL_CONFIG2, data, sizeof(data));
    std::cout << "MPU9250_ACCEL_CONFIG2 : 0x" << std::hex << convert_data << "\n"
              << std::endl;

    //  Set Sampling Rate From 1/1 To 1/256
    //  While 5 ~ 184 Hz
    this->WriteData2SpiDevice(MPU9250_SMPLRT_DIV, srd);
    std::this_thread::sleep_for(std::chrono::microseconds(100000));

    return 0;
}

void MPU9250::ReadDataFromAk8963(
    unsigned char address,
    unsigned char *data,
    int bytes)
{
    this->WriteData2SpiDevice(MPU9250_I2C_SLV0_ADDR, AK8963_I2C_ADDRESS | 0x80);
    this->WriteData2SpiDevice(MPU9250_I2C_SLV0_REG, address);
    this->WriteData2SpiDevice(MPU9250_I2C_SLV0_CTRL, bytes | 0x80);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    this->ReadDatafromSpiDevice(MPU9250_EXT_SENS_DATA_00, data, bytes);
}

void MPU9250::WriteData2Ak8963(
    unsigned char address,
    unsigned char data)
{
    this->WriteData2SpiDevice(MPU9250_I2C_SLV0_ADDR, AK8963_I2C_ADDRESS);
    this->WriteData2SpiDevice(MPU9250_I2C_SLV0_REG, address);
    this->WriteData2SpiDevice(MPU9250_I2C_SLV0_DO, data);
    this->WriteData2SpiDevice(MPU9250_I2C_SLV0_CTRL, 0x01 | 0x80);
}

void MPU9250::GetVelocitoesandAccelerations()
{
    unsigned char data[14];

    this->ReadDatafromSpiDevice(MPU9250_ACCEL_XOUT_H, data, sizeof(data));

    for (int i = 0; i < 3; i++)
    {
        raw_gyro_values_[i] = gyro_scale_ * this->Ushort2Signed((data[2 * i + 8] << 8) | data[2 * i + 9]);
        raw_gyro_values_[i] *= sensor_alignments_[i];

        raw_accel_values_[i] = accel_scale_ * this->Ushort2Signed((data[2 * i + 0] << 8) | data[2 * i + 1]);
        raw_accel_values_[i] *= sensor_alignments_[i];
    }
}

void MPU9250::GetGeomagnetisms()
{
    unsigned char data[7];
    double temp;

    ReadDataFromAk8963(AK8963_HXL, data, sizeof(data));

    for (int i = 0; i < 3; i++)
    {
        raw_mag_values_h_[i] = mag_scales_[i] * this->Ushort2Signed((data[2 * i + 1] << 8) | data[2 * i]);
    }

    temp = raw_mag_values_h_[0];
    raw_mag_values_[0] = raw_mag_values_h_[1] - mag_offsets_[0];
    raw_mag_values_[1] = temp - mag_offsets_[1];
    raw_mag_values_[2] = -raw_mag_values_h_[2] - mag_offsets_[2];

    for (int i = 0; i < 3; i++)
    {
        raw_mag_values_[i] *= sensor_alignments_[i];
    }
}

void MPU9250::GetEulerRadAngles(double T_s_usec)
{
    double T_s_sec = T_s_usec / 1000.0 / 1000.0;

    double sin_phi, cos_phi;
    double sin_theta, cos_theta;

    double temp1 = 0.0, temp2 = 0.0;

    raw_rad_angles_[0] = atan2(raw_accel_values_[1], raw_accel_values_[2]);

    sin_phi = sin(raw_rad_angles_[0]);
    cos_phi = cos(raw_rad_angles_[0]);
    raw_rad_angles_[1] = atan(-raw_accel_values_[0] / (raw_accel_values_[1] * sin_phi + raw_accel_values_[2] * cos_phi));

    sin_theta = sin(raw_rad_angles_[1]);
    cos_theta = cos(raw_rad_angles_[1]);
    temp1 = -raw_mag_values_[1] * cos_phi + raw_mag_values_[2] * sin_phi;
    temp2 = raw_mag_values_[0] * cos_theta + raw_mag_values_[1] * sin_phi * sin_theta + raw_mag_values_[2] * cos_phi * sin_theta;
    raw_rad_angles_[2] = atan2(temp1, temp2);

    for (int i = 0; i < 3; i++)
    {
        raw_rad_angles_[i] = 0.95 * (raw_rad_angles_[i] + raw_gyro_values_[i] * T_s_sec) + 0.05 * raw_rad_angles_[i];
        raw_rad_angles_[i] = raw_rad_angles_[i] - angles_rad_offsets_[i];
    }
}

void MPU9250::GetEulerDegAngles(double T_s_usec)
{
    double T_s_sec = T_s_usec / 1000.0 / 1000.0;

    double sin_phi, cos_phi;
    double sin_theta, cos_theta;

    double temp1 = 0.0, temp2 = 0.0;

    raw_rad_angles_[0] = atan2(raw_accel_values_[1], raw_accel_values_[2]);

    sin_phi = sin(raw_rad_angles_[0]);
    cos_phi = cos(raw_rad_angles_[0]);
    raw_rad_angles_[1] = atan(-raw_accel_values_[0] / (raw_accel_values_[1] * sin_phi + raw_accel_values_[2] * cos_phi));

    sin_theta = sin(raw_rad_angles_[1]);
    cos_theta = cos(raw_rad_angles_[1]);
    temp1 = -raw_mag_values_[1] * cos_phi + raw_mag_values_[2] * sin_phi;
    temp2 = raw_mag_values_[0] * cos_theta + raw_mag_values_[1] * sin_phi * sin_theta + raw_mag_values_[2] * cos_phi * sin_theta;
    raw_rad_angles_[2] = atan2(temp1, temp2);

    for (int i = 0; i < 3; i++)
    {
        raw_rad_angles_[i] = 0.95 * (raw_rad_angles_[i] + raw_gyro_values_[i] * T_s_sec) + 0.05 * raw_rad_angles_[i];
        raw_deg_angles_[i] = raw_rad_angles_[i] * RAD2DEG;
        raw_deg_angles_[i] = (raw_rad_angles_[i] - angles_rad_offsets_[i]) * RAD2DEG;
    }
}

void MPU9250::CalibrateMagnimeter(double lr, double stop, int step)
{
    std::vector<double> b = std::vector<double>(4, 0.0); // 推定対象[x_0, y_0, z_0, r]
    b[3] = 1.0;
    double dx = 0.0, dy = 0.0, dz = 0.0, f = 0.0;

    std::string filename = "../data/mag_offsets_data.csv";

    std::ofstream writing_file;
    writing_file.open(filename, std::ios::trunc);

    std::cout << "writing " << filename << "..." << std::endl;

    writing_file
        << "RAW_MAG_X,"
        << "RAW_MAG_Y,"
        << "RAW_MAG_Z,"
        << "CALIB_MAG_X,"
        << "CALIB_MAG_Y,"
        << "CALIB_MAG_Z,"
        << "CALIB_MAG_R,"
        << "EPSILON,"
        << "DX,"
        << "DY,"
        << "DZ,"
        << "LEARNING_RATE"
        << std::endl;

    for (int i = 0; i < step; i++)
    {
        std::cout << "\r"
                  << "Loop:" << std::dec << i
                  << " Epsilon= " << f
                  << std::string(20, ' ');

        this->GetGeomagnetisms();
        dx = raw_mag_values_[0] - b[0];
        dy = raw_mag_values_[1] - b[1];
        dz = raw_mag_values_[2] - b[2];
        f = dx * dx + dy * dy + dz * dz - b[3] * b[3];
        b[0] = b[0] + 4.0 * lr * f * dx;
        b[1] = b[1] + 4.0 * lr * f * dy;
        b[2] = b[2] + 4.0 * lr * f * dz;
        b[3] = b[3] + 4.0 * lr * f * b[3];

        mag_offsets_[0] = b[0];
        mag_offsets_[1] = b[1];
        mag_offsets_[2] = b[2];

        writing_file
            << raw_mag_values_[0] << ","
            << raw_mag_values_[1] << ","
            << raw_mag_values_[2] << ","
            << b[0] << ","
            << b[1] << ","
            << b[2] << ","
            << b[3] << ","
            << f << ","
            << dx << ","
            << dy << ","
            << dz << ","
            << lr
            << std::endl;

        if (std::abs(f) <= stop)
            break;
    }
}