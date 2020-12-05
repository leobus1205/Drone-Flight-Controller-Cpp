#include "bme280.hpp"

BME280::BME280()
{
    uint8_t osrs_t = 1;   //Temperature oversampling x 1
    uint8_t osrs_p = 1;   //Pressure oversampling x 1
    uint8_t osrs_h = 1;   //Humidity oversampling x 1
    uint8_t mode = 3;     //Normal mode
    uint8_t t_sb = 7;     //Tstandby 0.5ms
    uint8_t filter = 0;   //Filter off
    uint8_t spi3w_en = 0; //3-wire SPI Disable

    int convert_data = 0;

    unsigned char ctrl_meas_reg[1];
    ctrl_meas_reg[0] = (osrs_t << 5) | (osrs_p << 2) | mode;
    convert_data = ctrl_meas_reg[0];
    std::cout << "ctrl_meas_reg :\t0x" << std::hex << convert_data << std::endl;
    unsigned char config_reg[1];
    config_reg[0] = (t_sb << 5) | (filter << 2) | spi3w_en;
    convert_data = config_reg[0];
    std::cout << "config_reg :\t0x" << std::hex << convert_data << std::endl;
    unsigned char ctrl_hum_reg[1];
    ctrl_hum_reg[0] = osrs_h;
    convert_data = ctrl_hum_reg[0];
    std::cout << "ctrl_hum_reg :\t0x" << std::hex << convert_data << std::endl;

    unsigned char data[1];

    this->OpenSpiDevice(SPI_DEVICE_PATH_1, SPI_MODE_0, SPI_BITS_PER_WORD, SPI_SPEED);
    this->ReadDatafromSpiDevice(BME280_WHO_AM_I, data, sizeof(data));

    convert_data = data[0];
    std::cout << "Who Am I :\t0x" << std::hex << convert_data << std::endl;

    /* this->WriteData2SpiDevice(BME280_MEAS, ctrl_meas_reg);
    this->WriteData2SpiDevice(BME280_CONFIG, config_reg);
    this->WriteData2SpiDevice(BME280_CTRL_HUM, ctrl_hum_reg); */
    data[0] = 0;
    this->WriteData2SpiDevice(BME280_MEAS, data[0]);
    this->WriteData2SpiDevice(BME280_CONFIG, data[0]);
    this->WriteData2SpiDevice(BME280_CTRL_HUM, data[0]);

    this->ReadDatafromSpiDevice(BME280_MEAS, data, 1);
    convert_data = data[0];
    std::cout << "BME280_MEAS :\t0x" << std::hex << convert_data << std::endl;
    this->ReadDatafromSpiDevice(BME280_CONFIG, data, sizeof(data));
    convert_data = data[0];
    std::cout << "BME280_CONFIG :\t0x" << std::hex << convert_data << std::endl;
    this->ReadDatafromSpiDevice(BME280_CTRL_HUM, data, sizeof(data));
    convert_data = data[0];
    std::cout << "BME280_CTRL_HUM :\t0x" << std::hex << convert_data << std::endl;

    unsigned char data_up[24], data_down[9]; // Fix 2014/04/06
    this->ReadDatafromSpiDevice(BME280_CORRECTIONS_1, data_up, sizeof(data_up));
    this->ReadDatafromSpiDevice(BME280_CORRECTIONS_2, data_down, sizeof(data_down));

    dig_T1 = (data_up[1] << 8) | data_up[0];
    dig_T2 = (data_up[3] << 8) | data_up[2];
    dig_T3 = (data_up[5] << 8) | data_up[4];
    dig_P1 = (data_up[7] << 8) | data_up[6];
    dig_P2 = (data_up[9] << 8) | data_up[8];
    dig_P3 = (data_up[11] << 8) | data_up[10];
    dig_P4 = (data_up[13] << 8) | data_up[12];
    dig_P5 = (data_up[15] << 8) | data_up[14];
    dig_P6 = (data_up[17] << 8) | data_up[16];
    dig_P7 = (data_up[19] << 8) | data_up[18];
    dig_P8 = (data_up[21] << 8) | data_up[20];
    dig_P9 = (data_up[23] << 8) | data_up[22];
    dig_H1 = data_down[0];
    dig_H2 = (data_down[2] << 8) | data_down[1];
    dig_H3 = data_down[3];
    dig_H4 = (data_down[4] << 4) | (0x0F & data_down[5]);
    dig_H5 = (data_down[7] << 4) | ((data_down[6] >> 4) & 0x0F); // Fix 2014/04/06
    dig_H6 = data_down[8];                                       // Fix 2014/04/06
}

BME280::~BME280()
{
    this->CloseSpiDevice();
}

unsigned long int BME280::CalibrationTemperature(signed long int adc_T)
{
    signed long int var1, var2, T;
    var1 = ((((adc_T >> 3) - ((signed long int)dig_T1 << 1))) * ((signed long int)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((signed long int)dig_T1)) * ((adc_T >> 4) - ((signed long int)dig_T1))) >> 12) * ((signed long int)dig_T3)) >> 14;

    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;

    return T;
}

unsigned long int BME280::CalibrationPressure(signed long int adc_P)
{
    signed long int var1, var2;
    unsigned long int P;
    var1 = (((signed long int)t_fine) >> 1) - (signed long int)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((signed long int)dig_P6);
    var2 = var2 + ((var1 * ((signed long int)dig_P5)) << 1);
    var2 = (var2 >> 2) + (((signed long int)dig_P4) << 16);
    var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((signed long int)dig_P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((signed long int)dig_P1)) >> 15);
    if (var1 == 0)
    {
        return 0;
    }
    P = (((unsigned long int)(((signed long int)1048576) - adc_P) - (var2 >> 12))) * 3125;
    if (P < 0x80000000)
    {
        P = (P << 1) / ((unsigned long int)var1);
    }
    else
    {
        P = (P / (unsigned long int)var1) * 2;
    }
    var1 = (((signed long int)dig_P9) * ((signed long int)(((P >> 3) * (P >> 3)) >> 13))) >> 12;
    var2 = (((signed long int)(P >> 2)) * ((signed long int)dig_P8)) >> 13;
    P = (unsigned long int)((signed long int)P + ((var1 + var2 + dig_P7) >> 4));

    return P;
}

unsigned long int BME280::CalibrationHumidity(signed long int adc_H)
{
    signed long int v_x1;

    v_x1 = (t_fine - ((signed long int)76800));
    v_x1 = (((((adc_H << 14) - (((signed long int)dig_H4) << 20) - (((signed long int)dig_H5) * v_x1)) +
              ((signed long int)16384)) >>
             15) *
            (((((((v_x1 * ((signed long int)dig_H6)) >> 10) *
                 (((v_x1 * ((signed long int)dig_H3)) >> 11) + ((signed long int)32768))) >>
                10) +
               ((signed long int)2097152)) *
                  ((signed long int)dig_H2) +
              8192) >>
             14));
    v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * ((signed long int)dig_H1)) >> 4));
    v_x1 = (v_x1 < 0 ? 0 : v_x1);
    v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);

    return (unsigned long int)(v_x1 >> 12);
}

void BME280::GetDatas()
{
    unsigned char data_2[8];

    this->ReadDatafromSpiDevice(BME280_DATAS_ADDRESS, data_2, sizeof(data_2));

    raw_temperature_ = (data_2[0] << 12) | (data_2[1] << 4) | (data_2[2] >> 4);
    raw_pressure_ = (data_2[3] << 12) | (data_2[4] << 4) | (data_2[5] >> 4);
    raw_humidity_ = (data_2[6] << 8) | data_2[7];

    calib_temperature_ = this->CalibrationTemperature(raw_temperature_);
    calib_pressure_ = this->CalibrationPressure(raw_pressure_);
    calib_humidity_ = this->CalibrationHumidity(raw_humidity_);
    actual_temperature_ = (double)calib_temperature_ / 100.0;
    actual_pressure_ = (double)calib_pressure_ / 1024.0;
    actual_humidity_ = (double)calib_humidity_ / 100.0;

    printf("temp_raw:%lf\tpress_raw:%lf\thum_raw:%f\n", (double)raw_temperature_, (double)raw_pressure_, (double)raw_humidity_);
    printf("temp_cal:%lf\tpress_cal:%lf\thum_cal:%f\n", (double)calib_temperature_, (double)calib_pressure_, (double)calib_humidity_);
    printf("temp_act:%lf\tpress_act:%lf\thum_act:%f\n", (double)actual_temperature_, (double)actual_pressure_, (double)actual_humidity_);
    printf("\n");
}

void BME280::CalculateHightValue()
{
}