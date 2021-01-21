#ifndef INCLUDED_RECIEVE_BT_INPUT_HPP
#define INCLUDED_RECIEVE_BT_INPUT_HPP

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
// #include "motor_control.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <thread>

using namespace std;

void recieve_bt_input(bool &flag_loopbreak, vector<double> &target_angles, vector<double> &outputs);
void read_inputs(bool&, int, int&);
vector<string> split(string, char);

#endif