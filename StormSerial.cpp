#include "Arduino.h"
#include "StormSerial.h"

StormSerial::StormSerial(bool serialEcho,int BOM, int EOM){
    this->_echo = serialEcho;
    this->messageStarted = false;
    this->nonBlockingString = "";
    this->_messageAvailable = false;
    this->_BOM = BOM;
    this->_EOM = EOM;    
}

void StormSerial::dbg(String s){
  #ifdef DEBUG
    Serial.println("## DEBUG ##: "+s);
  #endif
}

// termChar 13 - enter
String StormSerial::getSerialStringBlocking(){
  char inChar=0;
  String inString = "";
  while(inChar!=this->_EOM){ // or while (inchar!='\n')
   WAIT;
   inChar = Serial.read();
   if(this->_echo) Serial.print(inChar);
   if((int)inChar==127){ // backspace
     if (inString.length()>0) inString=inString.substring(0,inString.length()-1);
   } else inString += inChar;
  }
  return inString;
}

String StormSerial::getSerialStringNonblocking(){
    if(Serial.available() > 0){
      char receivedByte = Serial.read();
      if(this->_echo) Serial.print(receivedByte);
      if(!messageStarted){
        
        if(receivedByte == this->_BOM) {
          this->messageStarted = true;
          dbg("Message started"); // debug
        }

      } else {

        if(receivedByte == this->_EOM){
          this->messageStarted = false;
          this->_messageAvailable = true;
          dbg("Message finished"); // debug
        } else {
          this->nonBlockingString += receivedByte;
          #ifdef DEBUG
          Serial.print("## DEBUG ##: "); Serial.println(receivedByte,DEC);
          #endif
        }

      }
    }
    return "";
}

String StormSerial::readMessage(){
    return nonBlockingString;
}

String StormSerial::readMessageAndClear(){
    String tmp = this->nonBlockingString;
    this->clearMessage();
    return tmp;
}

bool StormSerial::checkMessage(){
    this->getSerialStringNonblocking();
    return this->_messageAvailable;
}

void StormSerial::clearMessage(){
    this->nonBlockingString = "";
    this->_messageAvailable = false;
}
