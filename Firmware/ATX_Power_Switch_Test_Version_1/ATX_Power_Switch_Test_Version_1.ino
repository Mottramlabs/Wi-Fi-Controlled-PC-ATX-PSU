/*
    Title:        ATX Switch Test
    Date:         5th December 2020
    Version:      1
    Description:  Sample code
    Device:       ESP8266
*/

/* ********************************** Compiler settings, un-comment to use ************************** */
//#define Fixed_IP                      // un-comment to use a fixed IP address to speed up development
//#define Print_Report_Level_1          // un-comment for option
#define Print_Report_Level_2          // un-comment for option, report received MQTT message
#define Enable_Watchdog                 // un-comment to enable the watchdog option
/* ************************************************************************************************** */

#include <ESP8266WiFi.h>              // needed for EPS8266
#include <WiFiClient.h>               // WiFi client

// custom settings files
#include "Project_Settings.h"         // board specific details.


void setup() {

  GPIO_Setup();                       // Setup relay, LED and switch (if present)

} // end of setup


void loop() {

  Relay_Control();                    // Relay control, also LED if present
  Switch_Control();                   // Test switch input

} // end of loop
