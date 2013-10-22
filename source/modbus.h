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
#include "AbstractModbus.h"
#include "ModBusWrapper.h"
#include "LibModBusWrap.h"
#include <string>

namespace common {
    namespace modbus{
        
        template<class MODBUSWRAPPER>
        class ModBusTcpT:public ModBusWrapper<MODBUSWRAPPER>{
        public:
            ModBusTcpT(const char* _ip,int _port){ModBusWrapper<MODBUSWRAPPER>::init(_ip,_port);}
            
        };
        
        template<class MODBUSWRAPPER>
        class ModBusRTUT:public ModBusWrapper<MODBUSWRAPPER>{
        public:
            ModBusRTUT(const char* serialdev,int baudrate,char parity,int bits,int stop){ModBusWrapper<MODBUSWRAPPER>::init(serialdev,baudrate,parity,bits,stop);}
            
        };

        
        typedef ModBusTcpT<common::modbus::LibModBusWrap> ModBusTcp;
        typedef ModBusRTUT<common::modbus::LibModBusWrap> ModBusRTU;

        
    }
}
#endif /* defined(__CHAOS_SWPowerSupplyModbusDD__ModBusTcp__) */
