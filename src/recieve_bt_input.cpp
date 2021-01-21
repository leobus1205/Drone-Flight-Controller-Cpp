#include "recieve_bt_input.hpp"

using namespace std;

char buf[1024];
const double PI=3.14159;

void recieve_bt_input(bool &flag_loopbreak, std::vector<double> &target_angles, std::vector<double> &outputs){
    bdaddr_t bdaddr_any = {{ 0, 0, 0, 0, 0, 0 }};
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    int sd, client_sd = 0;
    int bytes_read = 0;
    bool bt_check = true;
    socklen_t opt = sizeof(rem_addr);

    vector<string> inputs;
    string input_command;
    vector<double> input_angles;
    double input_lift;

    sd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = bdaddr_any;
    loc_addr.rc_channel = (uint8_t) 1;

    bind(sd, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    listen(sd, 1);
    client_sd = accept(sd, (struct sockaddr *)&rem_addr, &opt);
    ba2str(&rem_addr.rc_bdaddr, buf);
    fprintf(stderr, "accepted connection from %s\n", buf);

    thread thread_bt_input(read_inputs, ref(bt_check), client_sd, ref(bytes_read));

    while(1){
        if( bytes_read > 0 ){
            inputs = split(buf,',');
            input_command = inputs[0];

            if(input_command.compare("angle") == 0){
                //単位を[rad]に変換
                input_angles = { stod(inputs[1].c_str())*PI/180, stod(inputs[2].c_str())*PI/180, stod(inputs[3].c_str())*PI/180 };
                target_angles = input_angles;
                for(int i=0;i<input_angles.size();i++) cout << input_angles[i] << ", ";
                cout << endl;
            }else if(input_command.compare("lift") == 0){
                input_lift = stod(inputs[1].c_str());
                outputs[3] = input_lift;
                cout << input_lift << endl;
            }else if(input_command.compare("finish") == 0) {
                cout << "Quite." << endl;
                flag_loopbreak = true;
                break;
            }else{
                cout << "error" << endl;
            }
            memset(buf, 0, sizeof(buf));
            bytes_read = 0;
        }
        if(write(client_sd, "1", 1) < 0) break;
    }
    thread_bt_input.detach();

    close(client_sd);
    close(sd);
}

void read_inputs(bool &bt_check, int client_sd, int &bytes_read){
    while(bt_check) bytes_read = read(client_sd, buf, sizeof(buf));
}

vector<string> split(string str, char del) {
    int first = 0;
    int last = str.find_first_of(del);
    vector<string> result;

    while (first < str.size()) {
        string subStr(str, first, last - first);

        result.push_back(subStr);

        first = last + 1;
        last = str.find_first_of(del, first);

        if (last == string::npos) last = str.size();
    }
    return result;
}