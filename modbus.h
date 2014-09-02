//
//  ModBus.h
//  CHAOS_SWPowerSupplyModbusDD
//
//  Created by andrea michelotti on 9/10/13.
//
// implementation of the modbus tcp/rtu clients

#ifndef __CHAOS_SWPowerSupplyModbusDD__ModBusTcp__
#define __CHAOS_SWPowerSupplyModbusDD__ModBusTcp__

#include <iostream>
#include "common/modbus/core/AbstractModbus.h"
#include "common/modbus/core/LibModBusWrap.h"
#include <string>

namespace common {
    namespace modbus{
        
        template<class MODBUSWRAPPER>
        class ModBusTcpT:public MODBUSWRAPPER {
        public:
            ModBusTcpT(const char* _ip,int _port):MODBUSWRAPPER(_ip,_port){}
            
        };
        
        template<class MODBUSWRAPPER>
        class ModBusRTUT:public MODBUSWRAPPER {
        public:
            ModBusRTUT(const char* serialdev,int baudrate,char parity,int bits,int stop):MODBUSWRAPPER(serialdev,baudrate,parity,bits,stop){}
            
        };

        
        typedef ModBusTcpT<common::modbus::LibModBusWrap> ModBusTcp;
        typedef ModBusRTUT<common::modbus::LibModBusWrap> ModBusRTU;

        
    }
}
#endif /* defined(__CHAOS_SWPowerSupplyModbusDD__ModBusTcp__) */
