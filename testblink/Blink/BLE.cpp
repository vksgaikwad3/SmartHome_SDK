/**************************************************************

*File:BLE.cpp
 Author:Vikas Gaikwad
 Company: Honey Bees Technologies
*Description: This file contains the  Methods of class BLE4.0 CC2541 Chip-set is used

*Date:05-07-2015

*
****************************************************************/

#include "BLE.h"
#include "QTouch.h"
#include "HBT.h"


HBT myHBTstatus;  //HBT object
QTouch capTouch;



BLE::BLE()
{
  //do notihing
}


byte BLE::getStatus(void)
{
  byte curr_data = capTouch.readCapsense();    //Read current switchboard status
  
                                                //Read all I/P pins status and update a status with return type "String"
  return curr_data;
    
}

void BLE::updateStatus(byte data)      // send to Smart phone App
{
  curr_status = data;                          // send current status to Smartphone
                                               //l11 l10 ,l21 l20 , l31 l30 ,l41 l40 ,l51 l50 m0 m1
  Serial.print("in a updateStatus");
  Serial.println(curr_data);                                                
  
  switch(curr_status)                          
  {
    case(0x00)://Master + ALL switches are OFF   D23,D22,D21,D20,D19 
               myHBTstatus.print("s10,s20,s30,s40,s50,m0"); 
               break;
                           
    case(0x01):
              myHBTstatus.print("s11,s20,s30,s40,s50,m0");  break;
                
    case(0x02):
              myHBTstatus.print("s10,s21,s30,s40,s50,m0"); break;

    case(0x03):
              myHBTstatus.print("s11,s21,s30,s40,s50,m0"); break;
    case(0x04):
              myHBTstatus.print("s10,s20,s31,s40,s50,m0"); break;
    case(0x05):
              myHBTstatus.print("s11,s20,s31,s40,s50,m0"); break;
    case(0x06):
              myHBTstatus.print("s10,s21,s31,s40,s50,m0"); break;
     case(0x07):
              myHBTstatus.print("s11,s21,s31,s40,s50,m0"); break;
     case(0x08):
              myHBTstatus.print("s10,s20,s30,s41,s50,m0"); break;
     case(0x09):
              myHBTstatus.print("s11,s21,s30,s41,s50,m0"); break;
     case(0x0A):
             myHBTstatus.print("s10,s21,s30,s41,s50,m0"); break;
     case(0x0B):
             myHBTstatus.print("s11,s21,s30,s41,s50,m0"); break;
     case(0x0C):
             myHBTstatus.print("s10,s20,s31,s41,s50,m0"); break;
     case(0x0D):
             myHBTstatus.print("s11,s20,s31,s41,s50,m0"); break;
     case(0x0E):
             myHBTstatus.print("s10,s21,s31,s41,s50,m0"); break;
     case(0x0F):
             myHBTstatus.print("s11,s21,s31,s41,s50,m0"); break;
     case(0x10):
             myHBTstatus.print("s10,s20,s30,s40,s51,m0"); break;
     case(0x11):
             myHBTstatus.print("s11,s20,s30,s40,s51,m0"); break;
     case(0x12):
             myHBTstatus.print("s10,s21,s30,s40,s51,m0"); break;
     case(0x13):
             myHBTstatus.print("s11,s21,s30,s40,s51,m0"); break;
     case(0x14):
             myHBTstatus.print("s10,s20,s31,s40,s51,m0"); break;
     case(0x15):
             myHBTstatus.print("s11,s20,s31,s40,s51,m0"); break;
     case(0x16):
             myHBTstatus.print("s10,s21,s31,s40,s51,m0"); break;
     case(0x17):
             myHBTstatus.print("s11,s21,s31,s40,s51,m0"); break;
     case(0x18):
             myHBTstatus.print("s10,s20,s30,s41,s51,m0"); break;
     case(0x19):
             myHBTstatus.print("s11,s20,s30,s41,s51,m0"); break;
     case(0x1A):
             myHBTstatus.print("s10,s21,s30,s41,s51,m0"); break;
     case(0x1B):
             myHBTstatus.print("s11,s21,s30,s41,s51,m0"); break;
     case(0x1C):
             myHBTstatus.print("s10,s20,s31,s41,s51,m0"); break;
     case(0x1D):
             myHBTstatus.print("s11,s20,s31,s41,s51,m0"); break;
     case(0x1E):
             myHBTstatus.print("s10,s21,s31,s41,s51,m0"); break;
     case(0x1F):
             myHBTstatus.print("s11,s21,s31,s41,s51,m0"); break;
     case(0x20):
             myHBTstatus.print("s10,s20,s30,s40,s50,m1"); break;
          
          // from here below all coditions Master gets ON
  }    
    
}


