

// Mostra os dados atuais do Sistema
bool  estado = false; 
const int tEspera = 100; 
void displayDados() {
  unsigned int line = 0;
  display.drawString(0, 0, "Modulo:" + String(Mod));
  line++;

  display.drawString(60, 0, "Bateria:" + String(Bat) + "%");
  line++;

  if (statusAlarme == 2 && staPorta == 1) {
    display.drawString(0, line * 10, "Status:" + String(statusAlarme) + " Alarme Acionado!!!");
    //digitalWrite(25, HIGH);
    //digitalWrite(32, HIGH);
    //digitalWrite(33, HIGH);
  } else {
    display.drawString(0, line * 10, "Status:" + String(statusAlarme));
    //digitalWrite(25, LOW);
    // digitalWrite(32, HIGH);
      if (estado == true){
        display.drawString(0, line * 10, "Status:" + String(statusAlarme) + "Religando");
        delay(tEspera);
        digitalWrite(33, LOW);
      }
     
  }
  if (statusAlarme == 3 && staPorta == 1) {
    display.drawString(0, line * 10, "Status:" + String(statusAlarme) + "Rele Desligado!");
    //digitalWrite(25, HIGH);
    estado = true; 
    digitalWrite(33, HIGH);
  }

  line++;
  if (staPorta == 1) {
    display.drawString(0, line * 10, "Porta Fechada");
  } else {
    display.drawString(0, line * 10, "Porta Aberta");
  }
  rssi = "RSSI=  " + String(LoRa.packetRssi(), DEC) + "dB";
  display.drawString(60, 10, rssi);
  display.drawString(90, 20, "D:" + String(D));
   line++;
  display.drawString(0,line * 10 , "Freq : " + String(BAND / 1000000) + "mHz");
  display.display();

}
