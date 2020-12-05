#ifndef FLIGHTCONTROLLER_SRC_CONFIGLOADER_HPP_
#define FLIGHTCONTROLLER_SRC_CONFIGLOADER_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>

class LoadConfigFileClass
{
private:
    /* data */
public:
    bool ReadConfigFileDouble(std::string filename, std::list<double> &parameters);
    bool ReadConfigFileBool(std::string filename, std::list<bool> &parameters);
};

#endif // FLIGHTCONTROLLER_SRC_CONFIGLOADER_HPP_