#include "../src/config_loader.hpp"
#include <gtest/gtest.h>

TEST(TestReadConfigFile, ReadDouble)
{
    LoadConfigFileClass LoadConfigFile;
    std::list<double> parameters;
    std::list<double> assert_parameters = {1.000, -1.234, 5.678};

    LoadConfigFile.ReadConfigFileDouble("../conf/test_config_double.conf", parameters);

    auto itr_assert = assert_parameters.begin();
    for (auto itr_param = parameters.begin(); itr_param != parameters.end(); ++itr_param)
    {
        EXPECT_DOUBLE_EQ(*itr_assert, *itr_param);
        itr_assert++;
    }
}

TEST(TestReadConfigFile, ReadBool)
{
    LoadConfigFileClass LoadConfigFile;
    std::list<bool> parameters;
    std::list<bool> assert_parameters = {true, false};

    LoadConfigFile.ReadConfigFileBool("../conf/test_config_bool.conf", parameters);

    auto itr_assert = assert_parameters.begin();
    for (auto itr_param = parameters.begin(); itr_param != parameters.end(); ++itr_param)
    {
        EXPECT_EQ(*itr_assert, *itr_param);
        itr_assert++;
    }
}