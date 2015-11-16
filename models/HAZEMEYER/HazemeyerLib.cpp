
#include "HazemeyerLib.h"
using namespace Hazemeyer;
extern "C"
{
    int HZTest()
    {
      printf("Ciao questo è un test\n");
      return 0;
    };
}


Corrector::Corrector( char* SerialParameters){
    //la stringa viene scritta con ogni parametro separato da virgola
    char* tmp=NULL;
    char lwPar;
    int parsingStatus=1;
    tmp=strtok(SerialParameters,",");
    //cout << tmp << endl; //serialdev
    //checking serial dev as a string
    if (strlen(tmp) )
        this->SerialDev= tmp;
    tmp= strtok(NULL,",");
    //cout << tmp << endl;    //slave
    this->slave=atoi(tmp);
    tmp= strtok(NULL,",");
    //cout << tmp << endl;    //baudRate
    this->baudRate=atoi(tmp);
    tmp= strtok(NULL,",");
    //cout << tmp << endl;    //parity
    this->parity=tmp[0];
    
    tmp= strtok(NULL,",");
    //cout << tmp << endl;    //bits
    this->bits=atoi(tmp);
    tmp= strtok(NULL,",");
    //cout << tmp << endl;    //stop
    this->stopBits=atoi(tmp);
    lwPar=tolower(this->parity);
    //check consistence.
    
    
    this->modbus_drv= new common::modbus::ModBusRTU(this->SerialDev.c_str(),this->baudRate,this->parity,
            this->bits,this->stopBits);
    
    
     if(this->modbus_drv==NULL)
     {
        cerr<< "## invalid parameters:"<<endl;
        this->connectionStatus=Hazemeyer::ConnectStatus::UNDEFINED;
     }
     else
         this->connectionStatus=Hazemeyer::ConnectStatus::UNCONNECTED;
    
    
    
    
};
Corrector::Corrector(){
    //this->Name="KKKK";
    this->slave=2;
    this->modbus_drv= new common::modbus::ModBusRTU("/dev/ttyr0f",9600,'N',8,1);
     if(this->modbus_drv==NULL)
     {
        cerr<< "## invalid parameters:"<<endl;
        this->connectionStatus=Hazemeyer::ConnectStatus::UNDEFINED;
     }
     else
         this->connectionStatus=Hazemeyer::ConnectStatus::UNCONNECTED;
  

};

Corrector::~Corrector(){
    delete (this->modbus_drv);

};
bool Corrector::Connect(){
    if (this->connectionStatus != Hazemeyer::ConnectStatus::UNCONNECTED)
        return false;
    if(this->modbus_drv->connect()!=true)
    {
        cerr<< "## cannot connect to slave"<<endl;
        return false;
    }
    this->connectionStatus = Hazemeyer::ConnectStatus::CONNECTED;
    return true;
}  
bool Corrector::CloseConnection(){
    if (this->connectionStatus != Hazemeyer::ConnectStatus::CONNECTED)
        return false;
    bool ret = !(this->modbus_drv->close() );
    if (ret)
        this->connectionStatus= Hazemeyer::ConnectStatus::UNCONNECTED;
    return ret;
}
ConnectStatus::ConnectStatus Hazemeyer::Corrector::getConnectionStatus() { 
    return this->connectionStatus;
}
bool Corrector::ReadBitRegister(Corrector::ReadReg address, short int* cont) {
    int err;
    uint16_t* data;
    //data=0;
    
    if (this->connectionStatus != Hazemeyer::ConnectStatus::CONNECTED)
    {
        cerr << "The connection is not established. Nothing to read" << endl;
        return false;
    }
    if (this->slave < 0)
    {
        cerr << "The slave parameter is not set. Nothing to read" << endl;
        return false;
    }
    data= (uint16_t*)malloc(sizeof(uint16_t));
    err = this->modbus_drv->read_input_registers(address,1,(uint16_t*)data,this->slave);
    if (err <= 0) 
    {
            
        cout << "err " << err << "(" << *cont << ")" << endl;
        free(data);
        return false;
    }
    *cont = (*data) & 0x0000FFFF;       
    free(data); 
    return true;
}
 bool Corrector::SendChannelCommand(unsigned int channel,Corrector::Commands com) {
      Corrector::WriteReg chan;
     switch (channel)
     {
         case 0 :chan=Corrector::CH0_COMMANDS; break;
        case 1 : chan=Corrector::CH1_COMMANDS; break;
        case 2 : chan=Corrector::CH2_COMMANDS; break;
        case 3 : chan=Corrector::CH3_COMMANDS; break;
        case 4 : chan=Corrector::CH4_COMMANDS; break;
        case 5 : chan=Corrector::CH5_COMMANDS; break;
        case 6 : chan=Corrector::CH6_COMMANDS; break;
        case 7 : chan=Corrector::CH7_COMMANDS; break;
        default : cerr << "Invalid channel Number" << endl; return false;
     }
     if ( this->WriteRegister(chan,com) )
     {
         cout << "Command "<< com << " sent to channel " << std::hex << chan << endl;
         return true;
     }
     else return false;
 }

bool Corrector::SetChannelCurrent(unsigned int channel,double val){
     Corrector::WriteReg chan;
     short int convertedValue;
     convertedValue= (short int) (208.333*val);
     switch (channel)
     {
         case 0 :chan=Corrector::CH0_SETCURR; break;
        case 1 : chan=Corrector::CH1_SETCURR; break;
        case 2 : chan=Corrector::CH2_SETCURR; break;
        case 3 : chan=Corrector::CH3_SETCURR; break;
        case 4 : chan=Corrector::CH4_SETCURR; break;
        case 5 : chan=Corrector::CH5_SETCURR; break;
        case 6 : chan=Corrector::CH6_SETCURR; break;
        case 7 : chan=Corrector::CH7_SETCURR; break;
        default : cerr << "Invalid channel Number" << endl; return false;
     }
     if ( this->WriteRegister(chan,convertedValue) )
     {
       cout << "write current " << std::hex << convertedValue << endl; 
       return true;
     }
     else
        return false;
 }
void Corrector::ScreenMenu(){
    char input;
    if (this->connectionStatus != Hazemeyer::ConnectStatus::CONNECTED)
    {
        cout << "Fatal! No Hardware connected" << endl;
        return;
    }
    while (1)
    {
        char  cc[2];
        short int val;
        cout << endl << endl << endl << endl ;
        cout  << "1: Read channels current" << endl;
        cout  << "2: Read channels voltage" << endl;
        cout  << "3: Read main status" << endl;
        cout  << "4: Set Current Channel" << endl;
        cout  << "5: Main Commands" << endl;
        cout  << "6: Channel Commands" << endl;
        cout  << "7: Read Single Register" << endl;
        cout  << "9: Read Main Parameters" << endl;
        cout  << "others : Exit" << endl;
        cin >> input;
        cc[0]=input;
        cc[1]='\0';
        val=atoi(cc);
        
        switch (val)
        {
            bool ret;
            double dCont;
            case 1:  for (int i=0;i<8;i++)
                    {
                        ret=this->ReadChannelCurrent(i,&dCont);
                        if (ret)
                            cout << "read current " << i << "= " << dCont << endl; 
                        else
                            cout << "error reading" << endl; 
                    } break;
            case 2: for (int i=0;i<8;i++)
                    {
                        ret=this->ReadChannelVoltage(i,&dCont);
                        if (ret)
                            cout << "read voltage " << i << "= " << dCont << endl; 
                        else
                            cout << "error reading" << endl; 
                    } break;
            case 3: 
                    short int read;
                    ret= this->ReadBitRegister(Corrector::GENERAL_STATUS,&read) ;
                    if (ret)
                        cout << "read GENERAL_STATUS " << std::hex << read << endl; 
                    else
                         cout << "error reading" << endl; 
                    ret= this->ReadBitRegister(Corrector::GENERAL_FAULTS,&read) ;
                    if (ret)
                        cout << "read GENERAL_FAULTS " << std::hex << read << endl; 
                    else
                         cout << "error reading" << endl; 
                    break;
            case 4: this->ScreenSetChanCurrent(); break;
            case 5: this->ScreenMainCommands();break;
            case 6: this->ScreenChannelCommands();break;
            case 7: this->ScreenReadSingleRegister(); break;
            case 9: ret= this->ReadBitRegister(Corrector::MAIN_1PHASE_V,&read) ;
                    double ccc;
                    if (ret)
                    {
                        ccc=this->ConvertFromDigit(Corrector::CONV_MAIN_VOLT,read);
                        cout << "MAIN_1PHASE_V " <<  ccc << endl; 
                    }
                    else
                        cout << "error reading" << endl;
                    ret= this->ReadBitRegister(Corrector::MAIN_2PHASE_V,&read) ;
                    if (ret)
                    {
                        ccc=this->ConvertFromDigit(Corrector::CONV_MAIN_VOLT,read);
                        cout << "MAIN_2PHASE_V " <<  ccc << endl; 
                    }
                    else
                        cout << "error reading" << endl; 
                    ret= this->ReadBitRegister(Corrector::MAIN_3PHASE_V,&read) ;
                    if (ret)
                    {
                        ccc=this->ConvertFromDigit(Corrector::CONV_MAIN_VOLT,read);
                        cout << "MAIN_3PHASE_V " <<  ccc << endl; 
                    }
                    else
                        cout << "error reading" << endl; 
                    /******************************************/
                    ret= this->ReadBitRegister(Corrector::MAIN_1PHASE_I,&read) ;
                    if (ret)
                    {
                        ccc=this->ConvertFromDigit(Corrector::CONV_MAIN_AMP,read);
                        cout << "MAIN_1PHASE_I " <<  ccc << endl; 
                    }
                    else
                        cout << "error reading" << endl;
                    ret= this->ReadBitRegister(Corrector::MAIN_2PHASE_I,&read) ;
                    if (ret)
                    {
                        ccc=this->ConvertFromDigit(Corrector::CONV_MAIN_AMP,read);
                        cout << "MAIN_2PHASE_I " <<  ccc << endl; 
                    }
                    else
                        cout << "error reading" << endl; 
                    ret= this->ReadBitRegister(Corrector::MAIN_3PHASE_I,&read) ;
                    if (ret)
                    {
                        ccc=this->ConvertFromDigit(Corrector::CONV_MAIN_AMP,read);
                        cout << "MAIN_3PHASE_I " <<  ccc << endl; 
                    }
                    else
                        cout << "error reading" << endl; 
                    ret= this->ReadBitRegister(Corrector::MAIN_TOTAL_ACTIVE_W,&read) ;
                    if (ret)
                    {
                        ccc=this->ConvertFromDigit(Corrector::CONV_TOTAL_KW,read);
                        cout << "MAIN_TOTAL_ACTIVE_W " <<  ccc << endl; 
                    }
                    else
                        cout << "error reading" << endl; 
                    break;
                    
            default:  return;
        }
      cout << "Press any key to continue" << endl;
      scanf("%*c%*c");
    }
}
bool Corrector::ReadChannelCurrent(unsigned int channel, double *data){
    Corrector::ReadReg chan;
    short int readData;
    switch (channel)
    {
        case 0 : chan=Corrector::CH0_ALIAS_CURRENT; break;
        case 1 : chan=Corrector::CH1_ALIAS_CURRENT; break;
        case 2 : chan=Corrector::CH2_ALIAS_CURRENT; break;
        case 3 : chan=Corrector::CH3_ALIAS_CURRENT; break;
        case 4 : chan=Corrector::CH4_ALIAS_CURRENT; break;
        case 5 : chan=Corrector::CH5_ALIAS_CURRENT; break;
        case 6 : chan=Corrector::CH6_ALIAS_CURRENT; break;
        case 7 : chan=Corrector::CH7_ALIAS_CURRENT; break;
        default : cerr << "Invalid channel Number" << endl; return false;
    }
    if ( this->ReadBitRegister(chan,&readData))
    {
        cout << "read current " << std::hex << readData << endl; 
        *data = this->ConvertFromDigit(Corrector::CONV_CHAN_AMP,readData); 
        return true;
    }
    else
        return false;
}
bool Corrector::ReadChannelVoltage(unsigned int channel, double *data){
    Corrector::ReadReg chan;
    short int readData;
    switch (channel)
    {
        case 0 : chan=Corrector::CH0_VOLTAGE; break;
        case 1 : chan=Corrector::CH1_VOLTAGE; break;
        case 2 : chan=Corrector::CH2_VOLTAGE; break;
        case 3 : chan=Corrector::CH3_VOLTAGE; break;
        case 4 : chan=Corrector::CH4_VOLTAGE; break;
        case 5 : chan=Corrector::CH5_VOLTAGE; break;
        case 6 : chan=Corrector::CH6_VOLTAGE; break;
        case 7 : chan=Corrector::CH7_VOLTAGE; break;
        default : cerr << "Invalid channel Number" << endl; return false;
    }
    if ( this->ReadBitRegister(chan,&readData))
    {
        *data = this->ConvertFromDigit(Corrector::CONV_CHAN_VOLT,readData); 
        return true;
    }
    else
        return false;
};
double Corrector::ConvertFromDigit(Corrector::Conversions mode,int digital) {
      short int value;
     value= digital & 0x0000FFFF;
      switch (mode)
    {
        case CONV_MAIN_VOLT: /*MAIN VOLTAGE Vrms*/ return (value*0.311);
        case CONV_MAIN_AMP: /*MAIN CURRENT Arms*/ return (value*0.0025);
        case CONV_MAIN_KW: /*MAIN POWER KW*/  return (value*0.006223);
        case CONV_TOTAL_KW: /*MAIN TOTAL POWER KW*/  return (value*0.006223); 
        case CONV_EARTH_FAULT_CURR: /*EARTH FAULT CURRENT percent*/return (double) value;
        case CONV_CHAN_VOLT: /*CHAN VOLTAGE V*/ return value*0.001833;  
        case CONV_CHAN_AMP: /*CHAN CURRENT A*/ return value*0.00122;
        //case 7: /*SET CURRENT A*/ return digital*0.00488;
        default: return (double) digital;
    };
}
bool Corrector::WriteRegister(Corrector::WriteReg address, short int data) {
    int ret;
    if (this->connectionStatus != Hazemeyer::ConnectStatus::CONNECTED)
    {
        cerr << "The connection is not established. Write is impossible" << endl;
        return false;
    }
   // cout << "fake writing: on address " << std::hex << address << " data " << std::hex << data <<endl;
    //return true;
    ret=this->modbus_drv->preset_single_register(address,(int) data,this->slave);
    if (ret < 0)
        return false;
    return true;
}
void Corrector::ScreenSetChanCurrent(){
    short int channel;
    double val;
    do
    {
        channel=0;
        cout << "   which channel (1-8) ?" << endl;
        cin >> channel;
    }
    while  ((channel <= 0) || (channel >8));
    channel--;
    do
    {
        cout << "   Ampere ?" << endl;
        cin >> val;
    }
    while ((val < -10) || (val > 10));
    this->SetChannelCurrent(channel,val);
}

void Corrector::ScreenReadSingleRegister() {
    unsigned short reg;
    int count=0;
    time_t now, bef;
    bool loop=false;
    short int data;
    cout << "Enter Register address in hex" << endl;
    cin >> std::hex >> reg;
    cout << "Unlimited Loop on?" << endl;
    cin >> loop;
    cout << std::hex << loop << endl;
    
    cout << "Read at address " << reg;
    do
    {
        if (this->ReadBitRegister((Corrector::ReadReg) reg,&data) )
            cout << "read: " << data << endl;
        else
            cout << "Error" << endl;
        if (loop)
        {
            now=time(NULL);
            if ((bef != 0) && (now != bef)) 
                cout << (now -bef) << " second passed" << endl;
            count++;
            bef=now;
        }
    }
    while (loop);
}
void Corrector::ScreenChannelCommands(){
    int read=0,rn=0;
    int func, chan,ret;
    do
    {
        cout << "1: Power On Channel" << endl;
        cout << "2: Reset Channel" << endl;
        cout << "3: Power Off Channel" << endl;
        cout << "4: Read Channel Status" << endl;
        rn= scanf("%d",&read);
    }
    while ((rn!=1) || (read < 0) || (read > 4) );
    func=read;
    do
    { 
        cout << "   which channel (1-8) ?" << endl;
        rn=scanf("%d",&read);
    }
    while ((rn!=1) || (read <= 0) || (read > 8) );
    chan=read-1;
    //cout << "func " << func << " Phchannel " << chan << endl;
    if (func == 4)
    {
        short int read;
        Corrector::ReadReg BaseAddress;
        switch(chan)
        {
            default:
            case 0: BaseAddress=Corrector::CH0_FAULTS;break;
            case 1: BaseAddress=Corrector::CH1_FAULTS;break;
            case 2: BaseAddress=Corrector::CH2_FAULTS;break;
            case 3: BaseAddress=Corrector::CH3_FAULTS;break;
            case 4: BaseAddress=Corrector::CH4_FAULTS;break;
            case 5: BaseAddress=Corrector::CH5_FAULTS;break;
            case 6: BaseAddress=Corrector::CH6_FAULTS;break;
            case 7: BaseAddress=Corrector::CH7_FAULTS;break;
             
        }
        ret= this->ReadBitRegister(BaseAddress,&read) ;
        if (ret)
            cout << "read channel "<< chan << " Faults " << std::hex << read << endl; 
        else
            cout << "error reading" << endl; 
        ret= this->ReadBitRegister((Corrector::ReadReg) (BaseAddress+1),&read) ;
        if (ret)
            cout << "read channel "<< chan  << " Status " << std::hex << read << endl; 
        else
            cout << "error reading" << endl; 
    }
    else
    {
        Corrector::Commands Com;
        if (func == 1)
            Com=Corrector::CHANNEL_ON;
        else if (func == 2)
            Com=Corrector::CHANNEL_RESET;
        if (func == 3)
            Com=Corrector::CHANNEL_OFF;
        
        this->SendChannelCommand(chan,Com);
    }
    
}
void Corrector::ScreenMainCommands(){
    int read=0,rn=0;
    do
    {
        cout << "1: Power On Main Unit" << endl;
        cout << "2: StandBy Main Unit" << endl;
        cout << "3: Reset Main Unit" << endl;
        cout << "4: Power Off Main Unit" << endl;
        
        rn= scanf("%d",&read);
    }
    while ((rn!=1) || (read < 0) || (read > 4) );
    switch (read)
    {
        case 1:  this->TurnOnMainUnit()? cout << "Command executed\n" : cout << "Error\n" ;
                 break;
        case 2:  this->TurnStandbyMainUnit()? cout << "Command executed\n" : cout << "Error\n" ;
                 break; 
        case 3:  this->ResetMainUnit()? cout << "Command executed\n" : cout << "Error\n" ;
                 break; 
        case 4:  { 
                    char c;
                    cout << "Warning! You will lost Remote control. Are you sure (y/n)?" << endl;
                    cin >> c;
                    if (c=='y')
                    {
                       cout <<"Shutting Down!" <<endl;
                       this->TurnOffMainUnit()? cout << "Command executed\n" : cout << "Error\n" ;
                    }
                    else
                       cout << "aborted" << endl;
                   } break;
        default: break; 
    }
    
}