/*
    variables and functions used by this project
*/

// Heatbeat items
#define Heartbeat_Range   99                      // event count max value
#define Heatbeat    10000                         // heartbeat timer value, in mS

// Watchdog items
volatile int Watchdog_Timout = 1200;              // time in seconds before watchdog times out, 1200 = 1200 seconds or 20 minutes

// Heatbeat timer
int Heart_Value = 0;                              // timer
long LastMsg = 0;                                 // message sent counter

// Custom default values
String WiFi_SSID = "None";                        // SSID string
String My_MAC = "";                               // MAC address, tobe read from ESP8266
char MAC_array[13] = "000000000000";              // MAC definition
String My_IP = "";                                // IP address

volatile int Report_Request = 0;                  // set to 1 if report required
String Mode = "";                                 // default message event


// ********************************************************************************************************
// ************************ project specific variables ****************************************************
// ********************************************************************************************************

// I/O and product variables
#define Run_LED     13                          // GPIO for LED
#define Relay       12                          // GPIO for Relay
#define Switch      0                           // GPIO for Switch


#ifdef Enable_Watchdog
// ********************************************************************************************************
// ************************ watchdog items ****************************************************************
// ********************************************************************************************************

#include <Ticker.h>
Ticker secondTick;
volatile int watchdogCount = 0;

// watchdog timer function, watchdog is clear when the ststus report is requested by a GET request
void ISRwatchdog() {
  watchdogCount++;
  if (watchdogCount == Watchdog_Timout) {
    Serial.println();
    Serial.println("The watchdog bites !!!!!");
    ESP.reset();
  } // end of timeout test
} // end of watchdog count
#endif


// ********************************************************************************************************
// ************************ custom functions **************************************************************
// ********************************************************************************************************
void GPIO_Setup() {

  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);           //Relay off

  pinMode(Run_LED, OUTPUT);
  digitalWrite(Run_LED, LOW);       //LED off

  pinMode(Switch, INPUT);

} // end of function


void LED_Blink_On() {
  digitalWrite(Run_LED, HIGH);      //LED on
}


void LED_Blink_Off() {
  digitalWrite(Run_LED, LOW);       //LED off
}


void Relay_Control() {

  if ((Mode.indexOf("On") >= 0) ) {

    digitalWrite(Relay, HIGH);
    delay(50);
    LED_Blink_On();
    delay(50);
    LED_Blink_Off();

  } // End of test

  else {

    digitalWrite(Relay, LOW);
    LED_Blink_On();

  } // end of else

} // end of function


// Test manual override button
void Switch_Control() {

  int val = digitalRead(Switch);  // Read Switch
  if (val == LOW) {         // Is it pressed?

    // Wait for switch release, and debounce
    while (val == LOW) {
      val = digitalRead(Switch);  // Read Switch
      delay(100);
    } // end of while

    // Toggle Relay mode
    if ((Mode.indexOf("On") >= 0) ) {
      Mode = "Off";
      Report_Request = 1;   // Request a report to be sent

    } // end of if

    else {
      Mode = "On";
      Report_Request = 1;   // Request a report to be sent

    } // end of else

  } // Finished testing button

} // end of function
