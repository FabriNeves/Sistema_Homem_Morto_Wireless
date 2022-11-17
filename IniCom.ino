


int iniCom(String stringOne) {
  display.clear();
  int line;
  int Mod ;
  String tempStr = "";


  String index1 = "Mod:";
  if (stringOne != "" && stringOne.startsWith("ini:")) {
    //Serial.println(stringOne);

    int posiA = stringOne.indexOf(index1);
   
    // Recebe Qual Modulo Está Funcionando; 
    tempStr = stringOne.substring(posiA + index1.length());
    Mod = tempStr.toInt();
  

    display.drawString(0, 0, "Mod = " + String(Mod));
    display.display();
    stringOne = "";
  }

  return Mod;
 
}
