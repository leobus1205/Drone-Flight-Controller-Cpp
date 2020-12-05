#ifndef FLIGHTCONTROLLER_SRC_BME280_HPP_
#define FLIGHTCONTROLLER_SRC_BME280_HPP_

#include "use_sensor.hpp"
#include "config_loader.hpp"

#include <chrono>
#include <iostream>
#include <thread>

class BME280 : public UseSensorClass, public LoadConfigFileClass
{
#define BME280_WHO_AM_I 0xD0
#define BME280_CTRL_HUM 0xF2
#define BME280_MEAS 0xF4
#define BME280_CONFIG 0xF5
#define BME280_DATAS_ADDRESS 0xF7
#define BME280_CORRECTIONS_1 0x88
#define BME280_CORRECTIONS_2 0xE1

public:
    int fd_;
    unsigned long int raw_temperature_ = 0.0, raw_pressure_ = 0.0, raw_humidity_ = 0.0;
    double actual_temperature_ = 0.0, actual_pressure_ = 0.0, actual_humidity_ = 0.0;
    signed long int calib_temperature_ = 0;
    unsigned long int calib_pressure_ = 0, calib_humidity_ = 0;
    signed long int t_fine;

    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
    int8_t dig_H1;
    int16_t dig_H2;
    int8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;

    BME280();
    ~BME280();
    unsigned long int CalibrationTemperature(signed long int adc_T);
    unsigned long int CalibrationPressure(signed long int adc_P);
    unsigned long int CalibrationHumidity(signed long int adc_H);
    void GetDatas();
    void CalculateHightValue();
};

#endif // FLIGHTCONTROLLER_SRC_BME280_HPP_