#include "use_sensor.hpp"
#include <iostream>

// Written in C-like.

using namespace std;

//  Transfer
static struct spi_ioc_transfer tr;

double UseSensorClass::Ushort2Double(
    unsigned short value)
{
    if ((value & (0x01 << 15)) != 0)
    {
        return -1.0 * ((value ^ 0xFFFF) + 1);
    }
    else
    {
        return value;
    }

    // static_cast<double>(uShort);
}

short int UseSensorClass::Ushort2Signed(unsigned short value)
{
    if ((value & 0x8000) != 0)
    {
        return -1 * ((value ^ 0xFFFF) + 1);
    }
    else
    {
        return value;
    }

    // static_cast<short int>(value)
}

int UseSensorClass::OpenSpiDevice(
    char *path,
    unsigned char mode,
    unsigned char bits,
    unsigned int speed)
{
    //  Open SPI Device
    fd_ = open(path, O_RDWR);
    if (fd_ < 0)
    {
        std::cout << "Failed to open spi device." << std::endl;
        return -1;
    }

    //  Set R/W Mode
    ioctl(fd_, SPI_IOC_WR_MODE, &mode);
    ioctl(fd_, SPI_IOC_RD_MODE, &mode);

    // Set R/W Bit Per Data
    ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd_, SPI_IOC_RD_BITS_PER_WORD, &bits);

    // Set R/W Communication Speed
    ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    ioctl(fd_, SPI_IOC_RD_MAX_SPEED_HZ, &speed);

    // Set Transfer
    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned long)NULL;
    tr.rx_buf = (unsigned long)NULL;
    tr.len = 0;
    tr.delay_usecs = 0;
    tr.bits_per_word = bits;
    tr.speed_hz = speed;
    tr.cs_change = 0;

    return 1;
}

void UseSensorClass::CloseSpiDevice()
{
    close(fd_);
}

void UseSensorClass::ReadDatafromSpiDevice(
    unsigned char address,
    unsigned char *data,
    int bytes)
{
    //  Buffers
    unsigned char buffer_tx[bytes + 1];
    unsigned char buffer_rx[bytes + 1];

    // Set Address to Buffer
    ////    | Is Bit OR Operetor
    ////    0x80 Is The Hexadecimal Literal For Decimal 128
    buffer_tx[0] = address | 0x80;

    // Set Transfer
    tr.tx_buf = (unsigned long)buffer_tx;
    tr.rx_buf = (unsigned long)buffer_rx;
    tr.len = sizeof(buffer_tx);

    // Read A Data
    ioctl(fd_, SPI_IOC_MESSAGE(1), &tr);
    this_thread::sleep_for(chrono::microseconds(500));

    // Copy The Data In Output From Buffer
    for (int i = 0; i < bytes; i++)
    {
        data[i] = buffer_rx[i + 1];
    }
}

void UseSensorClass::WriteData2SpiDevice(
    unsigned char address,
    unsigned char data)
{
    //  Set Address To Buffer
    unsigned char buffer_tx[] = {address, data};
    unsigned char buffer_rx[] = {0, 0};

    //  Set Transfer
    tr.tx_buf = (unsigned long)buffer_tx;
    tr.rx_buf = (unsigned long)buffer_rx;
    tr.len = sizeof(buffer_tx);

    //  Write A Data
    ioctl(fd_, SPI_IOC_MESSAGE(1), &tr);
    this_thread::sleep_for(chrono::microseconds(500));
}