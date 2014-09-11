//
//  LibModBusWrap.h
//  CHAOS_SWPowerSupplyModbusDD
//
//  Created by andrea michelotti on 9/10/13.
//
// wrapper to the libmodbus

#ifndef __CHAOS_SWPowerSupplyModbusDD__LibModBusWrap__
#define __CHAOS_SWPowerSupplyModbusDD__LibModBusWrap__

#include <iostream>
#include "common/modbus/core/ModBusWrapper.h"

extern "C" {
#include "modbus/modbus.h"
};

namespace common{
    namespace modbus {
               
        class LibModBusWrap:public AbstractModbus {
            
        private:
            uint8_t *tab_rp_bits;
            uint16_t *tab_rp_registers;
            uint16_t *tab_rp_registers_bad;
            modbus_t *ctx;
        public:
            LibModBusWrap(){}
            // tcp
            LibModBusWrap(const char* ip,int port);
            //serial
            LibModBusWrap(const char* serial,int baudrate,char parity,int bits,int stop);
            ~LibModBusWrap();
            int init(const char* ip,int port);
            int init(const char* serialdev,int baudrate,char parity,int bits,int stop);
            int init(std::string initialization);
            int deinit();
            bool connect(void);
            int flush(void);
            int close(void);
            void set_write_timeo(uint32_t us);
            void set_read_timeo(uint32_t us);
            uint32_t get_write_timeo();
            uint32_t get_read_timeo();
            int read_coil_status(int addr,int nb, uint8_t*dest,int slave_id=-1);
            int read_input_status(int addr,int nb, uint8_t*dest,int slave_id=-1);
            int read_holding_registers(int addr,int nb, uint16_t*dest,int slave_id=-1);
            int read_input_registers(int addr,int nb, uint16_t*dest,int slave_id=-1);
            int report_slave(uint8_t*desc,int slave_id=-1);
            int force_single_coil(int addr,int status,int slave_id=-1);
            int preset_single_register(int addr,int value,int slave_id=-1);
            int force_multiple_coils(int addr,int nb,uint8_t*src,int slave_id=-1);
            int force_multiple_registers(int addr,int nb,uint16_t*src,int slave_id=-1);
            int write_and_read_registers(int waddr,int wnb,uint16_t*src,int raddr,int rnb,uint16_t*dest,int slave_id=-1);

        };
    }
}

#endif /* defined(__CHAOS_SWPowerSupplyModbusDD__LibModBusWrap__) */
