/**************************************************************

*File:QTouch.cpp
 Author:Vikas Gaikwad
 Company: Honey Bees Technologies
*Description: This file contains the  Methods of class QTouch

*Date:05-07-2015

*
****************************************************************/

#include "QTouch.h"
#include "BLE.h"
#include "HBT.h"
//#include "RTClib.h"


//RTC_DS1307 myRTC;
BLE mystatus;
//HBT myhbt;

QTouch::QTouch()
{
  //do nothings

}
/*========================================================================================================

	File: QTouch.cpp/

	Description: This File Contains the Methods of configuring the GPIO, INPUT and OUTPUT pinModes

	class: QTouch	

	Method: gpioInit()	

==========================================================================================================*/
void QTouch::gpioInit(void)
{
  
   uint8_t capsensepins[8] = {Lamp1,Lamp2,Lamp3,Socket,Fan_Control,Up,Down};
   uint8_t loadoutpins[8] = {Load1,Load2,Load3,Load4,Level1,Level2,Level3,Level4};//Load6=NC

  //Set Pinmodes of I/O pins

  for(uint8_t i=0;i<8;i++)
  {
    pinMode(capsensepins[i],INPUT);
    pinMode(loadoutpins[i],OUTPUT);
  }  
  
  //Set pinValue to LOW
  /*digitalWrite(Lamp1,LOW);
  digitalWrite(Lamp2,LOW);
  digitalWrite(Lamp3,LOW);
  digitalWrite(Socket,LOW);
  digitalWrite(Fan_Control,LOW);
  digitalWrite(Up,LOW);
  digitalWrite(Down,LOW);    */
  for(uint8_t i=0;i<8;i++)
  {
    digitalWrite(capsensepins[i],LOW);
    digitalWrite(loadoutpins[i],LOW);
  }
    
}


void QTouch:: LightControl(uint8_t pinNo, uint8_t pinValue)
{
  pin_num = pinNo;
  pin_Value = pinValue;
  Serial.print(pin_num);
  Serial.print(pin_Value);
  digitalWrite(pinNo,pinValue);                   // Lights ON/OFF
}

/*========================================================================================================

	File: QTouch.cpp

	Description: This methods used to set the Respective AC Loads 

==========================================================================================================*/

void QTouch::L1_ON(void)  
{	
     	
  digitalWrite(Load1,HIGH);
}

void QTouch::L2_ON(void)
{
  digitalWrite(Load2,HIGH);
}

void QTouch::L3_ON(void)
{
  digitalWrite(Load3,HIGH);
}

void QTouch::Socket_ON(void)      //Socket Control SW
{
  digitalWrite(Load4,HIGH);
}

void QTouch::FAN_ON(void)
{
  //Fan switch on and Level set to Default i.e Level-1
  digitalWrite(Level1,HIGH);    //set to Level-1
  Level2_OFF();Level3_OFF();Level4_OFF();
}
void QTouch::Level2_ON(void)
{
  digitalWrite(Level2,HIGH);
  digitalWrite(Level1,LOW);Level3_OFF();Level4_OFF();
}

void QTouch::Level3_ON(void)
{
  digitalWrite(Level3,HIGH);
  digitalWrite(Level1,LOW);Level2_OFF();Level4_OFF();
}

void QTouch::Level4_ON(void)
{
  digitalWrite(Level4,HIGH);
  digitalWrite(Level1,LOW);Level2_OFF();Level3_OFF();
}

  
void QTouch::Master_ON(void)  // Now Master is ON == Make ALL ON
{
  //curr_status = getstatus;
  //L1_ON();L2_ON();L3_ON();
  digitalWrite(Load1,HIGH);
  digitalWrite(Load2,HIGH);
  digitalWrite(Load3,HIGH);
  digitalWrite(Load4,HIGH);  //Make them OFF
  Socket_ON();
  FAN_ON();                  //Fan at Default Speed Level -1
  
}
/*========================================================================================================

	File: QTouch.cpp

	Description: This methods used to Switch OFF the Respective AC Loads 

==========================================================================================================*/

void QTouch::L1_OFF(void)
{
  digitalWrite(Load1,LOW);
}

void QTouch::L2_OFF(void)
{
  digitalWrite(Load2,LOW);
  
}

void QTouch::L3_OFF(void)
{
  digitalWrite(Load3,LOW);

}

void QTouch::Socket_OFF(void)
{
  digitalWrite(Load4,LOW);      //Socket OFF

}

void QTouch::FAN_OFF(void)
{
  digitalWrite(Level1,LOW);
  digitalWrite(Level2,LOW);
  digitalWrite(Level3,LOW);
  digitalWrite(Level4,LOW);
  
}

void QTouch::Level2_OFF(void)
{
  digitalWrite(Level2,LOW);
}

void QTouch::Level3_OFF(void)
{
  digitalWrite(Level3,LOW);
}

void QTouch::Level4_OFF(void)
{
  digitalWrite(Level4,LOW);
}


void QTouch::Master_OFF(void)
{
  digitalWrite(Load1,LOW);
  digitalWrite(Load2,LOW);
  digitalWrite(Load3,LOW);
  digitalWrite(Load4,LOW);  //Make them OFF
  FAN_OFF();
  
}
/*========================================================================================================

	File: QTouch.cpp

	Description: This method use to read Capsense Switch Board Inputs contineously
	             and returns a 8-bit number(byte)

==========================================================================================================*/

byte QTouch::readCapsense()
{
  //Read the Input Port Status and returns int value
  
  boolean l1 = digitalRead(Lamp1);
  boolean l2 = digitalRead(Lamp2);
  boolean l3 = digitalRead(Lamp3);
  boolean l4 = digitalRead(Socket);
  boolean l5 = digitalRead(Fan_Control);
  boolean l6 = digitalRead(Up);
  boolean l7 = digitalRead(Down);
  
  
  byte no[8] = {l1,l2,l3,l4,l6,l5,l7};//no[0] = LSB
  byte val=0;
  
  for(uint8_t i=0;i<=7;i++)
  {
    val|= (no[i]<< i);    //convert bits to byte
  }
      
  //Serial.print(val);
   
  return val;
  
}

/*========================================================================================================

	File: QTouch.cpp

	Description: This method used to set the Load as per present Capsense Inputs 

==========================================================================================================*/


void QTouch::cap_setLoad(byte loadValue)
{
  m_loadValue = loadValue;
  prev_loadValue = m_loadValue;
  
  //Serial.println(m_loadValue,HEX);
  switch(m_loadValue)
  {
    case(0x00)://Master + ALL switches are OFF   D23,D22,D21,D20,D19 
              Master_OFF(); break;
              
    case(0x01):
            FAN_OFF();Socket_OFF();L3_OFF();L2_OFF();L1_ON(); break;
  
    case(0x02):
             FAN_OFF();Socket_OFF();L3_OFF();L2_ON();L1_OFF();  break;

    case(0x03):
             FAN_OFF();Socket_OFF();L3_OFF();L2_ON();L1_ON();  break;

    case(0x04):
             FAN_OFF();Socket_OFF();L3_ON();L2_OFF();L1_OFF();  break;
     
    case(0x05):
             FAN_OFF();Socket_OFF();L3_ON();L2_OFF();L1_ON();  break;
 
     case(0x06):
             FAN_OFF();Socket_OFF();L3_ON();L2_ON();L1_OFF();  break;

     case(0x07):
             FAN_OFF();Socket_OFF();L3_ON();L2_ON();L1_ON();  break;
    
     case(0x08):
             FAN_OFF();Socket_ON();L3_OFF();L2_OFF();L1_OFF();  break;

     case(0x09):
             FAN_OFF();Socket_ON();L3_OFF();L2_OFF();L1_ON();  break;

     case(0x0A):
             FAN_OFF();Socket_ON();L3_OFF();L2_ON();L1_OFF();  break;

     case(0x0B):
             FAN_OFF();Socket_ON();L3_OFF();L2_ON();L1_ON();  break;

     case(0x0C):
             FAN_OFF();Socket_ON();L3_ON();L2_OFF();L1_OFF();  break;

     case(0x0D):
             FAN_OFF();Socket_ON();L3_ON();L2_OFF();L1_ON();  break;
    
     case(0x0E):
             FAN_OFF();Socket_ON();L3_ON();L2_ON();L1_OFF();  break;

     case(0x0F):
             FAN_OFF();Socket_ON();L3_ON();L2_ON();L1_ON();  break;

     case(0x10):
             Serial.println("Level - 1");
             FAN_ON();Socket_OFF();L3_OFF();L2_OFF();L1_OFF();  break;  //Set to Level-1
  
     case(0x11):
             FAN_ON();Socket_OFF();L3_OFF();L2_OFF();L1_ON();  break;

     case(0x12):
             FAN_ON();Socket_OFF();L3_OFF();L2_ON();L1_OFF();  break;

     case(0x13):
            FAN_ON();Socket_OFF();L3_OFF();L2_ON();L1_ON();  break;

     case(0x14):
             FAN_ON();Socket_OFF();L3_ON();L2_OFF();L1_OFF();  break;

     case(0x15):
             FAN_ON();Socket_OFF();L3_ON();L2_OFF();L1_ON();  break;

     case(0x16):
            FAN_ON();Socket_OFF();L3_ON();L2_ON();L1_OFF();  break;

     case(0x17):
            FAN_ON();Socket_OFF();L3_ON();L2_ON();L1_ON();  break;

     case(0x18):
            FAN_ON();Socket_ON();L3_OFF();L2_OFF();L1_OFF();  break;

     case(0x19):
            FAN_ON();Socket_ON();L3_OFF();L2_OFF();L1_ON();  break;

     case(0x1A):
            FAN_ON();Socket_ON();L3_OFF();L2_ON();L1_OFF();  break;

     case(0x1B):
            FAN_ON();Socket_ON();L3_OFF();L2_ON();L1_ON();  break;

     case(0x1C):
            FAN_ON();Socket_ON();L3_ON();L2_OFF();L1_OFF();  break;
           
     case(0x1D):
            FAN_ON();Socket_ON();L3_ON();L2_OFF();L1_ON();  break;

     case(0x1E):
            FAN_ON();Socket_ON();L3_ON();L2_ON();L1_OFF();  break;

     case(0x1F):
            FAN_ON();Socket_ON();L3_ON();L2_ON();L1_ON();  break;

     case(0x20):// master i/p ON    0010 0000                        Set to Level-2
                  Serial.println("Level - 2");
                  Level2_ON();Socket_OFF();L3_OFF();L2_OFF();L1_OFF();  break;
    
     case(0x21)://   0010 0001                        Set to Level-2
                  Level2_ON();Socket_OFF();L3_OFF();L2_OFF();L1_ON();  break;
      
     case(0x22)://    0010 0010                        Set to Level-2
                  Level2_ON();Socket_OFF();L3_OFF();L2_ON();L1_OFF();  break;

     case(0x23)://    0010 0011                        Set to Level-2
                  Level2_ON();Socket_OFF();L3_OFF();L2_ON();L1_ON();  break;

     case(0x24)://    0010 0100                        Set to Level-2
                  Level2_ON();Socket_OFF();L3_ON();L2_OFF();L1_OFF();  break;

     case(0x25)://    0010 0101                        Set to Level-2
                  Level2_ON();Socket_OFF();L3_ON();L2_OFF();L1_OFF();  break;

     case(0x26)://    0010 0110                        Set to Level-2
                  Level2_ON();Socket_OFF();L3_ON();L2_ON();L1_OFF();  break;
    
    case(0x27)://    0010 0111                        Set to Level-2
                  Level2_ON();Socket_OFF();L3_ON();L2_ON();L1_ON();  break;

    case(0x28)://    0010 1000                        Set to Level-2
                  Level2_ON();Socket_ON();L3_OFF();L2_OFF();L1_OFF();  break;

    case(0x29)://    0010 1001                        Set to Level-2
                   Level2_ON();Socket_ON();L3_OFF();L2_OFF();L1_ON();  break;

    case(0x2A)://    0010 1010                        Set to Level-2
                  Level2_ON();Socket_ON();L3_OFF();L2_ON();L1_OFF();  break;

    case(0x2B)://    0010 1011                        Set to Level-2
                  Level2_ON();Socket_ON();L3_OFF();L2_ON();L1_ON();  break;

    case(0x2C)://    0010 1100                        Set to Level-2
                  Level2_ON();Socket_ON();L3_ON();L2_OFF();L1_OFF();  break;

    case(0x2D)://    0010 1101                        Set to Level-2
                  Level2_ON();Socket_ON();L3_ON();L2_OFF();L1_ON();  break;

    case(0x2E)://    0010 1110                        Set to Level-2
                  Level2_ON();Socket_ON();L3_ON();L2_ON();L1_OFF();  break;

    case(0x2F)://    0010 1111                       Set to Level-2
                  Level2_ON();Socket_ON();L3_ON();L2_ON();L1_ON();  break;

    case(0x30)://    0011 0000                        Set to Level-3
                  Serial.println("Level - 3");
                  Level3_ON();Socket_OFF();L3_OFF();L2_OFF();L1_OFF();  break;

    case(0x31)://    0011 0001                        Set to Level-3
                  Level3_ON();Socket_OFF();L3_OFF();L2_OFF();L1_ON();  break;

    case(0x32)://    0011 0010                        Set to Level-3
                  Level3_ON();Socket_OFF();L3_OFF();L2_ON();L1_OFF();  break;

    case(0x33)://    0011 0011                        Set to Level-3
                  Level3_ON();Socket_OFF();L3_OFF();L2_ON();L1_ON();  break;

    case(0x34)://    0011 0100                        Set to Level-3
                  Level3_ON();Socket_OFF();L3_ON();L2_OFF();L1_OFF();  break;

    case(0x35)://    0011 0101                        Set to Level-3
                  Level3_ON();Socket_OFF();L3_ON();L2_OFF();L1_ON();  break;

    case(0x36)://    0011 0110                        Set to Level-3
                  Level3_ON();Socket_OFF();L3_ON();L2_ON();L1_OFF();  break;

    case(0x37)://    0011 0111                        Set to Level-3
                  Level3_ON();Socket_OFF();L3_ON();L2_ON();L1_ON();  break;

    case(0x38)://    0011 1000                      Set to Level-3
                  Level3_ON();Socket_ON();L3_OFF();L2_OFF();L1_OFF();  break;

    case(0x39)://    0011 1001                        Set to Level-3
                  Level3_ON();Socket_ON();L3_OFF();L2_OFF();L1_ON();  break;

    case(0x3A)://    0011 1010                        Set to Level-3
                  Level3_ON();Socket_ON();L3_OFF();L2_ON();L1_OFF();  break;

    case(0x3B)://    0011 1011                       Set to Level-3
                  Level3_ON();Socket_ON();L3_OFF();L2_ON();L1_ON();  break;

    case(0x3C)://    0011 1100                        Set to Level-3
                  Level3_ON();Socket_ON();L3_ON();L2_OFF();L1_OFF();  break;

    case(0x3D)://    0011 1101                        Set to Level-3
                  Level3_ON();Socket_ON();L3_ON();L2_OFF();L1_ON();  break;

    case(0x3E)://    0011 1110                        Set to Level-3
                  Level3_ON();Socket_ON();L3_ON();L2_ON();L1_OFF();  break;

    case(0x3F)://    0011 1111                        Set to Level-3
                  Level3_ON();Socket_ON();L3_ON();L2_ON();L1_ON();  break;

     
    case(0x40)://    0100 0000                        Set to Level-4
                  Serial.println("Level - 4");
                  Level4_ON();Socket_OFF();L3_OFF();L2_OFF();L1_OFF();  break;

    case(0x41)://    0100 0001                       Set to Level-4
                  Level4_ON();Socket_OFF();L3_OFF();L2_OFF();L1_ON();  break;

    case(0x42)://    0100 0010                        Set to Level-4
                  Level4_ON();Socket_OFF();L3_OFF();L2_ON();L1_OFF();  break;

    case(0x43)://    0100 0011                        Set to Level-4
                  Level4_ON();Socket_OFF();L3_OFF();L2_ON();L1_ON();  break;

    case(0x44)://    0100 0100                        Set to Level-4
                  Level4_ON();Socket_OFF();L3_ON();L2_OFF();L1_OFF();  break;

    case(0x45)://    0100 0101                        Set to Level-4
                  Level4_ON();Socket_OFF();L3_ON();L2_OFF();L1_ON();  break;

    case(0x46)://    0100 0110                        Set to Level-4
                  Level4_ON();Socket_OFF();L3_ON();L2_ON();L1_OFF();  break;

    case(0x47)://    0100 0111                        Set to Level-4
                  Level4_ON();Socket_OFF();L3_ON();L2_ON();L1_ON();  break;

    case(0x48)://    0100 1000                        Set to Level-4
                  Level4_ON();Socket_ON();L3_OFF();L2_OFF();L1_OFF();  break;

    case(0x49)://    0100 1001                        Set to Level-4
                  Level4_ON();Socket_ON();L3_OFF();L2_OFF();L1_ON();  break;

    case(0x4A)://    0100 1010                        Set to Level-4
                  Level4_ON();Socket_ON();L3_OFF();L2_ON();L1_OFF();  break;

    case(0x4B)://    0100 1011                        Set to Level-4
                  Level4_ON();Socket_ON();L3_OFF();L2_ON();L1_ON();  break;

    case(0x4C)://    0100 1100                        Set to Level-4
                  Level4_ON();Socket_ON();L3_ON();L2_OFF();L1_OFF();  break;

    case(0x4D)://    0100 1101                        Set to Level-4
                  Level4_ON();Socket_ON();L3_ON();L2_OFF();L1_ON();  break;

    case(0x4E)://    0100 1110                        Set to Level-4
                  Level4_ON();Socket_ON();L3_ON();L2_ON();L1_OFF();  break;

    case(0x4F)://    0100 1111                        Set to Level-4
                  Level4_ON();Socket_ON();L3_ON();L2_ON();L1_ON();  break;

    }
          
}

byte QTouch::readLoadstatus(void)
{
  //read output pins status
  
  boolean s1 = digitalRead(Load1);
  boolean s2 = digitalRead(Load2);
  boolean s3 = digitalRead(Load3);
  boolean s4 = digitalRead(Load4);
  boolean s5 = digitalRead(Level1);
  boolean s6 = digitalRead(Level2);
  boolean s7 = digitalRead(Level3);
  boolean s8 = digitalRead(Level4);
 
              //LSB......... ........MSB
  byte no[8] = {s1,s2,s3,s4,s5,s6,s7,s8};
  byte val=0;
  
  for(uint8_t i=0;i<=7;i++)
  {
    val|= (no[i]<< i);    //convert bits to byte
  }
      
  //Serial.pruint8_t(val);
  return val;
}


