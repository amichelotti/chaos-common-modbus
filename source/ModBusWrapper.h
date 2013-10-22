//
//  ModBusWrapper.h
//  CHAOS_SWPowerSupplyModbusDD
//
//  Created by andrea michelotti on 9/10/13.
//
//

#ifndef __CHAOS_SWPowerSupplyModbusDD__ModBusWrapper__
#define __CHAOS_SWPowerSupplyModbusDD__ModBusWrapper__

#include <iostream>
#include "AbstractModbus.h"

namespace common{
    namespace modbus {
        template <class WRAPPER>
        class ModBusWrapper:public AbstractModbus{
            public:
            ModBusWrapper(){}
            ~ModBusWrapper(){static_cast<WRAPPER*>(this)->deinit();}

            int init(const char* ip,int port) {static_cast<WRAPPER*>(this)->init(ip,port);};
            int init(const char* serialdev,int baudrate,char parity,int bits,int stop){static_cast<WRAPPER*>(this)->init(serialdev,baudrate,parity,bits,stop);}
            int deinit(){static_cast<WRAPPER*>(this)->deinit();}
            
            bool connect(void){return static_cast<WRAPPER*>(this)->connect();}
            int flush(void){return static_cast<WRAPPER*>(this)->flush();}
            int close(void){return static_cast<WRAPPER*>(this)->close();}
            void set_write_timeo(uint32_t us){static_cast<WRAPPER*>(this)->set_write_timeo(us);}
            void set_read_timeo(uint32_t us){static_cast<WRAPPER*>(this)->set_read_timeo(us);}
            uint32_t get_write_timeo(){return static_cast<WRAPPER*>(this)->get_write_timeo();}
            uint32_t get_read_timeo(){return static_cast<WRAPPER*>(this)->get_read_timeo();}
            
            int read_coil_status(int addr,int nb, uint8_t*dest,int slave_id=-1){return static_cast<WRAPPER*>(this)->read_coil_status(addr,nb, dest,slave_id);}
            int read_input_status(int addr,int nb, uint8_t*dest,int slave_id=-1){return static_cast<WRAPPER*>(this)->read_input_status(addr,nb, dest,slave_id);}
            int read_holding_registers(int addr,int nb, uint16_t*dest,int slave_id=-1){return static_cast<WRAPPER*>(this)->read_holding_registers(addr,nb, dest,slave_id);}
            int read_input_registers(int addr,int nb, uint16_t*dest,int slave_id=-1){return static_cast<WRAPPER*>(this)->read_input_registers(addr,nb, dest,slave_id);}
            int report_slave(uint8_t*desc,int slave_id=-1){return static_cast<WRAPPER*>(this)->report_slave(desc,slave_id);}
            int force_single_coil(int addr,int status,int slave_id=-1){return static_cast<WRAPPER*>(this)->force_single_coil(addr,status,slave_id);}
            int preset_single_register(int addr,int value,int slave_id=-1){return static_cast<WRAPPER*>(this)->preset_single_register(addr,value,slave_id);}
            int force_multiple_coils(int addr,int nb,uint8_t*src,int slave_id=-1){return static_cast<WRAPPER*>(this)->force_multiple_coils(addr,nb,src,slave_id);}
            int force_multiple_registers(int addr,int nb,uint16_t*src,int slave_id=-1){return static_cast<WRAPPER*>(this)->force_multiple_registers(addr,nb,src,slave_id);}
            int write_and_read_registers(int waddr,int wnb,uint16_t*src,int raddr,int rnb,uint16_t*dest,int slave_id=-1){return static_cast<WRAPPER*>(this)->write_and_read_registers(waddr,wnb,src,raddr,rnb,dest,slave_id);}
            
        };
    }
}
#endif /* defined(__CHAOS_SWPowerSupplyModbusDD__ModBusWrapper__) */
