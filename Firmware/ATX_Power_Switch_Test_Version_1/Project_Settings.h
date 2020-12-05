/*
    variables and functions used by this project
*/

// ********************************************************************************************************
// ************************ project specific variables ****************************************************
// ********************************************************************************************************
String Mode = "";

// I/O and product variables
#define Run_LED     13                          // GPIO for LED
#define Relay       12                          // GPIO for Relay
#define Switch      0                           // GPIO for Switch

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

    } // end of if

    else {
      Mode = "On";

    } // end of else

  } // Finished testing button

} // end of function
