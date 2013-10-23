//
//  ModBusWrapper.cpp
//  CHAOS_SWPowerSupplyModbusDD
//
//  Created by andrea michelotti on 9/10/13.
//
//

#include "ModBusWrapper.h"
/*
namespace common{
    namespace modbus {
        template <class T> void ModBusWrapper<T>::init(const char* ip,int port){wrap.init(ip,port);};
        template <class T> void ModBusWrapper<T>::init(const char* serialdev,int baudrate,char parity,int bits,int stop){wrap.init(serialdev,baudrate,parity,bits,stop);};
        template <class T> void ModBusWrapper<T>::deinit(){wrap.deinit();}
        template <class T> bool ModBusWrapper<T>::connect(void){return wrap.connect();}
        template <class T> int ModBusWrapper<T>::flush(void){return wrap.flush();};
        template <class T> int ModBusWrapper<T>::close(void){return wrap.close()};
        template <class T> void ModBusWrapper<T>::set_write_timeo(uint32_t us);
        template <class T> void ModBusWrapper<T>::set_read_timeo(uint32_t us);
        template <class T> uint32_t ModBusWrapper<T>::set_write_timeo();
        template <class T> void ModBusWrapper<T>::get_read_timeo(uint32_t us);
        template <class T> int ModBusWrapper<T>::read_coil_status(int addr,int nb, uint8_t*dest);
        template <class T> int ModBusWrapper<T>::read_input_status(int addr,int nb, uint8_t*dest);
        template <class T> int ModBusWrapper<T>::read_holding_registers(int addr,int nb, uint16_t*dest);
        template <class T> int ModBusWrapper<T>::read_input_registers(int addr,int nb, uint16_t*dest);
        template <class T> int ModBusWrapper<T>::report_slave(uint8_t*desc);
        template <class T> int ModBusWrapper<T>::force_single_coil(int addr,int status);
        template <class T> int ModBusWrapper<T>::preset_single_register(int addr,int value);
        template <class T> int ModBusWrapper<T>::force_multiple_coils(int addr,int nb,uint8_t*src);
        template <class T> int ModBusWrapper<T>::force_multiple_registers(int addr,int nb,uint16_t*src);
        template <class T> int ModBusWrapper<T>::write_and_read_registers(int waddr,int wnb,uint16_t*src,int raddr,int rnb,uint16_t*dest);
        
    }
 
}
*/