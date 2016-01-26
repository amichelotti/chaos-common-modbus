#include <cstring>
#include <vector>
#include <malloc.h>

#ifdef HAZEMEYER_DEBUG
#define DEBUG
#endif
#include <common/modbus/ModBus.h>
using namespace std;
extern "C"
{
int HZTest();
};

namespace Hazemeyer
{
    namespace ConnectStatus
    {
        enum  ConnectStatus  
        {   UNDEFINED=-1,
            UNCONNECTED=0,
            CONNECTED=1 
        };
        
        
    }
    
    class Corrector
    {
        
        
    public:
        enum Conversions
        {
            CONV_MAIN_VOLT=0,
            CONV_MAIN_AMP=1,
            CONV_MAIN_KW=2,
            CONV_TOTAL_KW=3,
            CONV_EARTH_FAULT_CURR=4,
            CONV_CHAN_VOLT=5,
            CONV_CHAN_AMP=6
        };
        enum Commands
        {
            CHANNEL_ON=1,
            CHANNEL_OFF=2,
            CHANNEL_RESET=4
        };
        enum WriteReg
        {
            GENERAL_COMMANDS=0,
            CH0_COMMANDS=0x10,
            CH1_COMMANDS=0x12,
            CH2_COMMANDS=0x14,
            CH3_COMMANDS=0x16,
            CH4_COMMANDS=0x18,
            CH5_COMMANDS=0x1A,
            CH6_COMMANDS=0x1C,
            CH7_COMMANDS=0x1E,
            CH0_SETCURR =0x60,
            CH1_SETCURR =0x61,
            CH2_SETCURR =0x62,
            CH3_SETCURR =0x63,
            CH4_SETCURR =0x64,
            CH5_SETCURR =0x65,
            CH6_SETCURR =0x66,
            CH7_SETCURR =0x67
        };
        enum ReadReg
        {
            GENERAL_FAULTS=0x20,
            GENERAL_STATUS=0x21,
            CH0_FAULTS=0x30,
            CH0_POWER_STATUS=0x31,
            CH1_FAULTS=0x32,
            CH1_POWER_STATUS=0x33,
            CH2_FAULTS=0x34,
            CH2_POWER_STATUS=0x35,
            CH3_FAULTS=0x36,
            CH3_POWER_STATUS=0x37,
            CH4_FAULTS=0x38,
            CH4_POWER_STATUS=0x39,
            CH5_FAULTS=0x3A,
            CH5_POWER_STATUS=0x3B,
            CH6_FAULTS=0x3C,
            CH6_POWER_STATUS=0x3D,
            CH7_FAULTS=0x3E,
            CH7_POWER_STATUS=0x3F,
            MAIN_1PHASE_V=0x80,
            MAIN_2PHASE_V=0x81,
            MAIN_3PHASE_V=0x82,
            MAIN_AVERAGE_V=0x83,
            MAIN_1PHASE_I=0x84,
            MAIN_2PHASE_I=0x85,
            MAIN_3PHASE_I=0x86,
            MAIN_AVERAGE_I=0x87,
            MAIN_1_ACTIVE_W=0x88,
            MAIN_2_ACTIVE_W=0x89,
            MAIN_3_ACTIVE_W=0x8A,
            MAIN_TOTAL_ACTIVE_W=0x8B,
            MAIN_1_REACTIVE_W=0x8C,
            MAIN_2_REACTIVE_W=0x8D,
            MAIN_3_REACTIVE_W=0x8E,
            MAIN_TOTAL_REACTIVE_W=0x8F,
            MAIN_1_APPARENT_W=0x90,
            MAIN_2_APPARENT_W=0x91,
            MAIN_3_APPARENT_W=0x92,
            MAIN_TOTAL_APPARENT_W=0x93,
            MAIN_1_POWER_FACTOR=0x94,
            MAIN_2_POWER_FACTOR=0x95,
            MAIN_3_POWER_FACTOR=0x96,
            MAIN_AVERAGE_POWER_FACTOR=0x97,
            EARTH_FAULT_CURRENT=0x9C,
            MAIN_FREQUENCY=0x9D,
            
            CH0_CURRENT=0xA0,
            CH0_VOLTAGE=0xA1,
            CH1_CURRENT=0xA4,
            CH1_VOLTAGE=0xA5,
            CH2_CURRENT=0xA8,
            CH2_VOLTAGE=0xA9,
            CH3_CURRENT=0xAC,
            CH3_VOLTAGE=0xAD,
            CH4_CURRENT=0xB0,
            CH4_VOLTAGE=0xB1,
            CH5_CURRENT=0xB4,
            CH5_VOLTAGE=0xB5,
            CH6_CURRENT=0xB8,
            CH6_VOLTAGE=0xB9,
            CH7_CURRENT=0xBC,
            CH7_VOLTAGE=0xBD,
            CHS_READY=0xC2,
            CHS_POWER=0xC3,
            
            CH0_ALIAS_CURRENT=0xC4,
            CH1_ALIAS_CURRENT=0xC5,
            CH2_ALIAS_CURRENT=0xC6,
            CH3_ALIAS_CURRENT=0xC7,
            CH4_ALIAS_CURRENT=0xC8,
            CH5_ALIAS_CURRENT=0xC9,
            CH6_ALIAS_CURRENT=0xCA,
            CH7_ALIAS_CURRENT=0xCB
        };
        //MANCANO LE LETTURE DOPPIE
        
        
        
        
        
        Corrector();
        Corrector(const char* SerialParameters);
        ~Corrector();
        bool Connect();
        bool CloseConnection();
        void ScreenMenu();
        bool setModbusWriteTimeout(uint32_t usec) {this->modbus_drv->set_write_timeo(usec);}
        bool setModbusReadTimeout(uint32_t usec) {this->modbus_drv->set_read_timeo(usec);}
        Hazemeyer::ConnectStatus::ConnectStatus getConnectionStatus();
        bool ReadBitRegister(Hazemeyer::Corrector::ReadReg address,  int16_t* data);
        bool TurnOnMainUnit() {
                    return this->WriteRegister(Corrector::GENERAL_COMMANDS,0x2);
                    };
        bool TurnOffMainUnit(){
                    return this->WriteRegister(Corrector::GENERAL_COMMANDS,0x4);
                    };
        bool TurnStandbyMainUnit(){
                    return this->WriteRegister(Corrector::GENERAL_COMMANDS,0x1);
                    };
        bool ResetMainUnit() {
            return this->WriteRegister(Corrector::GENERAL_COMMANDS,0x8);             
        };
        bool SetChannelCurrent(unsigned int channel,double val);
        bool SendChannelCommand(unsigned int channel,Corrector::Commands com);
        bool ReadChannelCurrent(unsigned int channel, double *data);
        bool ReadChannelVoltage(unsigned int channel, double *data);
        double ConvertFromDigit(Corrector::Conversions mode,int digital);
        
    private:
        
        std::string SerialDev;
        int baudRate;
        char parity;
        uint16_t bits;
        uint16_t stopBits;
        Hazemeyer::ConnectStatus::ConnectStatus connectionStatus;
        ::common::modbus::AbstractModbus* modbus_drv=NULL;
        int slave;
        bool WriteRegister(Hazemeyer::Corrector::WriteReg address, int16_t data);
        void ScreenSetChanCurrent();
        void ScreenMainCommands();
        void ScreenChannelCommands();
        void ScreenReadSingleRegister();
    };
};




