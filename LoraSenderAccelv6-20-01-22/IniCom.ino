

// string ini:1Mod:1
void iniCom(String stringOne) {
  
  String tempStr = "";

  String index1 = "ini:";
  String index2 = "Mod:";
  if (stringOne != "" && stringOne.startsWith("ini:")) {
    //Serial.println(stringOne);

    int posiA = stringOne.indexOf(index1);
    int posiB = stringOne.indexOf(index2);

    // Recebe Qual Modulo Está Funcionando;
    tempStr = stringOne.substring(posiA + index1.length(),posiB);
    ini = tempStr.toInt();
    tempStr = stringOne.substring(posiB + index2.length());
    Mod = tempStr.toInt();


  }

}
