/*
    Title:        MQTT and the new ATX Siwtch
    Date:         2nd March 2020
    Version:      1
    Description:  Sample code
    Device:       ESP8266
    WiFi:      See "WiFi_Settings.h" for wifi AP and password
    MQTT:     See "MQTT_Settings.h" for MQTT broker and topic
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
#include "Wifi_Settings.h"            // custom Wifi settings
#include "MQTT_Settings.h"            // MQTT broker and topic
#include "Project_Settings.h"         // board specific details.

// incude WiFi and MQTT functions
WiFiClient espClient;                 // for ESP8266 boards
#include <PubSubClient.h>             // http://pubsubclient.knolleary.net/api.html
PubSubClient client(espClient);       // ESP pubsub client
#include "WiFi_Functions.h"           // read wifi data
#include "MQTT_Functions.h"           // MQTT Functions


void setup() {

#ifdef Enable_Watchdog
  // watchdog items, comment out if not used
  secondTick.attach(1, ISRwatchdog);
#endif

  GPIO_Setup();                       // Setup relay, LED and switch (if present)

  // connect to WiFi access point
  Get_Wifi();

  // connect to the MQTT broker
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // reset heartbeat timer
  LastMsg = millis();

  Mode = "Boot"; Report_Request = 1;

} // end of setup


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Relay_Control();                    // Relay control, also LED if present
  Switch_Control();                   // Test switch input


  // headbeat or report requested
  if (millis() - LastMsg > Heatbeat || Report_Request == 1) {

    LastMsg = millis();
    Report_Request = 0;

    // update event progress counter
    ++Heart_Value;
    if (Heart_Value > Heartbeat_Range) {
      Heart_Value = 1;
    }

    // heartbeat timed out or report message requested

    // get a Report Make and make as an array
    String Report = Status_Report();
    char Report_array[(Report.length() + 1)];
    Report.toCharArray(Report_array, (Report.length() + 1));

#ifdef Print_Report_Level_2
    // display a report when publishing
    Serial.print("Published To topic: "); Serial.print(InStatus); Serial.print("  -  Report Sent: "); Serial.println(Report_array);
#endif

    // send a status report
    client.publish(InStatus, Report_array);

  } // end of heartbeat timer


} // end of loop
