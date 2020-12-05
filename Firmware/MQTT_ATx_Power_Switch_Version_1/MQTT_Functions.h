
// test if message received
void callback(char* topic, byte* payload, unsigned int length_1) {

#ifdef Print_Report_Level_1
  Serial.print("Message length = "); Serial.println(length_1);
#endif

  // make a string of the received message
  String  Message_Arrived = "";
  for (unsigned int i = 0; i < length_1; i++) {
    Message_Arrived = Message_Arrived + ((char)payload[i]);
  }

#ifdef Print_Report_Level_1
  Serial.print("Message arrived and made into string: "); Serial.println(Message_Arrived);
#endif

  // is the message for only you? Either contains your MAC address or the for all address (broadcast)
  if (Message_Arrived.indexOf(My_MAC) >= 0 || Message_Arrived.indexOf(Broadcast_All) >= 0) {

    // valid message received, remove the headers either All or Mac
    // remove *ALL if present
    if  (Message_Arrived.indexOf(Broadcast_All) >= 0) {
      // remove the address string characters
      Message_Arrived.replace(Broadcast_All, "");
    } // end of strip and test for ALL broadcast

    // remove the MAC number if present
    if  (Message_Arrived.indexOf(My_MAC) >= 0) {
      // remove the address string characters
      Message_Arrived.replace(My_MAC, "");
    } // end of strip and test for ALL broadcast


    if ((Message_Arrived.indexOf("On") >= 0) ) {
      Mode = "On";
      Report_Request = 1;
    }

    else {
      Mode = "Off";
      Report_Request = 1;
    } // End of received function test


    // test for Reboot command
    if ((Message_Arrived.indexOf("#REBOOT") >= 0) ) {

      Serial.println("Reboot Request!");

      ESP.restart();

    } // end of reboot test function


  } // end of a valid message received

  // clear watchdog timer
#ifdef Enable_Watchdog
  watchdogCount = 0;
#endif
  yield();

} // end of callback


// connect to MQTT broker
void reconnect() {

  // loop until we're reconnected
  while (!client.connected()) {

    // attempt to connect
    Serial.print("Attempting MQTT Broker connection...");

    // make MAC address array used for Client ID
    char  MAC_array[My_MAC.length()];
    My_MAC.toCharArray(MAC_array, (My_MAC.length() + 1));

    // connect client and use MAC address array as the Client ID
    if (client.connect(MAC_array)) {

      Serial.println("connected");
      Serial.print("This is the client ID Used: "); Serial.println(MAC_array);

      // once connected, publish an announcement...
      Report_Request = 1;   // Request a report after power up

      // ... and resubscribe
      client.subscribe(InControl);
      delay(10);  // It needs a delay here else does not subsribe correctly!
      Serial.print("Sunbscribed to: "); Serial.println(InControl);

    } // end of if connected

    else {

      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);

    } // end of else

  } // end of while loop

} // end of function


void Bollocks() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();


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


} // end of function
