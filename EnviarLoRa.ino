// Envia um pacote de dados dia Rede LoRa

void EnviaLoRa (String dados) {
  LoRa.beginPacket();
  LoRa.print(dados);
  LoRa.endPacket();
}
