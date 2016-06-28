/**************************************************************

*File:QTouch.cpp
 Author:Vikas Gaikwad
 Company: Honey Bees Technologies
*Description: This is a BLE.h header file contains the class BLE and Methods

*Date:05-07-2015

*
****************************************************************/

#ifndef BLE_h
#define BLE_h

#include <Arduino.h>
#include "HBT.h"
#include "QTouch.h"

class BLE
{
  public:
  
  BLE();          //constructor
  byte getStatus(void);    //follow some protocol
  void updateStatus(byte data);  //send status to Smartphone
  //void getCommand(String value);
  void ble_setLoad(byte value);
  
  
  private:
  
  byte curr_status;
  byte prev_status;
  byte curr_data;
  byte m_value;
  
  
};

#endif
