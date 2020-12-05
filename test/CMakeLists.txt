cmake_minimum_required(VERSION 3.1)

# Clone Gtest in this roject.
include(${PROJECT_SOURCE_DIR}/cmake/DownloadProject/DownloadProject.cmake)
download_project(PROJ googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG master
                UPDATE_DISCONNECTED 1
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

# Create Executable
# ReadConfigFile
add_executable(
    TestReadConfigFile.out
    ${PROJECT_SOURCE_DIR}/test/test_config_loader.cc
)
target_link_libraries(
    TestReadConfigFile.out
    ReadConfigFile
    gtest_main
)
add_executable(
    TestRunReadConfigFile.out
    ${PROJECT_SOURCE_DIR}/test/test_run_config_loader.cc
)
target_link_libraries(
    TestRunReadConfigFile.out
    ReadConfigFile
)

# UseSensorClass
add_executable(
    TestUseSensorClass.out
    ${PROJECT_SOURCE_DIR}/test/test_use_sensor.cc
)
target_link_libraries(
    TestUseSensorClass.out
    UseSensorClass
    gtest_main
)

# 9AxisSensor
add_executable(
    Test9AxisSensor.out
    ${PROJECT_SOURCE_DIR}/test/test_mpu9250.cc
)
target_link_libraries(
    Test9AxisSensor.out
    9AxisSensor
    gtest_main
)
add_executable(
    TestRun9AxisSensor.out
    ${PROJECT_SOURCE_DIR}/test/test_run_mpu9250.cc
)
target_link_libraries(
    TestRun9AxisSensor.out
    9AxisSensor
)
add_executable(
    CaliblateMagnimeter.out
    ${PROJECT_SOURCE_DIR}/test/caliblate_mag.cc
)
target_link_libraries(
    CaliblateMagnimeter.out
    9AxisSensor
)

# KalmanFilter
#add_executable(
#    TestKalmanFilter.out
#    ${PROJECT_SOURCE_DIR}/test/test_kalman_filter.cc
#)
#target_link_libraries(
#    TestKalmanFilter.out
#    KalmanFilter
#    gtest_main
#)
add_executable(
    TestRunKalmanFilter.out
    ${PROJECT_SOURCE_DIR}/test/test_run_kalman_filter.cc
)
target_link_libraries(
    TestRunKalmanFilter.out
    KalmanFilter
    9AxisSensor
)

# Hight Sensor
add_executable(
    TestRunHightSensor.out
    ${PROJECT_SOURCE_DIR}/test/test_run_bme280.cc
)
target_link_libraries(
    TestRunHightSensor.out
    HightSensor
)

include_directories(${PROJECT_SOURCE_DIR}/src ${GTEST_INCLUDE_DIRS})
include(GoogleTest)

# Motor Control
add_executable(
    TestRunMotors.out
    ${PROJECT_SOURCE_DIR}/test/test_run_motors.cc
)
target_link_libraries(
    TestRunMotors.out
    MotorControl
)

gtest_add_tests(TARGET TestReadConfigFile.out)
gtest_add_tests(TARGET TestUseSensorClass.out)
gtest_add_tests(TARGET Test9AxisSensor.out)
#gtest_add_tests(TARGET TestRunKalmanFilter.out)

