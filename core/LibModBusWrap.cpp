//
//  LibModBusWrap.cpp
//  CHAOS_SWPowerSupplyModbusDD
//
//  Created by andrea michelotti on 9/10/13.
//
//

#include "LibModBusWrap.h"
#include <stdio.h>
#include <errno.h>
#include <string>
#include <boost/regex.hpp>
static const boost::regex modbusTcp("(.+):(.+)");
static const boost::regex modbusRTU("([\\w\\/]+):([0-9]+):([ENO]):([78]):([01])");

// tcp
namespace common {
    
    
    namespace modbus {
        LibModBusWrap::~LibModBusWrap(){
            deinit();
        }
        LibModBusWrap::LibModBusWrap(const char* ip,int port){
            ctx = 0;
            init(ip,port);
        }
        //serial
        LibModBusWrap::LibModBusWrap(const char* serial,int baudrate,char parity,int bits,int stop){
            ctx = 0;
            init(serial,baudrate,parity,bits,stop);
        }
        int LibModBusWrap::init(const char* ip,int port) {
            if(ctx ==0){
                ctx = modbus_new_tcp(ip, port);
            }
            
            if (ctx == NULL) {
                fprintf(stderr, "Unable to allocate libmodbus [TCP] context\n");
                return -1;
            }
#ifdef DEBUG
            modbus_set_debug(ctx, TRUE);
#endif
            return 1;
        };
        
        int LibModBusWrap::init(const char* serialdev,int baudrate,char parity,int bits,int stop){
            if(ctx ==0){
                ctx = modbus_new_rtu(serialdev, baudrate, parity, bits, stop);
            }
            
            if (ctx == NULL) {
                fprintf(stderr, "Unable to allocate libmodbus [TCP] context\n");
                return -1;
            }
#ifdef DEBUG
            modbus_set_debug(ctx, TRUE);
#endif
            return 1;
        }
        
        int LibModBusWrap::init(std::string _init){
          boost::smatch match;
            if(regex_match(_init,match,modbusTcp,boost::match_extra)){
                std::string ip,port;
                ip = match[1];
                port =match[2];
                return init(ip.c_str(),atoi(port.c_str()));
                
            } else if(regex_match(_init,match,modbusRTU,boost::match_extra)){
                std::string dev,baudrate,parity,bits,stop;
                int par=-1;
                dev = match[1];
                baudrate = match[2];
                parity = match[3];
                bits =match[4];
                stop = match[5];
		if (parity == "E") par=2;
		if (parity == "O") par=1;
                if(par<0){
                    fprintf(stderr, "bad parity %s\n",parity.c_str());
                    return -2;
                }
                if(bits !="7" || bits!="8"){
                       fprintf(stderr, "bad bits  specification %s\n",bits.c_str());
                    return -4;
                }
                if(stop !="0" || stop!="1"){
                       fprintf(stderr, "bad stop bit  specification %s\n",stop.c_str());
                    return -5;
                }
                
                return init(dev.c_str(),::atoi(baudrate.c_str()),par,atoi(bits.c_str()),atoi(stop.c_str()));
            }
            return -3;
        }
        
        int LibModBusWrap::deinit(){
            if(ctx){
                /* Close the connection */
                modbus_close(ctx);
                modbus_free(ctx);
                ctx =0;
                return 1;
            }
            return -1;
        }
        bool LibModBusWrap::connect(void){
            if (modbus_connect(ctx) == -1) {
                fprintf(stderr, "Connection failed: %s\n",
                        modbus_strerror(errno));
                return false;
            }
            return true;
        }
        int LibModBusWrap::flush(void){
            int ret;
            ret = modbus_flush(ctx);
            if(ret==-1){
                fprintf(stderr, "Error flushing: %s\n",
                        modbus_strerror(errno));
            }
            return ret;
        }
        int LibModBusWrap::close(void){
            /* Close the connection */
            modbus_close(ctx);

            return 0;
        }
        void LibModBusWrap::set_write_timeo(uint32_t us){
            struct timeval tm;
            tm.tv_sec=0;
            tm.tv_usec=0;
            if(us==0){
                // no timeout
                tm.tv_sec=-1;
            } else {
                tm.tv_sec=us/1000000;
                tm.tv_usec=us%1000000;
            }
            modbus_set_byte_timeout(ctx, &tm);
        }
        void LibModBusWrap::set_read_timeo(uint32_t us){
            struct timeval tm;
            tm.tv_sec=0;
            tm.tv_usec=0;
            if(us==0){
                // no timeout
                tm.tv_sec=-1;
            } else {
                tm.tv_sec=us/1000000;
                tm.tv_usec=us%1000000;
            }
            modbus_set_response_timeout(ctx,&tm);
        }
        uint32_t LibModBusWrap::get_write_timeo(){
//            uint32_t tms,tmu;
  //          modbus_get_byte_timeout(ctx, &tms,&tmu);
            struct timeval tm;
            modbus_get_byte_timeout(ctx, &tm);
            return tm.tv_usec;
        }
        uint32_t LibModBusWrap::get_read_timeo(){
            struct timeval tm;
            //uint32_t tms,tmu;
          //  modbus_get_response_timeout(ctx, &tms,&tmu);
            modbus_get_response_timeout(ctx, &tm);
            return tm.tv_usec;
        }
        
        int LibModBusWrap::read_coil_status(int addr,int nb, uint8_t*dest,int slave_id){
            if(slave_id!=-1)
                modbus_set_slave(ctx, slave_id);
            return modbus_read_bits(ctx, addr, nb, dest);
        }
        int LibModBusWrap::read_input_status(int addr,int nb, uint8_t*dest,int slave_id){
            if(slave_id!=-1)
                modbus_set_slave(ctx, slave_id);
            return modbus_read_input_bits(ctx, addr, nb, dest);
        }
        int LibModBusWrap::read_holding_registers(int addr,int nb, uint16_t*dest,int slave_id){
            if(slave_id!=-1)
                modbus_set_slave(ctx, slave_id);
            return modbus_read_registers(ctx, addr, nb, dest);
        }
        int LibModBusWrap::read_input_registers(int addr,int nb, uint16_t*dest,int slave_id){
            if(slave_id!=-1)
                modbus_set_slave(ctx, slave_id);
            return modbus_read_input_registers(ctx, addr, nb, dest);
        }
        int LibModBusWrap::report_slave(uint8_t*desc,int slave_id){
            if(slave_id!=-1)
                modbus_set_slave(ctx, slave_id);
            return modbus_report_slave_id(ctx, desc);
        }
        int LibModBusWrap::force_single_coil(int addr,int status,int slave_id){
            if(slave_id!=-1)
                modbus_set_slave(ctx, slave_id);
            return modbus_write_bit(ctx, addr, status);
        }
        int LibModBusWrap::preset_single_register(int addr,int value,int slave_id){
            if(slave_id!=-1)
                modbus_set_slave(ctx, slave_id);
            return modbus_write_register(ctx, addr, value);
        }
        int LibModBusWrap::force_multiple_coils(int addr,int nb,uint8_t*src,int slave_id){
            if(slave_id!=-1)
                modbus_set_slave(ctx, slave_id);
            return modbus_write_bits(ctx, addr, nb, src);
        }
        int LibModBusWrap::force_multiple_registers(int addr,int nb,uint16_t*src,int slave_id){
            if(slave_id!=-1)
                modbus_set_slave(ctx, slave_id);
            return modbus_write_registers(ctx, addr, nb, src);
        }
        int LibModBusWrap::write_and_read_registers(int waddr,int wnb,uint16_t*src,int raddr,int rnb,uint16_t*dest,int slave_id){
            if(slave_id!=-1)
                modbus_set_slave(ctx, slave_id);
            return modbus_write_and_read_registers(ctx, waddr,wnb, src, raddr, rnb, dest);
        }
    }
}
