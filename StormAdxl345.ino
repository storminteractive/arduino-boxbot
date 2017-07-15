#include <Wire.h> 
#define adxlAddr 0x53 // ADXL345 I2C address is 0x53(83)

bool wireSend(byte dataAddress, byte data){
  Wire.beginTransmission(adxlAddr);
  Wire.write(dataAddress);
  Wire.write(data);
  Wire.endTransmission();
  return true;
}

byte wireReadByte(byte dataAddress){
  Wire.beginTransmission(adxlAddr);
  Wire.write(dataAddress);
  Wire.endTransmission();
  Wire.requestFrom(adxlAddr,1);
  if(Wire.available() == 1) {
    return Wire.read();
  } else {
    return 0x00;
  }
}

void setup(){  
  Wire.begin(); // Initialise I2C communication as MASTER  
  Serial.begin(9600);    
  
  wireSend(0x2C,0x0A);// Bandwidth rate register, 100Hz (145uA)
  //wireSend(0x2C,0x0F);// Bandwidth rate register, 3200Hz (145uA)
  //wireSend(0x2C,0x08);// Bandwidth rate register, 25Hz (65uA)
  wireSend(0x2D,0x08);// Power control register, auto-sleep disable
  // wireSend(0x31,B00001011);// no self test, 4-wire interface, Full resolution, Range = +/-16g
  wireSend(0x31,B00001010); // 4-wire interface, Full resolution, Range = +/-8g
  delay(300);
}

int convertTo10bit(unsigned int msb,unsigned int lsb){
  int tmp = (((msb & 0x03) * 256) + lsb);
  if(tmp>511) tmp -= 1024;
  return tmp;
}

float acclToG(int accl){
  return accl*0.005; // resolution +/- 16g, 10 bits
}

void readAccel(int &x,int &y, int &z){
  unsigned int data[6];  
  for(int i = 0; i < 6; i++)  {
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb    
    data[i] = wireReadByte(50+i);
  }

  // Convert the data to 10-bits
  x = convertTo10bit(data[1],data[0]);
  y = convertTo10bit(data[3],data[2]);
  z = convertTo10bit(data[5],data[4]);
}

void loop(){  
  int xAccl,yAccl,zAccl;
  readAccel(xAccl,yAccl,zAccl);

  xAccl = (int)(xAccl/10);
  yAccl = (int)(yAccl/10);
  zAccl = (int)(zAccl/10);

  // Output data to serial monitor  
  // Serial.print("Acceleration in XYZ-Axis is : ");
  //Serial.print(xAccl);Serial.print(" ");Serial.print(yAccl);Serial.print(" ");Serial.println(zAccl);

  int tresh = 0;
  if((xAccl>tresh) || (yAccl>tresh) || (zAccl>tresh)){
    Serial.print(xAccl);Serial.print(" ");Serial.print(yAccl);Serial.print(" ");Serial.println(zAccl);
    delay(100);
  }
  

  /*
  float xG = acclToG(xAccl);
  float yG = acclToG(yAccl);
  float zG = acclToG(zAccl);
  float tresh = 1.3;  
  if((xG>tresh) || (yG>tresh) || (zG>tresh)){
    Serial.print(xG);Serial.print(" ");Serial.print(yG);Serial.print(" ");Serial.println(zG);
    delay(100);
  }
  */

}
