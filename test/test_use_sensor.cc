#include "../src/use_sensor.hpp"
#include <gtest/gtest.h>
#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <string>

const bool assert_bool = true;

/* template<typename Expected, typename F, typename... Args>
using check_r = std::is_same<Expected, typename std::invoke_result<F, Args...>::type>; */

TEST(TestUseSensorClass, Ushort2Double)
{
    UseSensorClass UseSensor;

    // 戻り値の型チェック
    /*  EXPECT_EQ(false, (typeid(double) == typeid(std::invoke_result<decltype(&UseSensorClass::Ushort2Double), unsigned short>::type)));
    const bool temp_bool = check_r<double, decltype(&UseSensorClass::Ushort2Double), unsigned short>::value;
    EXPECT_EQ(assert_bool, temp_bool);
 */
    // 戻り値そのもののチェック
    unsigned short test_number = 1;
    double assert_number = 1.0;
    ASSERT_DOUBLE_EQ(assert_number, UseSensor.Ushort2Double(test_number));
}

TEST(TestUseSensorClass, Ushort2Signed)
{
    UseSensorClass UseSensor;

    // 戻り値そのもののチェック
    unsigned short test_number = 1;
    double asssert_number = 1.0;
    ASSERT_DOUBLE_EQ(asssert_number, UseSensor.Ushort2Signed(test_number));
}

/* TEST(TestUseSensorClass, OpenColoseSpiDevice)
{
    UseSensorClass UseSensor;


    //　セルフテストEnable/Disableに読み書きして、てすとする。
    // Set Gyro Musurement Range
    WriteData2SpiDevice(fd, MPU9250_GYRO_CONFIG, data[0]);
    this_thread::sleep_for(chrono::microseconds(100000));
    ReadDatafromSpiDevice(fd, MPU9250_GYRO_CONFIG, data, 1);
    cout << "MPU9250_GYRO_CONFIG : 0x%2x"
         << "\n"
         << data[0] << endl;

}

TEST(TestUseSensorClass, ReadWriteDatafromSpiDevice)
{
    UseSensorClass UseSensor;

} */