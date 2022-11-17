

void decifraLoRa(String stringOne) {
  display.clear();
  int line;
  String tempStr = "";


  String index1 = "x:", index2 = "y:", index3 = "z:";
  if (stringOne != "" && stringOne.startsWith("x:")) {
    Serial.println(stringOne);

    int posiA = stringOne.indexOf(index1);
    int posiB = stringOne.indexOf(index2);
    int posiC = stringOne.indexOf(index3);

    tempStr = stringOne.substring(posiA + index1.length(), posiB);
    int a = tempStr.toInt();
    tempStr = stringOne.substring(posiB + index2.length(), posiC);
    int b = tempStr.toInt();
    tempStr = stringOne.substring(posiC + index3.length());
    int c = tempStr.toInt();

    display.drawString(0, 0, "x =" + String(a));
    line++;

    display.drawString(0, line * 10, "y =" + String(b));
    line++;
    display.drawString(0, line * 10, "z =" + String(c));
    display.display();
    stringOne = "";
  }

}
