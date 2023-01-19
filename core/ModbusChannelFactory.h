/*
 * ModbusChannelFactory.h
 *
 *  Created on: Aug 2, 2017
 *      Author: michelo
 */

#ifndef DRIVER_MODBUS_CHANNELFACTORY_H_
#define DRIVER_MODBUS_CHANNELFACTORY_H_
#include "AbstractModbus.h"
#include <boost/thread.hpp>
#include <common/misc/driver/ConfigDriverMacro.h>

#ifdef CHAOS

#include <chaos/common/exception/CException.h>
#include <chaos/common/data/CDataWrapper.h>

#endif
#include <stdexcept>
#include <map>
namespace common {
namespace modbus {

class ModbusChannelFactory {
	static std::map<std::string,AbstractModbusChannel_psh> unique_channels;
	static ChaosMutex chanmutex;
public:
	ModbusChannelFactory();
	virtual ~ModbusChannelFactory();
	// retrieve a Serial Channel
	static AbstractModbusChannel_psh getChannel(std::string serial_dev,int baudrate,int parity,int bits,int stop,bool hwctrl=false);
	//retrieve a tcp channel
	static AbstractModbusChannel_psh getChannel(const std::string& ip, int port );
#ifdef CHAOS
	static AbstractModbusChannel_psh getChannel(const chaos::common::data::CDataWrapper& config)  ;
#endif
	static AbstractModbusChannel_psh getChannelFromJson(const std::string& json)  ;
	static void removeChannel(const std::string& uid);
	static void removeChannel(AbstractModbusChannel_psh& ch);

};

} /* namespace misc */
} /* namespace common */

#endif /* DRIVER_CHANNELFACTORY_H_ */
