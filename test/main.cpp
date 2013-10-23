//
//  main.cpp
//  test_modbus
//
//  Created by andrea michelotti on 9/16/13.
//  Copyright (c) 2013 andrea michelotti. All rights reserved.
//

#include <iostream>
#include "Modbus.h"
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include "common/modbus/modbus.h"
#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>

static const boost::regex litteral_ip_port("([0-9a-zA-Z]+\\.[0-9a-zA-Z]+\\.[0-9a-zA-Z]+\\.[0-9a-zA-Z]+):([0-9]+)");
// serial_device,baudrate,parity,bits,stop
static const boost::regex serial_parameter("([[::word::]\\/]+):([0-9]+):([EO]):([78]):([01])");
#define USAGE \
 std::cout<<"Usage:"<<argv[0]<<" --mc <communication channel [ip:port or /dev/ttySxx,baudrate:parity:bits:stop]>"<<std::endl;
using namespace std;

unsigned long int getData(const char*what){
    unsigned long int data;
    char sdata[128];
    printf("%s:",what);
    scanf("%s",sdata);
    data = strtoul(sdata,0,0);
    printf("\n");
    return data;
}
int main(int argc, const char * argv[])
{

    boost::program_options::options_description desc("options");
    desc.add_options()("help","help");
    desc.add_options()("mc",boost::program_options::value<std::string>(),"modbus client");
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc,argv, desc),vm);
    boost::program_options::notify(vm);
    common::modbus::AbstractModbus* modbus_drv=NULL;
    std::string parameters;
    boost::smatch match;
    int modbusrtu=0;
    if(vm.count("help")){
        USAGE;
        return 0;
    }
    if(vm.count("mc")){
    } else {
     std::cout<<"Usage:"<<argv[0]<<" --mc <communication channel [ip:port or /dev/ttySxx,baudrate:parity:bits:stop]>"<<std::endl;
        return -1;
    }
    
    parameters = vm["mc"].as<std::string>();
    if(regex_match(parameters,match,litteral_ip_port)){
        std::string ip = match[1];
        std::string port = match[2];
        modbus_drv= new common::modbus::ModBusTcp(ip.c_str(),atoi(port.c_str()));
    } else if(regex_match(parameters,match,serial_parameter)) {
        std::string serial_dev= match[1];
        std::string baudrate=match[2];
        std::string parity = match[3];
        std::string bits = match[4];
        std::string stop = match[5];
        modbus_drv = new common::modbus::ModBusRTU(serial_dev.c_str(),atoi(baudrate.c_str()),parity.c_str()[0],atoi(bits.c_str()),atoi(stop.c_str()));
        modbusrtu=1;
    }
    
    if(modbus_drv==NULL){
        cerr<< "## invalid parameters:"<<parameters<<endl;
        return -2;
    }
    
    if(modbus_drv->connect()!=true){
        cerr<< "## cannot connect to slave"<<endl;
        return -3;
    }
    
    while(1){
        int ch;
        int slave =-1;
        printf("\n1] write register \n");
        printf("2] read register \n");
        printf("3] quit \n");
        ch = getchar();
        if(modbusrtu){
            slave = getData("slave address");
        }
        if(ch=='1'){
            unsigned long address,data;
            address = getData("write address");
            data = getData("data");
            modbus_drv->preset_single_register(address,data,slave);
        } else if(ch=='2'){
            unsigned long address;
            uint16_t data;
            address = getData("read address");
            modbus_drv->read_input_registers(address,1,&data);
            printf("->0x%x(%d)\n",data,data);
        } else {
            break;
        }
    }
    modbus_drv->close();
    delete modbus_drv;
    // insert code here...
    return 0;
}

