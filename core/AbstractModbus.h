//
//  AbstractModbus.h
//  CHAOS_SWPowerSupplyModbusDD
//
//  Created by andrea michelotti on 9/3/13.
//
// base class for modbus clients 

#ifndef __CHAOS_SWPowerSupplyModbusDD__AbstractModbus__
#define __CHAOS_SWPowerSupplyModbusDD__AbstractModbus__

#include <iostream>
#include <stdint.h>
namespace common {
    namespace modbus {
        
        typedef struct {
            uint8_t count; // byte count of the answer
            uint8_t uid; // unique slave ID
            uint8_t status; // run state (0 = OFF, 0xFF =ON)
            // follow additional data specific to each controller
        } report_slave_header_t;
        
        class AbstractModbus {
            
        public:
            AbstractModbus(){};
            virtual ~AbstractModbus(){};
            /**
             connect to a target modbus device
             @return true on success
             */
            virtual bool connect(void)=0;
            
            /**
             The flush() function shall discard data received but not read to the socket or file descriptor associated.
             */
            virtual int flush(void)=0;
            
            /**
                close a previously open connection
             */
            virtual int close(void)=0;

            /**
             set_write_timeo
             
             sets the timeout between the write of two consecutive bytes
             @param us timeout
             */
            virtual void set_write_timeo(uint32_t us)=0;
            
            
            /**
             set_read_timeo
             
             sets the timeout waiting an answer
             @param us timeout
             */
            virtual void set_read_timeo(uint32_t us)=0;
            
            /**
             get_write_timeo
             
             gets the timeout between the write of two consecutive bytes
             @return the us timeout
             */
            virtual uint32_t get_write_timeo()=0;
            
            
            /**
             get_read_timeo
             
             gets the timeout waiting an answer
             @return us timeout
             */
            virtual uint32_t get_read_timeo()=0;
            
            
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
            virtual int read_coil_status(int addr,int nb, uint8_t*dest,int slave_id=-1)=0;
            
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
            virtual int read_input_status(int addr,int nb, uint8_t*dest,int slave_id=-1)=0;
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
            virtual int read_holding_registers(int addr,int nb, uint16_t*dest,int slave_id=-1)=0;
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
            virtual int read_input_registers(int addr,int nb, uint16_t*dest,int slave_id=-1)=0;

            
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
            virtual int report_slave(uint8_t*desc,int slave_id=-1)=0;
            
            
            
            
            /**
             The force_single_coil() function shall write the status of status at the address addr of the remote device. The value must be set to TRUE or FALSE.
             
             The function uses the Modbus function code 0x05 (force single coil).
             
             @param addr address of the remote device
             @param status (1/0)
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return 1 if success,negative if error
             */
            virtual int force_single_coil(int addr,int status,int slave_id=-1)=0;
            
            /**
             The preset_single_register() function shall write the value of value holding registers at the address addr of the remote device.
             
             The function uses the Modbus function code 0x06 (preset single register).             
             
             @param addr address of the remote device
             @param value of the register
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return 1 if success,negative if error
             */
            virtual int preset_single_register(int addr,int value,int slave_id=-1)=0;
            
            
            /**
             The force_multiple_coils() function shall write the status of the nb bits (coils) from src at the address addr of the remote device. The src array must contains bytes set to TRUE or FALSE.
             
             The function uses the Modbus function code 0x0F (force multiple coils).
             
             @param addr address of the remote device
             @param nb number of bits to write
             @param src source buffer
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return the number of sucessfully written bits, negative if error
             */
            virtual int force_multiple_coils(int addr,int nb,uint8_t*src,int slave_id=-1)=0;
            
            
            /**
             The preset_multiple_registers() function shall write the content of the nb holding registers from the array src at address addr of the remote device.
             
             The function uses the Modbus function code 0x10 (preset multiple registers).
             
             @param addr address of the remote device
             @param nb number of registers to write
             @param src source buffer
             @param slave_id in RTU mode defines the slave device 0=broadcast
             @return the number of sucessfully written registers, negative if error
             */
            virtual int force_multiple_registers(int addr,int nb,uint16_t*src,int slave_id=-1)=0;

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

            virtual int write_and_read_registers(int waddr,int wnb,uint16_t*src,int raddr,int rnb,uint16_t*dest,int slave_id=-1)=0;

            

        };
    };
};
#endif /* defined(__CHAOS_SWPowerSupplyModbusDD__AbstractModbus__) */
