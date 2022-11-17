

void decifraLoRa(String stringOne) {
  
  display.clear();
  int line;
  String tempStr = "";

 //"Mod:%dBat:%dSta:%d"
  String index1 = "Mod:", index2 = "Bat:", index3 = "Sta:", index4 = "D:";
  if (stringOne != "" && stringOne.startsWith("Mod")) {
    Serial.println(stringOne);

    int posiA = stringOne.indexOf(index1);
    int posiB = stringOne.indexOf(index2);
    int posiC = stringOne.indexOf(index3);
    int posiD = stringOne.indexOf(index4);

    tempStr = stringOne.substring(posiA + index1.length(), posiB);
    Mod = tempStr.toInt();
    tempStr = stringOne.substring(posiB + index2.length(), posiC);
    Bat = tempStr.toInt();
    tempStr = stringOne.substring(posiC + index3.length(),posiD);
    statusAlarme = tempStr.toInt();
    tempStr = stringOne.substring(posiD + index4.length());
    D = tempStr.toInt();

    stringOne = "";
    displayDados();
  }

}
