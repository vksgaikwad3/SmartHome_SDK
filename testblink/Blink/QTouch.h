/**************************************************************

*File:QTouch.h
 Author:Vikas Gaikwad
 Company: Honey Bees Technologies
*Description: This is a header file contains the  declarations of class QTouch and its methods

*Date:05-07-2015

*
****************************************************************/

#ifndef  QTouch_h
#define  QTouch_h

#include <Arduino.h>
#include "HBT.h"
#include "BLE.h"
#include <avr/io.h>
#include "RTClib.h"

//#define Lamp1_Dir_Input   (DDRD &= 0b0000000)       //PD5 SW1 

//#define SW1   (PIND & 0b00100000)    //PD5
// Capsence Input Pins

#define Lamp1      7  //(PIND & 0b00100000)    PD5 Input pin             
#define Lamp2      4    
#define Lamp3      12     //A6 
#define Socket     6  
#define Fan_Control 10 
#define Up          8
#define Down        9
//Output Pins to control Triac Switching

#define Load1   14   //Relay 1 -PB3
#define Load2   15    //Relay 2 -PB1
#define Load3   16   //Relay 3  -PB2
#define Load4   17   //D17 Relay 4 - Socket pin		-PB0
#define Level1  5   //D5 (PC6)			
#define Level2  13   //D13(PC7)	
#define Level3  18   //D18(A0)	
#define Level4  19   //D19(A1)  



class QTouch
{
  
  public:
  
  uint8_t i;
  QTouch();        //Constructor
  void gpioInit(void);
  byte readCapsense(void);
  byte readLoadstatus(void);
  void cap_setLoad(byte loadValue);
  void L1_ON(void);
  void L2_ON(void);
  void L3_ON(void);
  void Socket_ON(void);
  void FAN_ON(void);             
  void Level2_ON(void);
  void Level3_ON(void);
  void Level4_ON(void);
  void Master_ON(void);
  
  void L1_OFF(void);
  void L2_OFF(void);
  void L3_OFF(void);
  void Socket_OFF(void);
  void FAN_OFF(void);
  void Level2_OFF(void);
  void Level3_OFF(void);
  void Level4_OFF(void);
  void Master_OFF(void);

  void LightControl(uint8_t pinNo , uint8_t pinValue);
  
  
  private:
  
  byte curr_status, prev_status;
  
  uint8_t m_loadValue, pin_num,pin_Value;
  uint8_t prev_loadValue;
  
  
};

#endif


