//
//  Modbus.h
//
//
//  Created by andrea michelotti on 9/10/13.
//
// implementation of the modbus tcp/rtu clients

#ifndef __COMMON_MODBUS_H__
#define __COMMON_MODBUS_H__

#include <iostream>
#include <common/modbus/core/AbstractModbus.h>
#include <common/modbus/core/LibModBusWrap.h>
#include <chaos/common/utility/SingletonOnArguments.h>
#include <string>
#include <boost/regex.hpp>

namespace common {
  namespace modbus {
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

        template<class MODBUSWRAPPER>
        class ModBus:public MODBUSWRAPPER, public chaos::common::utility::SingletonOnArguments<ModBus<MODBUSWRAPPER> >{

        protected:
            int callInit(std::string initialisation){return static_cast<MODBUSWRAPPER*>(this)->init(initialisation);}

        public:
            ModBus(){};

            ~ModBus(){

            }

        };

        typedef ModBusTcpT< ::common::modbus::LibModBusWrap> ModBusTcp;
        typedef ModBusRTUT< ::common::modbus::LibModBusWrap> ModBusRTU;
        typedef ModBus< ::common::modbus::LibModBusWrap> ModBusDrv;


  }
}
#endif 
