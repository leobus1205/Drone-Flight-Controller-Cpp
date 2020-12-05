#ifndef FLIGHTCONTROLLER_SRC_USESENSOR_HPP_
#define FLIGHTCONTROLLER_SRC_USESENSOR_HPP_

#include <chrono>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include <stdlib.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define _USE_MATH_DEFINES
#include <cmath>

/** クラスの概要説明 */
class UseSensorClass
{
#define SPI_DEVICE_PATH_0 "/dev/spidev0.0"
#define SPI_DEVICE_PATH_1 "/dev/spidev0.1"
#define SPI_BITS_PER_WORD 8
#define SPI_SPEED 1000000

public:
    const double DEG2RAD = M_PI / 180.0;
    const double RAD2DEG = 180.0 / M_PI;
    int fd_;

    //  Return Converted Double
    double Ushort2Double(
        unsigned short); // Original Data

    //  Convert Unsigned To Signed
    short int Ushort2Signed(
        unsigned short); //  Original Data

    //  Open SPI Device
    int OpenSpiDevice(
        char *,        //  Path To SPI Device
        unsigned char, //  Drive Mode
        unsigned char, //  Bit Per data
        unsigned int); //  Communication Speed

    //  Close SPI Device
    void CloseSpiDevice();

    //  Read A Data In Register
    void ReadDatafromSpiDevice(
        unsigned char,   //  Target Address
        unsigned char *, //  Recived Data
        int);            //  Data Size

    //  Write A Data In Register
    void WriteData2SpiDevice(
        unsigned char,  //   Target Address
        unsigned char); //   Writing Datab
};

#endif // FLIGHTCONTROLLER_SRC_USESENSOR_HPP_