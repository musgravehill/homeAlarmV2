void SYS_init() {
  MCUSR = 0;  //VERY VERY IMPORTANT!!!! ELSE WDT DOESNOT RESET, DOESNOT DISABLED!!!
  wdt_disable();
  delay(10);
  analogReference(INTERNAL); //1.1 V
  //use the 1.1 V internal reference => other A* can NOT receive VCC, only 1.1V max. But i2c a4a5 can works?
  /*
    DEFAULT: опорное напряжение по умолчанию, равное 5 В (на 5В-платах Ардуино) или 3.3 В (на 3.3В-платах Ардуино)
    INTERNAL: внутренне опорное напряжение, равное 1.1 В в микроконтроллерах ATmega168 и ATmega328, или 2.56 В в микроконтроллере ATmega8 (не доступно в Arduino Mega)
    INTERNAL1V1: внутреннее опорное напряжение 1.1 В (только для Arduino Mega)
    INTERNAL2V56: внутреннее опорное напряжение 2.56 В (только для Arduino Mega)
    EXTERNAL: в качестве опорного напряжения будет использоваться напряжение, приложенное к выводу AREF (от 0 до 5В)
  */
  pinMode(BUTTON_WAKEUP_PIN, INPUT_PULLUP);
}

void SYS_collectSensorsData() {
  //возможно, тут надо dht.setup после долгого сна
  humidity = (int) dht.getHumidity();
  temperature = (int) dht.getTemperature();
  batteryVoltage = 0.00596285 * analogRead(BATT_CONTROL_PIN_1V1);
  // 1M, 220K divider across battery and using internal ADC ref of 1.1V
  // Sense point is bypassed with 0.1 uF cap to reduce noise at that point
  // ((1e6+220e3)/220e3)*1.1 = Vmax = 6.1000000 Volts
  // 6.1000/1023 = Volts per bit = 0.003363075

  //delay(dht.getMinimumSamplingPeriod()); //dht22 need 2s pause between requesting data

}
