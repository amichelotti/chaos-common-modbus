/*
 * ModbusChannelFactory.cpp
 *
 *  Created on: Aug 2, 2017
 *      Author: michelo
 */

#include "ModbusChannelFactory.h"
#include "LibModBusWrap.h"
#include <common/debug/core/debug.h>


namespace common {
namespace modbus {

std::map<std::string,AbstractModbusChannel_psh> ModbusChannelFactory::unique_channels;
boost::mutex ModbusChannelFactory::chanmutex;
#ifdef CHAOS
using namespace chaos::common::data;
AbstractModbusChannel_psh ModbusChannelFactory::getChannelFromJson(const std::string& json)  throw (std::logic_error){
	try{
		chaos::common::data::CDataWrapper data;
		data.setSerializedJsonData(json.c_str());
		return ModbusChannelFactory::getChannel(data);
	} catch (...){
		throw std::logic_error("bad json");
	}
}
AbstractModbusChannel_psh ModbusChannelFactory::getChannel(const chaos::common::data::CDataWrapper& json )  throw(chaos::CException) {
	AbstractModbusChannel_psh ret;
	GET_PARAMETER_TREE((&json),channel){
		GET_PARAMETER_DO(channel,serdev,string,0){
			//serial channel
			GET_PARAMETER(channel,baudrate,int32_t,1);
			GET_PARAMETER(channel,parity,int32_t,1);
			GET_PARAMETER(channel,stop,int32_t,1);
			GET_PARAMETER(channel,hwctrl,int32_t,1);
			GET_PARAMETER(channel,bits,int32_t,1);
			return getChannel(serdev,baudrate,parity,bits,stop,hwctrl);

		}
		GET_PARAMETER_DO(channel,tcp,string,0){
			GET_PARAMETER(channel,port,int32_t,1);

			return getChannel(tcp,port);

		}
	}
	return ret;
}
#else
AbstractModbusChannel_psh ModbusChannelFactory::getChannelFromJson(const std::string& json)  throw (std::logic_error){
	throw std::logic_error("not implemented");

}

#endif

AbstractModbusChannel_psh ModbusChannelFactory::getChannel(std::string serial_dev,int baudrate,int parity,int bits,int stop,bool hwctrl){
	boost::mutex::scoped_lock l(chanmutex);
	std::map<std::string,AbstractModbusChannel_psh>::iterator i=unique_channels.find(serial_dev);
	if(i!=unique_channels.end()){
		DPRINT("retrieving SERIAL channel '%s' @%p in use count %ld",serial_dev.c_str(),i->second.get(),i->second.use_count());
		return i->second;
	}
	LibModBusWrap* ptr=new LibModBusWrap(serial_dev.c_str(),baudrate,parity,bits,stop);
	AbstractModbusChannel_psh ret(ptr);
	unique_channels[serial_dev]=ret;
	DPRINT("creating Modbus SERIAL channel '%s' @%p",serial_dev.c_str(),ret.get());

	return ret;

}
AbstractModbusChannel_psh ModbusChannelFactory::getChannel(const std::string& ip, int port ){
	AbstractModbusChannel_psh p;
	std::stringstream ss;
	ss<<ip<<":"<<port;

	boost::mutex::scoped_lock l(chanmutex);
	std::map<std::string,AbstractModbusChannel_psh>::iterator i=unique_channels.find(ss.str());
	if(i!=unique_channels.end()){
		DPRINT("retrieving TCP channel '%s' @%p in use count %ld",ss.str().c_str(),i->second.get(),i->second.use_count());
		return i->second;
	}
	DPRINT("creating TCP channel '%s' @%p",ss.str().c_str(),p.get());
	LibModBusWrap* ptr=new LibModBusWrap(ss.str());
	AbstractModbusChannel_psh ret(ptr);
	unique_channels[ss.str()]=ret;
	return ret;
}

void ModbusChannelFactory::removeChannel(const std::string& uid){
	boost::mutex::scoped_lock l(chanmutex);

	std::map<std::string,AbstractModbusChannel_psh>::iterator i=unique_channels.find(uid);
	if(i!=unique_channels.end()){
		if(i->second.use_count()==1){
			DPRINT("REMOVING CHANNEL '%s' @%p in use %ld",uid.c_str(),i->second.get(),i->second.use_count());
			unique_channels.erase(i);
		} else {
			DPRINT("CANNOT REMOVE CHANNEL '%s' @%p in use %ld",uid.c_str(),i->second.get(),i->second.use_count());
		}
	}
}
void ModbusChannelFactory::removeChannel(AbstractModbusChannel_psh& ch){
	if(ch.get()==0){
		DPRINT("CHANNEL REMOVED in use %ld",ch.use_count());
		return;
	}
	std::string uid=ch->getUid();

	DPRINT("ATTEMPT TO REMOVE CHANNEL'%s' @%p in use in %ld places",uid.c_str(),ch.get(),ch.use_count());
	ch.reset();

	removeChannel(uid);
}

ModbusChannelFactory::ModbusChannelFactory() {
	// TODO Auto-generated constructor stub

}

ModbusChannelFactory::~ModbusChannelFactory() {
	// TODO Auto-generated destructor stub
}

} /* namespace misc */
} /* namespace common */
