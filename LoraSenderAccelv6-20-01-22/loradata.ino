
/*
void loraData(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0 , 18 , "Rx "+ packSize + " bytes");
  
  if (packet.length()>30){
    packet = packet+"/n";
  }
  
  display.drawStringMaxWidth(0 , 39 , 128, packet);
  display.drawString(0, 0, rssi);  
  display.display();
  delay(100);
  decifraLoRa(packet);
}
*/

/*
void printBuffer(String Frase) {
  // Initialize the log buffer
  // allocate memory to store 8 lines of text and 30 chars per line.
  display.setLogBuffer(5, 30);
  for (uint8_t i = 0; i < 11; i++) {
    display.clear();
    // Print to the screen
    display.println(Frase[i]);
    // Draw it to the internal screen buffer
    display.drawLogBuffer(0, 0);
    // Display it on the screen
    display.display();
    delay(500);
  }
}
*/
