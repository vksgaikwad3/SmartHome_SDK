/* Project: Smart Switch Board Vr1
   Author:Vikas Gaikwad
   Company : Honey Bees Technologies
   Date  :03/02/2016
   Descriptions: This is a Smart Switch board can be communicated with Capsence Touch Switch Board,RF Remote and a Bluetooth Low Energy [BTL4.0] using Smart Phone.  
                 Followed the Protocol while communicating through BLE4.0 to set and get Acknowledgment from Host micro controller.
   Note:    1. After every change in state status gets sync on Smart phone (in Smart phone App).
            2. Capacitive Touch and RF Remote and BLE4.0 Enabled. 
               i) Capacitive Touch = Direct control (TTL)
               ii)RF434 MHz Transmitter and receiver communication
               iii)BLE4.0 =  Access string to control and Read Status of Switch Boards   
 */


#include <avr/wdt.h>
#include "HBT.h"
#include <EEPROM.h>
#include <Wire.h>   // Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "QTouch.h"
#include "BLE.h"
#include <avr/sleep.h>
#include "RTClib.h"
#include "avr/interrupt.h"
#include <util/delay.h>
//#include <stdio.h>



void device1_onTime_log(byte ,byte ,byte ,byte );
void device2_onTime_log(byte ,byte ,byte ,byte );
void device3_onTime_log(byte ,byte ,byte ,byte );
void device4_onTime_log(byte ,byte ,byte ,byte );
void device5_onTime_log(byte ,byte ,byte ,byte );


void device1_offTime_log(byte, byte, byte, byte);
void device2_offTime_log(byte, byte, byte, byte);
void device3_offTime_log(byte, byte, byte, byte);
void device4_offTime_log(byte, byte, byte, byte);
void device5_offTime_log(byte, byte, byte, byte);
uint32_t device_delta_Time_Log(uint8_t, uint8_t, byte, byte, byte, byte);


RTC_DS1307 rtc;		// instance of class
HBT myHBT;          // Object Variables
QTouch myTouch;


//Global Variables

int Pin_Type;    //  Stores Pin Type
int Pin_No;      //  Stores Pin Number
int Pin_Value;   //  Stores Pin Value


volatile byte touch_input=0,prev_touch_input=0,temp,change,new_state;
volatile byte prev_load_output_status=0,curr_load_output_status=0,prev_output_status=0;
int switch_num=0;
bool onflag = false;
uint32_t  time;
byte mask_byte_0, mask_byte_1, mask_byte_2, mask_byte_3;
uint8_t adr_index=0;
uint8_t device2_str_adr=205, device3_str_adr = 500, device4_str_adr=617, device5_str_adr = 817;
uint8_t device2_offTime_adr=207, device3_offTime_adr =505, device4_offTime_adr=622, device5_offTime_adr=822;
bool d1_onFlag = false,d2_onFlag = false,d3_onFlag = false,d4_onFlag = false,d5_onFlag = false; 
bool d1_offFlag = false,d2_offFlag = false,d3_offFlag = false,d4_offFlag = false,d5_offFlag = false;

void setup()
{
	Serial.begin(9600);
	myHBT.begin(9600);

	myHBT.enterCommandMode();

	myHBT.setName("Bed Room");
	myHBT.setTransmissionPower(Six);
	myHBT.setWorkRole(SLAVE);
	myHBT.setAuth(OPEN);
	myHBT.setWorkMode(REMOTE_CONTROL);
	myHBT.setiBeaconMode(On);
	
	myHBT.exitCommandMode();
	
	myTouch.gpioInit();    // set all pins
	
	if (! rtc.begin()) {					// very important to begin RTC
		Serial.println("Couldn't find RTC");
		while (1);
	}
	
	if (! rtc.isrunning()) {
		Serial.println("RTC is NOT running!");
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		//rtc.adjust(DateTime(2016, 5, 24, 16, 05, 0));
	}
	
	for (int addr = 0; addr <= EEPROM.length(); addr++)	//Clears the EEPROM
	{  
		EEPROM.write(addr,0);	
		//myHBT.print("A:");myHBT.print(addr);
	}
	
	DDRD |= (1<<DDD6);
	
}

void loop()
{  
   sei();				// Global Interrupts Enable 
   touch_input = myTouch.readCapsense();  //Read capsense values
   
   touch_input = touch_input ;
   //myHBT.print(touch_input,HEX);
   
   DateTime now = rtc.now();		// This has to be in while(1)
   
   //uint32_t elpsec = now.unixtime();
   
   /*byte offTime_byte_0 = elpsec & 0x000000FF;		    //masking Lower 
   
   byte offTime_byte_1 = (elpsec & 0x0000FF00) >> 8;		//masking byte1 & shift it
   byte offTime_byte_2 = (elpsec & 0x00FF0000) >> 16;		//masking byte2 & shift it
   byte offTime_byte_3 = (elpsec & 0xFF000000) >> 24;		//masking byte2 & shift it
   
   
   myHBT.print("B3:");myHBT.print(offTime_byte_3,HEX);		//returns total no of seconds, since standard epoch of 1/1/1970 till now.
   myHBT.print("B2:"); myHBT.print(offTime_byte_2,HEX);
   myHBT.print("B1:"); myHBT.print(offTime_byte_1,HEX);
   myHBT.print("B0:"); myHBT.print(offTime_byte_0,HEX);
	*/  
  
   if(myHBT.available()>0)                //phone connected and Rx commands
   {
	   Pin_Type = myHBT.parseInt();
	   Pin_No = myHBT.parseInt();
	   Pin_Value = myHBT.parseInt();

	   //Serial.print(Pin_Type);
	   //Serial.print(",");
	   //Serial.print(Pin_No);
	   //Serial.print(",");
	   //Serial.println(Pin_Value);
	   //Serial.print("I am Here");
	   // myTouch.LightControl(Pin_No,Pin_Value);
	   if(myHBT.read() =='H')                      // detects end line character "H"
	   {
		   //Serial.print("I am in if.read()");
		   //if Pin Type is 0 means Digital Output
		   switch(Pin_Type)
		   {
			   case(0):
			   if((Pin_No == Level1 || Pin_No == Level2 || Pin_No == Level3 || Pin_No == Level4) && (Pin_Value == 1))    // Any one Level is selected and Value set to HIGH
			   { Serial.print("In Case 0");
				   if(Pin_No == Level1 && Pin_Value ==1){ Serial.print("Level 1");myTouch.FAN_ON();}       //  Level 1 ON
				   if(Pin_No == Level2 && Pin_Value ==1){ Serial.print("Level 2");myTouch.Level2_ON();}    //  Level 2 ON
				   if(Pin_No == Level3 && Pin_Value ==1) {Serial.print("Level 3");myTouch.Level3_ON();}    //  Level 3 ON
				   if(Pin_No == Level4 && Pin_Value ==1) {Serial.print("Level 4");myTouch.Level4_ON();}    //  Level 4 ON
			   }
			   
			   else
			   digitalWrite(Pin_No,Pin_Value); break;                // Relay ON/OFF
			   
			   case(222):myTouch.FAN_OFF();              // FAN OFF
			   //Serial.print("FAN OFF");
			   break;
			   
			   case(111):
			   myTouch.Master_ON();
			   //Serial.println("Master ON");
			   break;
			   case(999):
			   myTouch.Master_OFF();
			   //Serial.println("Master OFF");
			   break;
			   
		   }
		   
	   }
	   curr_load_output_status = myTouch.readLoadstatus();myHBT.print("v:");
	   myHBT.print(curr_load_output_status);  //Output Status To update into Smart-phone app to display current status of lights
	    
	
   }
   
   
    if(touch_input ^ prev_touch_input )		// Cap sense Inputs
  {
	//myHBT.print(now.minute(), DEC);
    //when BLE is not connected and signals coming from IR Remote and Cap sense Board
    //Serial.print("Prev Touch:");Serial.println(prev_touch_input,HEX);
    //Serial.print("Touch_input:");Serial.println(touch_input,HEX);
    
    temp=touch_input;       // current status byte 
    change = prev_touch_input ^ touch_input;    //if there is a change in state then o/p = !0 
   // Serial.print("Temp:");Serial.println(temp);
    new_state    = change & curr_load_output_status;
    myTouch.cap_setLoad(temp);          //switch as per new_state
    
	curr_load_output_status = myTouch.readLoadstatus() ;      // Output status of Loads
	myHBT.print("LO:");myHBT.print(curr_load_output_status);
		
	prev_load_output_status = (curr_load_output_status & 0x0F);
	
	prev_touch_input = touch_input;
    	
  }
  
  //myHBT.print("LOT:");myHBT.print(curr_load_output_status & 0x0F);
  // Note: Day month year DD:MM HH:Min 
  
 myHBT.print("B3:");myHBT.print(EEPROM.read(3),HEX);		//returns total no of seconds, since standard epoch of  

  if (curr_load_output_status ^ prev_load_output_status )		// we can use X-OR 0 0 -> 0, 1 x -> 1 ,0 x -> 1
  {
	  //curr_load_output_status = curr_load_output_status; // Masking for SWitches only
	  myHBT.print("Out:");myHBT.print(curr_load_output_status);
    
		switch(curr_load_output_status)				// output state
	   {
		  case(0x00):
				 myHBT.print("In Zero");
				time = now.unixtime();
				
				mask_byte_0 = time & 0x000000FF;		    //masking Lower
				mask_byte_1 = (time & 0x0000FF00) >> 8;		//masking byte1 & shift it
				mask_byte_2 = (time & 0x00FF0000) >> 16;		//masking byte2 & shift it
				mask_byte_3 = (time & 0xFF000000) >> 24;		//masking byte2 & shift it


				 if( d1_onFlag == 1 ) // d1 ON -OFF
				 {
					//device1_offTime_log()
				 }		 
				 delay(2000);
				 break;
	  
		 case(0x01):
	 			 //uint8_t new_State = curr_load_output_status ^ prev_load_output_status ; 
				 //Store the ON Time To EEPROM				Day,Hour,Minutes,sec
	     		 				 		
				 //device1_onTime_log(device1_onTime_byte_0,device1_onTime_byte_1,device1_onTime_byte_2,device1_onTime_byte_3);					 

				//Store Device 1 ON Time to EEPROM from locations 0 - 3
				// myHBT.print("B3:");myHBT.print(device1_onTime_byte_3,HEX);		//returns total no of seconds, since standard epoch of 1/1/1970 till now.
				 //myHBT.print("B2:");myHBT.print(device1_onTime_byte_2,HEX);
				 //myHBT.print("B1:"); myHBT.print(device1_onTime_byte_1,HEX);
				 //myHBT.print("B0:"); myHBT.print(device1_onTime_byte_0,HEX);
				 
				 delay(2000);							
				 break;
	  
		 case(0x02):
				myHBT.print("In 2:");
				delay(2000);		
				break;
	   }
  
    prev_load_output_status = curr_load_output_status;
  
 } 
  
  delay(500);    //Update Rate
  wdt_enable(WDTO_4S);
} // loop

/// Functions to keep a log of device ON time and device OFF time

	
void device1_onTime_log(byte byte0,byte byte1,byte byte2,byte byte3)
{	
		uint8_t device1On_Time_array[4] = {byte0,byte1,byte2,byte3};
	    adr_index =  0;
			for(adr_index = 0; adr_index < 4; adr_index++)
			{
				EEPROM.write(adr_index,device1On_Time_array[adr_index]);		//Store byte0 - byte3 from Address 0 to 3 
			}
d1_onFlag =true;
}			

void device2_onTime_log(byte byte0,byte byte1,byte byte2,byte byte3)
{		
		uint8_t device2On_Time_array[4] = {byte0,byte1,byte2,byte3};
		device2_str_adr = 205;
			for(adr_index =0 ; adr_index < 4; adr_index++)
			{
				EEPROM.write( device2_str_adr + adr_index ,device2On_Time_array[adr_index]);		//Store byte0 - byte3 from Address 0 to 3
			}
d2_onFlag = true;
}

void device3_onTime_log(byte byte0,byte byte1,byte byte2,byte byte3)
{			
			uint8_t device3On_Time_array[4] = {byte0,byte1,byte2,byte3};
			device3_str_adr = 500;
			for(adr_index =0 ; adr_index < 4; adr_index++)                                              
			{
				EEPROM.write( device3_str_adr + adr_index ,device3On_Time_array[adr_index]);		//Store byte0 - byte3 from Address 0 to 3
			}
d3_onFlag = true;
}

void device4_onTime_log(byte byte0,byte byte1,byte byte2,byte byte3)
{
			uint8_t device4On_Time_array[4] = {byte0,byte1,byte2,byte3};
			device4_str_adr = 617;
			for(adr_index =0 ; adr_index < 4; adr_index++)
			{
				EEPROM.write(device4_str_adr + adr_index ,device4On_Time_array[adr_index]);		//Store byte0 - byte3 from Address 0 to 3
			}
d4_onFlag = true;
}

void device5_onTime_log(byte byte0,byte byte1,byte byte2,byte byte3)
{
			uint8_t device5On_Time_array[4] = {byte0,byte1,byte2,byte3};
			uint8_t device5_str_adr = 817;
			for(adr_index =0 ; adr_index < 4; adr_index++)
			{
				EEPROM.write(device5_str_adr + adr_index ,device5On_Time_array[adr_index]);		//Store byte0 - byte3 from Address 0 to 3
			}
d5_onFlag = true;
}

void device1_offTime_log(byte byte0,byte byte1,byte byte2,byte byte3)
{
	uint8_t device1Off_Time_array[4] = {byte0,byte1,byte2,byte3};
	uint8_t adr_index =  5;
	for(adr_index = 5; adr_index < 9; adr_index++)
	{
		EEPROM.write(adr_index,device1Off_Time_array[adr_index]);		//Store byte0 - byte3 from Address 0 to 3
	}
d1_offFlag = true;
}

void device2_offTime_log(byte byte0,byte byte1,byte byte2,byte byte3)
{
	uint8_t device2Off_Time_array[4] = {byte0,byte1,byte2,byte3};
	uint8_t device2OffTime_address =  207;
	
	for(uint8_t adr_index = 0; adr_index < 4; adr_index++)
	{
		EEPROM.write(device2OffTime_address + adr_index, device2Off_Time_array[adr_index]);		//Store byte0 - byte3 from Address 0 to 3
	}
d2_offFlag = true;
}

void device3_offTime_log(byte byte0, byte byte1, byte byte2, byte byte3)
{
	uint8_t device3Off_Time_array[4] = {byte0,byte1,byte2,byte3};
	uint8_t device3_offTime_adr = 505;
	for(uint8_t adr_index =0 ; adr_index < 4; adr_index++)
	{
		EEPROM.write( device3_offTime_adr + adr_index , device3Off_Time_array[adr_index]);		//Store byte0 - byte3 from Address 0 to 3
	}
d3_offFlag =true;
}

void device4_offTime_log(byte byte0, byte byte1, byte byte2, byte byte3)
{
	uint8_t device4Off_Time_array[4] = {byte0,byte1,byte2,byte3};
	uint8_t device4_offTime_adr = 622;
	for(uint8_t adr_index =0 ; adr_index < 4; adr_index++)
	{
		EEPROM.write(device4_offTime_adr + adr_index ,device4Off_Time_array[adr_index]);		//Store byte0 - byte3 from Address 0 to 3
	}
d4_offFlag =true;
}

void device5_offTime_log(byte byte0,byte byte1, byte byte2, byte byte3)
{
	uint8_t device5Off_Time_array[4] = {byte0,byte1,byte2,byte3};
	uint8_t device5_offTime_adr = 822;
	for(uint8_t adr_index =0 ; adr_index < 4; adr_index++)
	{
		EEPROM.write(device5_offTime_adr + adr_index ,device5Off_Time_array[adr_index]);		//Store byte0 - byte3 from Address 0 to 3
	}
d5_offFlag =true;
}
