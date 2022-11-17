
void executeInExactly500ms(void *pvParameters){
  char *nomeDaTarefa;
  TickType_t momentoDoInicio;
  nomeDaTarefa    = (char *) pvParameters;
  momentoDoInicio = xTaskGetTickCount();
  for (;;){
      Serial.print("exatamente em 500ms: ");
      Serial.println(nomeDaTarefa);
      vTaskDelayUntil(&momentoDoInicio,pdMS_TO_TICKS(500));
  }
}
