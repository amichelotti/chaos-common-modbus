/* 
 * File:   provalo.cpp
 * Author: alex
 *
 * Created on 11 novembre 2015, 14.00
 */

#include <cstdlib>
#include "HazemeyerLib.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    char SERIALCONN[]="/dev/ttyr0f,9600,N,8,1";
    std::string SerString="/dev/ttyr0f,2,9600,N,8,1";
    //Hazemeyer::Corrector Corr2( SERIALCONN );
    Hazemeyer::Corrector CORRETTORE( (char*) SerString.c_str() );
    
    if(CORRETTORE.Connect())
    {
        bool ret;
        short int read=0;
        cout << "connection established" << endl;
        cout << "stato connessione " << CORRETTORE.getConnectionStatus() <<endl;
        CORRETTORE.ScreenMenu();
        goto end;
        
                      
        ret= CORRETTORE.ReadBitRegister(Hazemeyer::Corrector::CH0_POWER_STATUS,&read) ;
        if (ret)
            cout << "read CH0_POWER_STATUS " << std::hex << read << endl; 
        else
            cout << "error reading" << endl; 
        usleep(100000);
        
       
      
        
  end:      CORRETTORE.CloseConnection();
        cout << "stato connessione " << CORRETTORE.getConnectionStatus() <<endl;
    }
    return 0;
}


/*
 unsigned long address;
            uint32_t data=0,toconv;

            address = getData("read address");
            if((err=modbus_drv->read_input_registers(address,1,(uint16_t*)&data,slave))<0)
*/
