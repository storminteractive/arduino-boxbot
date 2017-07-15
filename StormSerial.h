#ifndef StormSerial_h
#define StormSerial_h
#include "Arduino.h"

#define WAIT while(Serial.available()<=0) {} 
#define DEBUG

class StormSerial{
    public:
    StormSerial(bool echo, int BOM, int EOM);
    
    String getSerialStringNonblocking();
    String getSerialStringBlocking();
    
    bool checkMessage();   
    String readMessage();
    String readMessageAndClear();
    void clearMessage();
    void dbg(String);
    //String getSerialStringBlocking();
    private:
    String nonBlockingString;
    bool messageStarted;
    bool _echo;  
    bool _messageAvailable;
    int _BOM;
    int _EOM;
};

#endif
