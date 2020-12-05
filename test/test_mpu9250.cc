#include "../src/mpu9250.hpp"
#include <gtest/gtest.h>

TEST(Test9AxisSensor, TestMpu9250Access)
{
    UseSensorClass UseSensor;

    unsigned char data[3];
    UseSensor.OpenSpiDevice(SPI_DEVICE_PATH_0, SPI_MODE_0, SPI_BITS_PER_WORD, SPI_SPEED);

    EXPECT_EQ(true, UseSensor.fd_ >= 0);
}

TEST(Test9AxisSensor, TestMpu9250ReadData)
{
    UseSensorClass UseSensor;

    unsigned char data[3];
    UseSensor.OpenSpiDevice(SPI_DEVICE_PATH_0, SPI_MODE_0, SPI_BITS_PER_WORD, SPI_SPEED);
    UseSensor.ReadDatafromSpiDevice(MPU9250_WHO_AM_I, data, 1);

    int convert_data = data[0];
    int assert_data = 113;

    EXPECT_EQ(assert_data, convert_data);
}