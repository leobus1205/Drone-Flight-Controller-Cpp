#include "config_loader.hpp"

//  ファイル名からファイルを開き、stringのlistに格納する。ポインタで直接格納する。
bool LoadConfigFileClass::ReadConfigFileDouble(std::string filename, std::list<double> &parameters)
{
    std::ifstream ifs(filename);
    std::string configname, parameter;
    double buf;

    if (ifs.fail())
    {
        std::cerr << "Failed to open file." << std::endl;
        return false;
    }

    while (ifs >> configname >> parameter)
    {
        buf = std::stod(parameter);
        parameters.push_back(buf);
    };

    return true;
}

bool LoadConfigFileClass::ReadConfigFileBool(std::string filename, std::list<bool> &parameters)
{
    std::ifstream ifs(filename);
    std::string configname, parameter;
    bool buf;

    if (ifs.fail())
    {
        std::cerr << "Failed to open file." << std::endl;
        return false;
    }

    while (ifs >> configname >> parameter)
    {
        std::istringstream(parameter) >> std::boolalpha >> buf;
        parameters.push_back(buf);
    };

    return true;
}