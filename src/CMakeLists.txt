cmake_minimum_required(VERSION 3.1)

add_library(ReadConfigFile STATIC config_loader.cc)

add_library(UseSensorClass STATIC use_sensor.cc)

add_library(9AxisSensor STATIC mpu9250.cc)
target_link_libraries(
    9AxisSensor
    ReadConfigFile
    UseSensorClass
)

add_library(KalmanFilter STATIC kalman_filter.cc)
target_link_libraries(
    KalmanFilter
    ReadConfigFile
)

add_library(HightSensor STATIC bme280.cc)
target_link_libraries(
    HightSensor
    ReadConfigFile
    UseSensorClass
)

add_library(MotorControl STATIC motor_control.cc)
target_include_directories(
    MotorControl
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include/pigpio
)

add_library(DescretePidController STATIC descrete_pid_controller.cc)
target_link_libraries(
    DescretePidController
    ReadConfigFile
)

add_library(GetInertialHeight STATIC get_inertial_height.cc)
target_link_libraries(
    GetInertialHeight
    ReadConfigFile
)

# ライブラリファイル名の指定
target_link_libraries(MotorControl pigpio)