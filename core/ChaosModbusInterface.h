//
//  ChaosModbusInterface.h
//  modbus
//
//  Created by andrea michelotti on 03/09/14.
//  Copyright (c) 2014 andrea michelotti. All rights reserved.
//

#ifndef __modbus__ChaosModbusInterface__
#define __modbus__ChaosModbusInterface__

#include <iostream>
#include "AbstractModbus.h"
#include <chaos/cu_toolkit/driver_manager/driver/DriverTypes.h>
#include <chaos/cu_toolkit/driver_manager/driver/DriverAccessor.h>

namespace chaos_driver=::chaos::cu::driver_manager::driver;

namespace common {
    namespace modbus {
        typedef enum {
            OP_INIT = chaos_driver::OpcodeType::OP_USER, // init low level driver
            OP_DEINIT, // deinit low level driver
            
            OP_CONNECT, // connect to a TCP modbus master
            OP_FLUSH, //clean
            OP_CLOSE, // close a connection
            OP_SET_WRITE_TIMEO, // set Write timeout among two subseguent bytes
            OP_SET_READ_TIMEO, // set Read timeout among two subseguent bytes

            OP_GET_WRITE_TIMEO, // return the write timeoout
            OP_GET_READ_TIMEO, // return read timeout
            OP_READ_COIL_STATUS,  // read byte
            OP_READ_INPUT_STATUS, //read byte
            OP_READ_HOLDING_REGISTERS,
            OP_READ_INPUT_REGISTERS,
            OP_REPORT_SLAVE,
            OP_FORCE_SINGLE_COIL,
            OP_PRESET_SINGLE_REGISTER,
            OP_FORCE_MULTIPLE_COILS,
            OP_FORCE_MULTIPLE_REGISTERS,
            OP_WRITE_AND_READ_REGISTERS,
        } ChaosModbusOpcode;
        
        typedef struct {
            union {
                int slaveid;
                int param0;
            };
            
            int regadd;
            int sizeb;
            
            int regadd2;
            int sizeb2;
            // in write operations data follow
        } modbus_iparams_t;
        
        typedef struct {
            int ret;
            // in read operation data follow
            
        } modbus_oparams_t;
        
        class ChaosModbusInterface:public common::modbus::AbstractModbus {
            
        protected:
            chaos_driver::DrvMsg message;
            
        public:
            
            ChaosModbusInterface(chaos_driver::DriverAccessor*_accessor):accessor(_accessor){};
            
            chaos_driver::DriverAccessor* accessor;
            
            
            
            /**
             connect to a target modbus device
             @return true on success
             */
            bool connect(void);
            
            /**
             The flush() function shall discard data received but not read to the socket or file descriptor associated.
             */
            int flush(void);
            
            /**
             close a previously open connection
             */
            int close(void);
            
            /**
             set_write_timeo
             
             sets the timeout between the write of two consecutive bytes
             @param us timeout
             */
            void set_write_timeo(uint32_t us);
            
            
            /**
             set_read_timeo
             
             sets the timeout waiting an answer
             @param us timeout
             */
            void set_read_timeo(uint32_t us);
            
            /**
             get_write_timeo
             
             gets the timeout between the write of two consecutive bytes
             @return the us timeout
             */
            uint32_t get_write_timeo();
            
            
            /**
             get_read_timeo
             
             gets the timeout waiting an answer
             @return us timeout
             */
            uint32_t get_read_timeo();
            
            
            /**
             The read_coil_status, reads the status of the nb bits (coils) to the address addr of the remote device. The result of reading is stored in dest array as unsigned bytes (8 bits) set to TRUE or FALSE
             read  the status bits of the nb bits (coils), function (0x2( read input status
             You must take care to allocate enough memory to store the results in dest (at least nb * sizeof(uint8_t)).
             The function uses the Modbus function code 0x01 (read coil status).
             
             @param addr address of the remote device
             @param nb number of bits to read
             @param dest destination buffer that contains TRUE or FALSE
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return negative if error
             */
            int read_coil_status(int addr,int nb, uint8_t*dest,int slave_id=-1);
            
            /**
             The read_input_status() function shall read the content of the nb input bits to the address addr of the remote device. The result of reading is stored in dest array as unsigned bytes (8 bits) set to TRUE or FALSE.
             
             You must take care to allocate enough memory to store the results in dest (at least nb * sizeof(uint8_t)).
             The function uses the Modbus function code 0x02 (read input status).
             
             @param addr address of the remote device
             @param nb number of bits to read
             @param dest destination buffer that contains TRUE or FALSE
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return negative if error
             */
            int read_input_status(int addr,int nb, uint8_t*dest,int slave_id=-1);
            /**
             
             The read_holding_registers() function shall read the content of the nb holding registers to the address addr of the remote device. The result of reading is stored in dest array as word values (16 bits).
             You must take care to allocate enough memory to store the results in dest (at least nb * sizeof(uint16_t)).
             The function uses the Modbus function code 0x03 (read holding registers).
             
             @param addr address of the remote device
             @param nb number of registers to read
             @param dest destination buffer that contains
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return number of registers sucessfully read, negative if error
             */
            int read_holding_registers(int addr,int nb, uint16_t*dest,int slave_id=-1);
            /**
             The read_input_registers() function shall read the content of the nb input registers to address addr of the remote device. The result of the reading is stored in dest array as word values (16 bits).
             
             You must take care to allocate enough memory to store the results in dest (at least nb * sizeof(uint16_t)).
             
             The function uses the Modbus function code 0x04 (read input registers). The holding registers and input registers have different historical meaning, but nowadays it’s more common to use holding registers only.
             
             @param addr address of the remote device
             @param nb number of registers to read
             @param dest destination buffer
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return number of registers sucessfully read, negative if error
             
             */
            int read_input_registers(int addr,int nb, uint16_t*dest,int slave_id=-1);
            
            
            /**
             The report_slave() function shall send a request to the controller to obtain a description of the controller.
             
             the response stored in dest contains a report_slave_header_t structure plu additional data:
             - the byte count of the response
             - the slave ID, this unique ID is in reality not unique at all so it’s not possible to depend on it to know how the information are packed in the response.
             
             - the run indicator status (0x00 = OFF, 0xFF = ON)
             
             additional data specific to each controller. For example, libmodbus returns the version of the library as a string.
             
             @param desc response of the slave
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return number of byte read, negative if error
             */
            int report_slave(uint8_t*desc,int slave_id=-1);
            
            
            
            
            /**
             The force_single_coil() function shall write the status of status at the address addr of the remote device. The value must be set to TRUE or FALSE.
             
             The function uses the Modbus function code 0x05 (force single coil).
             
             @param addr address of the remote device
             @param status (1/0)
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return 1 if success,negative if error
             */
            int force_single_coil(int addr,int status,int slave_id=-1);
            
            /**
             The preset_single_register() function shall write the value of value holding registers at the address addr of the remote device.
             
             The function uses the Modbus function code 0x06 (preset single register).
             
             @param addr address of the remote device
             @param value of the register
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return 1 if success,negative if error
             */
            int preset_single_register(int addr,int value,int slave_id=-1);
            
            
            /**
             The force_multiple_coils() function shall write the status of the nb bits (coils) from src at the address addr of the remote device. The src array must contains bytes set to TRUE or FALSE.
             
             The function uses the Modbus function code 0x0F (force multiple coils).
             
             @param addr address of the remote device
             @param nb number of bits to write
             @param src source buffer
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return the number of sucessfully written bits, negative if error
             */
            int force_multiple_coils(int addr,int nb,uint8_t*src,int slave_id=-1);
            
            
            /**
             The preset_multiple_registers() function shall write the content of the nb holding registers from the array src at address addr of the remote device.
             
             The function uses the Modbus function code 0x10 (preset multiple registers).
             
             @param addr address of the remote device
             @param nb number of registers to write
             @param src source buffer
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return the number of sucessfully written registers, negative if error
             */
            int force_multiple_registers(int addr,int nb,uint16_t*src,int slave_id=-1);
            
            /**
             The write_and_read_registers() function shall write the content of the write_nb holding registers from the array src to the address write_addr of the remote device then shall read the content of the read_nb holding registers to the address read_addr of the remote device. The result of reading is stored in dest array as word values (16 bits).
             
             You must take care to allocate enough memory to store the results in dest (at least nb * sizeof(uint16_t)).
             
             The function uses the Modbus function code 0x17 (write/read registers).
             
             @param waddr address of the remote device to write into
             @param wnb number of registers to write
             @param src source buffer
             @param raddr address of the remote device to read from
             @param rnb number of registers to read
             @param dest destination buffer
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return the number of sucessfully read registers, negative if error
             */
            
            int write_and_read_registers(int waddr,int wnb,uint16_t*src,int raddr,int rnb,uint16_t*dest,int slave_id=-1);
            
        };

    };
};


#endif /* defined(__modbus__ChaosModbusInterface__) */
