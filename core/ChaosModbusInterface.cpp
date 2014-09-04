//
//  ChaosModbusInterface.cpp
//  modbus
//
//  Created by andrea michelotti on 03/09/14.
//  Copyright (c) 2014 andrea michelotti. All rights reserved.
//

#include "ChaosModbusInterface.h"
#include <chaos/common/exception/CException.h>


#define PREPARE_WRITE_OP(op,add,_slaveid,_buf,_sizeb) \
modbus_oparams_t ret; \
char*buff = (char*)malloc(sizeof(modbus_iparams_t)+_sizeb);\
if(buff==NULL) {throw chaos::CException(1, "cannot allocate memory for driver", "ChaosModbusInterface::write");}\
modbus_iparams_t *idata=(modbus_iparams_t *)buff;\
idata->regadd = add;idata->slaveid=_slaveid;idata->sizeb=_sizeb;\
memcpy(idata+1,_buf,_sizeb);\
message.opcode = op; \
message.inputData=(void*)idata;\
message.inputDataLength=sizeof(modbus_iparams_t)+_sizeb;\
message.resultDataLength=sizeof(modbus_oparams_t);\
message.resultData = (void*)&ret;\
accessor->send(&message);\
free(buff);

#define PREPARE_WRITE_READ_OP(op,add,_slaveid,_buf,_sizeb,radd,_rbuf,_rsizeb) \
modbus_oparams_t ret; \
char*buff = (char*)malloc(sizeof(modbus_iparams_t)+_sizeb);\
if(buff==NULL) {throw chaos::CException(1, "cannot allocate memory for driver", "ChaosModbusInterface::write");}\
char*rbuff = (char*)malloc(sizeof(modbus_oparams_t)+_rsizeb);\
if(rbuff==NULL) {throw chaos::CException(1, "cannot allocate memory for driver", "ChaosModbusInterface::read");}\
modbus_iparams_t *idata=(modbus_iparams_t *)buff;\
idata->regadd = add;idata->slaveid=_slaveid;idata->sizeb=_sizeb;\
idata->regadd2 = radd;idata->sizeb2 = _rsizeb;\
modbus_oparams_t* pret=(modbus_oparams_t*)rbuff;\
memcpy(idata+1,_buf,_sizeb);\
message.opcode = op; \
message.inputData=(void*)idata;\
message.inputDataLength=sizeof(modbus_iparams_t)+_sizeb;\
message.resultDataLength=sizeof(modbus_oparams_t);\
message.resultData = (void*)&ret;\
accessor->send(&message);\
memcpy(_rbuf,(char*)message.resultData+sizeof(modbus_oparams_t),_rsizeb);\
ret.ret = pret->ret;\
free(buff);\
free(rbuff);


#define RETURN_INT return ret.ret;
#define RETURN_BOOL return ret.ret?true:false;

#define PREPARE_READ_OP(op,add,_slaveid,buf,_sizeb) \
modbus_oparams_t ret;\
 char*buff = (char*)malloc(sizeof(modbus_oparams_t)+_sizeb);\
if(buff==NULL) {throw chaos::CException(1, "cannot allocate memory for driver", "ChaosModbusInterface::read");}\
modbus_iparams_t idata;\
idata.regadd = add;idata.slaveid=_slaveid;idata.sizeb=_sizeb;\
modbus_oparams_t* pret=(modbus_oparams_t*)buff;\
message.opcode = op; \
message.inputData=(void*)&idata;\
message.inputDataLength=sizeof(modbus_iparams_t);\
message.resultDataLength=sizeof(modbus_oparams_t)+_sizeb;\
message.resultData = (void*)pret;\
accessor->send(&message);\
memcpy(buf,(char*)message.resultData+sizeof(modbus_oparams_t),_sizeb);\
ret.ret = pret->ret;\
free(buff);


#define PREPARE_CALL0(op) PREPARE_WRITE_OP(op,0,0,0,0);
#define PREPARE_CALL1(op,data) PREPARE_WRITE_OP(op,data,0,0,0);

using namespace common::modbus;

bool ChaosModbusInterface::connect(void){
    PREPARE_CALL0(OP_CONNECT);
    RETURN_BOOL;
    
}

int ChaosModbusInterface::flush(void){
    PREPARE_CALL0(OP_FLUSH);
    RETURN_INT;

}

int ChaosModbusInterface::close(void){
    PREPARE_CALL0(OP_CLOSE);
    RETURN_INT;

}

void ChaosModbusInterface::set_write_timeo(uint32_t us){
    PREPARE_CALL1(OP_SET_WRITE_TIMEO,us);

}


void ChaosModbusInterface::set_read_timeo(uint32_t us){
    PREPARE_CALL1(OP_SET_READ_TIMEO,us);

}
uint32_t ChaosModbusInterface::get_write_timeo(){
    PREPARE_CALL0(OP_GET_WRITE_TIMEO);
    RETURN_INT;
    
}

uint32_t ChaosModbusInterface::get_read_timeo(){
    PREPARE_CALL0(OP_GET_READ_TIMEO);
    RETURN_INT;

}


int ChaosModbusInterface::ChaosModbusInterface::read_coil_status(int addr,int nb, uint8_t*dest,int slave_id){
    PREPARE_READ_OP(OP_READ_COIL_STATUS,addr,slave_id,dest,nb);
    RETURN_INT;
}

int ChaosModbusInterface::ChaosModbusInterface::read_input_status(int addr,int nb, uint8_t*dest,int slave_id){
    PREPARE_READ_OP(OP_READ_INPUT_STATUS,addr,slave_id,dest,nb);
    RETURN_INT;
}

int ChaosModbusInterface::read_holding_registers(int addr,int nb, uint16_t*dest,int slave_id){
    PREPARE_READ_OP(OP_READ_HOLDING_REGISTERS,addr,slave_id,dest,nb*sizeof(uint16_t));
    RETURN_INT;
}

int ChaosModbusInterface::read_input_registers(int addr,int nb, uint16_t*dest,int slave_id){
    PREPARE_READ_OP(OP_READ_INPUT_REGISTERS,addr,slave_id,dest,nb*sizeof(uint16_t));
    RETURN_INT;
}



int ChaosModbusInterface::report_slave(uint8_t*desc,int slave_id){
    PREPARE_READ_OP(OP_REPORT_SLAVE,0,slave_id,desc,0);
    RETURN_INT;
}

int ChaosModbusInterface::force_single_coil(int addr,int status,int slave_id){
    PREPARE_WRITE_OP(OP_FORCE_SINGLE_COIL,addr,slave_id,&status,1);
    RETURN_INT;

}

int ChaosModbusInterface::preset_single_register(int addr,int value,int slave_id){
    PREPARE_WRITE_OP(OP_PRESET_SINGLE_REGISTER,addr,slave_id,&value,2);
    RETURN_INT;
}


int ChaosModbusInterface::force_multiple_coils(int addr,int nb,uint8_t*src,int slave_id){
    PREPARE_WRITE_OP(OP_FORCE_MULTIPLE_COILS,addr,slave_id,src,nb);
    RETURN_INT;
}

int ChaosModbusInterface::force_multiple_registers(int addr,int nb,uint16_t*src,int slave_id){
    PREPARE_WRITE_OP(OP_FORCE_MULTIPLE_REGISTERS,addr,slave_id,src,nb*sizeof(uint16_t));
    RETURN_INT;
}


int ChaosModbusInterface::write_and_read_registers(int waddr,int wnb,uint16_t*src,int raddr,int rnb,uint16_t*dest,int slave_id){
    PREPARE_WRITE_READ_OP(OP_WRITE_AND_READ_REGISTERS,waddr,slave_id,src,wnb*sizeof(uint16_t),raddr,dest,rnb*sizeof(uint16_t));
    RETURN_INT;
    
}
