void OLED_init() {
  myOLED.begin();
  myOLED.setBrightness(1);
  delay(20);
  myOLED.setOff(); //oled sleep, else it consumes 1.5mA. On pixels fire it uses 10mA
  delay(20);
}

void OLED_display() {
  delay(5);
  myOLED.setOn();
  delay(5);
  myOLED.clrScr();
  myOLED.setFont(SmallFont);
  myOLED.print(F("ANNA LOVE! #"), 10, 32);
  myOLED.printNumI(IM_SENSOR_NUM, 90, 32);
  myOLED.update();
  delay(1000);
  SYS_collectSensorsData();
  myOLED.clrScr();
  myOLED.setFont(SmallFont);
  myOLED.print(F("%"), 55, 10);
  myOLED.print(F("o"), 122, 0);
  myOLED.print(F("V"), 55, 42);
  myOLED.setFont(BigNumbers);
  myOLED.printNumI(humidity, 0, 0);
  myOLED.printNumI(temperature, 66, 0);
  myOLED.printNumF(batteryVoltage, 2, 0, 32);
  OLED_render_batt(batteryVoltage);  
  myOLED.update();
  delay(5000);
  myOLED.clrScr();
  myOLED.update();
  delay(10);
  myOLED.setOff();
  delay(10);
}

void OLED_render_batt(float batteryVoltage) {
  if (batteryVoltage <= BATT_min) {
    myOLED.setFont(SmallFont);
    myOLED.print(F(" LOW BATT "), 66, 52);
  }

  batteryVoltage = constrain(batteryVoltage, BATT_min,  BATT_max);
  int countLines = map(batteryVoltage * 100, BATT_min * 100, BATT_max * 100, 0, (123 - 66));
  myOLED.drawRect(66, 32, 123, 48);
  myOLED.drawLine(124, 36, 124, 45);
  myOLED.drawLine(125, 36, 125, 45);
  myOLED.drawLine(126, 36, 126, 45);
  myOLED.drawLine(127, 36, 127, 45);

  for (int i = 0; i <= countLines; i++) {
    myOLED.drawLine(66 + i, 32, 66 + i, 48);
  }




}




