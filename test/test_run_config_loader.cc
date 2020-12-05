#include "../src/config_loader.hpp"
#include <iostream>

int main()
{
    LoadConfigFileClass LoadConfigFile;
    std::list<double> parameters;

    LoadConfigFile.ReadConfigFileDouble("./conf/physical_parameters.conf", parameters);

    for (auto itr_param = parameters.begin(); itr_param != parameters.end(); ++itr_param)
    {
        std::cout << *itr_param << std::endl;
    }

    LoadConfigFile.ReadConfigFileDouble("./conf/mpu9250.conf", parameters);

    for (auto itr_param = parameters.begin(); itr_param != parameters.end(); ++itr_param)
    {
        std::cout << *itr_param << std::endl;
    }

    return 0;
}