#ifndef FLIGHTCONTROLLER_SRC_MPU9250_HPP_
#define FLIGHTCONTROLLER_SRC_MPU9250_HPP_

#include "use_sensor.hpp"
#include "config_loader.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

class MPU9250 : public UseSensorClass, public LoadConfigFileClass
{
    /* UseSensorClass use_sensor_;
    LoadConfigFileClass* load_config_file_; */
    //  Range Of Gyro Sensor
    ////    NUMBER deg/s
    typedef enum
    {
        GYRO_RANGE_250DPS,
        GYRO_RANGE_500DPS,
        GYRO_RANGE_1000DPS,
        GYRO_RANGE_2000DPS,
    } MPU9250_GYRO_RANGE;

    //  Range Of Acceleration Sensor
    ////    NUMBER G (Gravity)
    typedef enum
    {
        ACCEL_RANGE_2G,
        ACCEL_RANGE_4G,
        ACCEL_RANGE_8G,
        ACCEL_RANGE_16G,
    } MPU9250_ACCEL_RANGE;

    //  Digital Low Pass Filter (DLPF) Bandwidth Of Gyro Sensor
    typedef enum
    {
        GYRO_BANDWIDTH_8800HZ,
        GYRO_BANDWIDTH_3600HZ,
        GYRO_BANDWIDTH_250HZ,
        GYRO_BANDWIDTH_184HZ,
        GYRO_BANDWIDTH_92HZ,
        GYRO_BANDWIDTH_41HZ,
        GYRO_BANDWIDTH_20HZ,
        GYRO_BANDWIDTH_10HZ,
        GYRO_BANDWIDTH_5HZ
    } MPU9250_GYRO_BANDWIDTH;

    //  DLPF Bandwidth Of Acceleration Sensor
    typedef enum
    {
        ACCEL_BANDWIDTH_1130HZ,
        ACCEL_BANDWIDTH_460HZ,
        ACCEL_BANDWIDTH_184HZ,
        ACCEL_BANDWIDTH_92HZ,
        ACCEL_BANDWIDTH_41HZ,
        ACCEL_BANDWIDTH_20HZ,
        ACCEL_BANDWIDTH_10HZ,
        ACCEL_BANDWIDTH_5HZ
    } MPU9250_ACCEL_BANDWIDTH;

//  I2C Addresses of MPU9250
////    I2C Address Of MPU9250
#define MPU9250_I2C_ADDRESS 0x68

////    I2C Address Of AK8963
#define AK8963_I2C_ADDRESS 0x0c

//  Registers Of MPU9250
////    Self tests Of Gyro Sensor (Read/Write)
#define MPU9250_SELF_TEST_X_GYRO 0x00
#define MPU9250_SELF_TEST_Y_GYRO 0x01
#define MPU9250_SELF_TEST_Z_GYRO 0x02

////    Self tests Of Acceleration Sensor (R/W)
#define MPU9250_SELF_TEST_X_ACCEL 0x0D
#define MPU9250_SELF_TEST_Y_ACCEL 0x0E
#define MPU9250_SELF_TEST_Z_ACCEL 0x0F

////    Offsets Of Gyro Sensor (R/W)
#define MPU9250_XG_OFFSET_H 0x13
#define MPU9250_XG_OFFSET_L 0x14
#define MPU9250_YG_OFFSET_H 0x15
#define MPU9250_YG_OFFSET_L 0x16
#define MPU9250_ZG_OFFSET_H 0x17
#define MPU9250_ZG_OFFSET_L 0x18

////    Sampling Rate Divider (R/W)
#define MPU9250_SMPLRT_DIV 0x19

////    Configuration (R/W)
#define MPU9250_CONFIG 0x1A

////    Configuration Of Gyro Sensor
#define MPU9250_GYRO_CONFIG 0x1B

////    Configuration Of Acceleration Sensor (R/W)
#define MPU9250_ACCEL_CONFIG 0x1C

////    Configuration "2" Of acceleration Sensor (R/W)
#define MPU9250_ACCEL_CONFIG2 0x1D

////    ODR Contorol Of Low Power Acceleration Sensor (R/W)
#define MPU9250_LP_ACCEL_ODR 0x1E

////    Wake-on Motion Threshold
#define MPU9250_WOM_THR 0x1F

////    First In First Out (FIFO) Enable (R/W)
/*
        FIFO : In FIFO, A Sensor Always Put Data Periodical,
                And A Computer Can Take It In Own Timing.
    */
#define MPU9250_FIFO_EN 0x23

////    I2C Master Control (R/W)
#define MPU9250_I2C_MST_CTRL 0x24

////    I2C Slave 0 (R/W)
#define MPU9250_I2C_SLV0_ADDR 0x25
#define MPU9250_I2C_SLV0_REG 0x26
#define MPU9250_I2C_SLV0_CTRL 0x27

////    I2C Slave 1 (R/W)
#define MPU9250_I2C_SLV1_ADDR 0x28
#define MPU9250_I2C_SLV1_REG 0x29
#define MPU9250_I2C_SLV1_CTRL 0x2A

////    I2C Slave 2 (R/W)
#define MPU9250_I2C_SLV2_ADDR 0x2B
#define MPU9250_I2C_SLV2_REG 0x2C
#define MPU9250_I2C_SLV2_CTRL 0x2D

////    I2C Slave 3 (R/W)
#define MPU9250_I2C_SLV3_ADDR 0x2E
#define MPU9250_I2C_SLV3_REG 0x2F
#define MPU9250_I2C_SLV3_CTRL 0x30

////    I2C Slave 4 (R/W)
#define MPU9250_I2C_SLV4_ADDR 0x31
#define MPU9250_I2C_SLV4_REG 0x32
#define MPU9250_I2C_SLV4_DO 0x33
#define MPU9250_I2C_SLV4_CTRL 0x34
#define MPU9250_I2C_SLV4_DI 0x35

////    I2C Master Status (R)
#define MPU9250_I2C_MST_STATUS 0x36

////    Configuration Of Interrupt Pin (R/W)
#define MPU9250_INT_PIN_CFG 0x37

////    Interrupt Enable (R/W)
#define MPU9250_INT_ENABLE 0x38

////    Interrupt Status (R)
#define MPU9250_INT_STATUS 0x3A

////    Acceleration Sensor Salues (R)
#define MPU9250_ACCEL_XOUT_H 0x3B
#define MPU9250_ACCEL_XOUT_L 0x3C
#define MPU9250_ACCEL_YOUT_H 0x3D
#define MPU9250_ACCEL_YOUT_L 0x3E
#define MPU9250_ACCEL_ZOUT_H 0x3F
#define MPU9250_ACCEL_ZOUT_L 0x40

////    Temperature Sensor Values (R)
#define MPU9250_TEMP_OUT_H 0x41
#define MPU9250_TEMP_OUT_L 0x42

////    Gyro Sensor Values (R)
#define MPU9250_GYRO_XOUT_H 0x43
#define MPU9250_GYRO_XOUT_L 0x44
#define MPU9250_GYRO_YOUT_H 0x45
#define MPU9250_GYRO_YOUT_L 0x46
#define MPU9250_GYRO_ZOUT_H 0x47
#define MPU9250_GYRO_ZOUT_L 0x48

////    External Sensor Data (R)
#define MPU9250_EXT_SENS_DATA_00 0x49
#define MPU9250_EXT_SENS_DATA_01 0x4A
#define MPU9250_EXT_SENS_DATA_02 0x4B
#define MPU9250_EXT_SENS_DATA_03 0x4C
#define MPU9250_EXT_SENS_DATA_04 0x4D
#define MPU9250_EXT_SENS_DATA_05 0x4E
#define MPU9250_EXT_SENS_DATA_06 0x4F
#define MPU9250_EXT_SENS_DATA_07 0x50
#define MPU9250_EXT_SENS_DATA_08 0x51
#define MPU9250_EXT_SENS_DATA_09 0x52
#define MPU9250_EXT_SENS_DATA_10 0x53
#define MPU9250_EXT_SENS_DATA_11 0x54
#define MPU9250_EXT_SENS_DATA_12 0x55
#define MPU9250_EXT_SENS_DATA_13 0x56
#define MPU9250_EXT_SENS_DATA_14 0x57
#define MPU9250_EXT_SENS_DATA_15 0x58
#define MPU9250_EXT_SENS_DATA_16 0x59
#define MPU9250_EXT_SENS_DATA_17 0x5A
#define MPU9250_EXT_SENS_DATA_18 0x5B
#define MPU9250_EXT_SENS_DATA_19 0x5C
#define MPU9250_EXT_SENS_DATA_20 0x5D
#define MPU9250_EXT_SENS_DATA_21 0x5E
#define MPU9250_EXT_SENS_DATA_22 0x5F
#define MPU9250_EXT_SENS_DATA_23 0x60

////    Digital Out Of I2C Slaves (R/W)
#define MPU9250_I2C_SLV0_DO 0x63
#define MPU9250_I2C_SLV1_DO 0x64
#define MPU9250_I2C_SLV2_DO 0x65
#define MPU9250_I2C_SLV3_DO 0x66

////    I2C Master Delay Control (R/W)
#define MPU9250_I2C_MST_DELAY_CTRL 0x67

////    Signal Path Reset (R/W)
#define MPU9250_SIGNAL_PATH_RESET 0x68

////    Interrupt Control Of Acceleration Sensor (R/W)
#define MPU9250_MOT_DETECT_CTRL 0x69

////    User Control (R/W)
#define MPU9250_USER_CTRL 0x6A

////    Power Management 1 (R/W)
#define MPU9250_PWR_MGMT_1 0x6B

////    Power Management 2 (R/W)
#define MPU9250_PWR_MGMT_2 0x6C

////    FIFO Count (R/W)
#define MPU9250_FIFO_COUNTH 0x72
#define MPU9250_FIFO_COUNTL 0x73

////    FIFO Read/Write (R/W)
#define MPU9250_FIFO_R_W 0x74

////    Who Am I (R)
#define MPU9250_WHO_AM_I 0x75
#define MPU9250_I_AM 0x71

////    Offsets Of Acceleration Sensor (R/W)
#define MPU9250_XA_OFFSET_H 0x77
#define MPU9250_XA_OFFSET_L 0x78
#define MPU9250_YA_OFFSET_H 0x7A
#define MPU9250_YA_OFFSET_L 0x7B
#define MPU9250_ZA_OFFSET_H 0x7D
#define MPU9250_ZA_OFFSET_L 0x7E

//	Registers Of AK8963
////    Who I Am (R)
#define AK8963_WIA 0x00

////    Information (R)
#define AK8963_INFO 0x01

////    Status 1 (R)
#define AK8963_ST1 0x02

////    Sensor Values (R)
#define AK8963_HXL 0x03
#define AK8963_HXH 0x04
#define AK8963_HYL 0x05
#define AK8963_HYH 0x06
#define AK8963_HZL 0x07
#define AK8963_HZH 0x08

////    Status 2 (R)
#define AK8963_ST2 0x09

////    Control 1 (R/W)
#define AK8963_CNTL1 0x0A

////    Control 2 (R/W)
#define AK8963_CNTL2 0x0B

////    Self Test (R/W)
#define AK8963_ASTC 0x0C

////    Test 1, Do Not Use (R/W)
#define AK8963_TS1 0x0D

////    Test 2, Do Not Use (R/W)
#define AK8963_TS2 0x0E

////    I2C Disable (R/W)
#define AK8963_I2CDIS 0x0F

////    Sensitivity Values (R)
#define AK8963_ASAX 0x10
#define AK8963_ASAY 0x11
#define AK8963_ASAZ 0x12

////    Reserved, Do Not Use (R)
#define AK8963_RSV 0x13

    ///     Physical parameters
    double GRAVITY;

    ///     MPU9250 parameters
    double MAG_X_INITIAL_OFFSET;
    double MAG_Y_INITIAL_OFFSET;
    double MAG_Z_INITIAL_OFFSET;
    double MAG_X_INITIAL_SCALE;
    double MAG_Y_INITIAL_SCALE;
    double MAG_Z_INITIAL_SCALE;
    double MAG_X_INITIAL_SENSE;
    double MAG_Y_INITIAL_SENSE;
    double MAG_Z_INITIAL_SENSE;
    double GYRO_X_INITIAL_OFFSET;
    double GYRO_Y_INITIAL_OFFSET;
    double GYRO_Z_INITIAL_OFFSET;

public:
    int fd_;

    double gyro_scale_ = 0.0, accel_scale_ = 0.0;
    std::vector<double> mag_scales_ = std::vector<double>(3, 0.0), mag_offsets_ = std::vector<double>(3, 0.0);
    std::vector<double> raw_gyro_values_ = std::vector<double>(3, 0.0), raw_accel_values_ = std::vector<double>(3, 0.0), raw_mag_values_ = std::vector<double>(3, 0.0);
    std::vector<double> raw_mag_values_h_ = std::vector<double>(3, 0.0);
    std::vector<double> raw_rad_angles_ = std::vector<double>(3, 0.0);
    std::vector<double> raw_deg_angles_ = std::vector<double>(3, 0.0);
    std::vector<double> angles_rad_offsets_ = std::vector<double>(3, 0.0);
    std::vector<double> angles_deg_offsets_ = std::vector<double>(3, 0.0);

    std::vector<double> sensor_alignments_ = std::vector<double>(3, 0.0);

    //  Constructor
    //  Read Config File and Open MPU9250 on SPI
    MPU9250();

    //  Destructor
    ~MPU9250();

    //  Set Gyro Scale
    void SetGyroScale(unsigned char gyro_range);

    //  Set Acceleration Scale
    void SetAccelScale(unsigned char accel_range);

    //  Initialize AK8963 Wthin MPU9250
    int InitializeAk8963();

    //  Initialize MPU9250
    int InitializeGyroAccelSensor(
        unsigned char gyro_range,   //  Gyro Range
        unsigned char accel_range); //  Acceleration Range

    //  Close MPU9250
    int CloseGyroAccelSensor();

    //  Set DLPF Bandwidth
    int SetDlpfBandwidth(
        unsigned char gyro_bandwidth,  //  Gyro Bandwidth
        unsigned char accel_bandwidth, //  Accerelation Bandwidth
        unsigned char srd);            //

    //  Read Data From Any Register On AK8963
    void ReadDataFromAk8963(
        unsigned char address, // Address
        unsigned char *data,   //  Recived Data
        int bytes);            //  Data Size

    //  Write Data In Any Register On AK8963
    void WriteData2Ak8963(
        unsigned char address, // Address
        unsigned char data);   //  Writing Data

    //  Get Anglar Velocities And Velocities
    void GetVelocitoesandAccelerations();

    //  Get Geomagnisms
    void GetGeomagnetisms();

    //  Get Euler Angles
    void GetEulerRadAngles();

    //  Get Euler Angles
    void GetEulerDegAngles();

    void CalibrateMagnimeter(double lr, double stop, int step); //  Learning Rate
};
#endif // FLIGHTCONTROLLER_SRC_MPU9250_HPP_