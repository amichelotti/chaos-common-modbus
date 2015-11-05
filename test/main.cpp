//
//  main.cpp
//  test_modbus
//
//  Created by andrea michelotti on 9/16/13.
//  Copyright (c) 2013 andrea michelotti. All rights reserved.
//

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include "common/modbus/modbus.h"
#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>

static const boost::regex litteral_ip_port("([0-9a-zA-Z]+\\.[0-9a-zA-Z]+\\.[0-9a-zA-Z]+\\.[0-9a-zA-Z]+):([0-9]+)");
// serial_device,baudrate,parity,bits,stop
static const boost::regex serial_parameter("([[\\w]\\/]+):([0-9]+):([ENO]):([78]):([01])");
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
    int slave =-1;

    boost::program_options::options_description desc("options");
    desc.add_options()("help","help");
    desc.add_options()("mc",boost::program_options::value<std::string>(),"modbus client");
    desc.add_options()("sv,slave",boost::program_options::value<int>(),"modbus slave");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc,argv, desc),vm);
    boost::program_options::notify(vm);
    common::modbus::AbstractModbus* modbus_drv=NULL;
    std::string parameters;
    boost::smatch match;
    int modbusrtu=0;
    if(vm.count("help")){
        cout<<"Usage:"<<desc<<endl;
        return 0;
    }
    if(vm.count("mc")){
    } else {
     std::cout<<"Usage:"<<argv[0]<<" --mc <communication channel [ip:port or /dev/ttySxx,baudrate:parity:bits:stop]>"<<std::endl;
        return -1;
    }
    
    parameters = vm["mc"].as<std::string>();
    if(vm.count("sv")){
        slave = vm["sv"].as<int>();
        printf("* setting slave:%d\n",slave);
    }
    
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
    modbus_drv->set_read_timeo(5000000);
    modbus_drv->set_write_timeo(5000000);
    while(1){
        int ch;
        int err;
        printf("\n1] write register \n");
        printf("2] read register \n");
        printf("3] read32 register \n");

        printf("q] quit \n");
        ch = getchar();
        if(modbusrtu){
            slave = getData("slave address");
        }
        if(ch=='1'){
            unsigned long address,data;
            address = getData("write address");
            data = getData("data");
            if((err=modbus_drv->preset_single_register(address,data,slave))<0){
                     printf("## error presetting single register err:%d\n",err);
                     return err;

            }
        } else if(ch=='2'){
            unsigned long address;
            uint32_t data=0,toconv;
            
            address = getData("read address");
            if((err=modbus_drv->read_input_registers(address,1,(uint16_t*)&data,slave))<0){
                printf("## error read single register err:%d\n",err);
                return err;
            }
            printf("%d->0x%x(%d)\n",err,data,data);
        } else if(ch=='3'){
            unsigned long address;
            uint32_t data;
            
            float* dataf=(float*)&data;
            address = getData("read32 address");
            if((err=modbus_drv->read_input_registers(address,2,(uint16_t*)&data,slave))<0){
                printf("## error reading err:%d\n",err);
                return err;
            }
            
            printf("%d->0x%x(%d) float:%f\n",err,data,data,*dataf);
        } else {
            break;
        }
    }
    modbus_drv->close();
    delete modbus_drv;
    // insert code here...
    return 0;
}

