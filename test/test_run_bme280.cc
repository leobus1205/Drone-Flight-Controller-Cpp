#include "../src/bme280.hpp"
#include <stdio.h>

int main()
{
    BME280 BME280;

    for (int i = 0; i < 10; i++)
    {
        BME280.GetDatas();

        printf("T:\t%lf\tP:\t%lf\tH:\t%lf\t", BME280.actual_temperature_, BME280.actual_pressure_, BME280.actual_humidity_);
    }

    return 0;
}