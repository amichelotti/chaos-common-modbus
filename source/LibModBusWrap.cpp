//
//  LibModBusWrap.cpp
//  CHAOS_SWPowerSupplyModbusDD
//
//  Created by andrea michelotti on 9/10/13.
//
//

#include "LibModBusWrap.h"
#include <errno.h>
#include <string>
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
            modbus_set_debug(ctx, TRUE);
            
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
            modbus_set_debug(ctx, TRUE);
            
            return 1;
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
            tm.tv_usec=us;
            if(us==0){
                // no timeout
                tm.tv_sec=-1;
            }
            modbus_set_byte_timeout(ctx, &tm);
        }
        void LibModBusWrap::set_read_timeo(uint32_t us){
            struct timeval tm;
            tm.tv_sec=0;
            tm.tv_usec=us;
            if(us==0){
                // no timeout
                tm.tv_sec=-1;
            }
            modbus_set_response_timeout(ctx, &tm);
        }
        uint32_t LibModBusWrap::get_write_timeo(){
            struct timeval tm;
            modbus_get_byte_timeout(ctx, &tm);
            return tm.tv_usec;
        }
        uint32_t LibModBusWrap::get_read_timeo(){
            struct timeval tm;
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