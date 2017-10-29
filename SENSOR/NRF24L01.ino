void NRF24_sendDataToBase() {
  SYS_collectSensorsData();

  int16_t arrayToBase[8] = {
    (int)batteryVoltage*100,               // 100*V.xx 0=null, voltage on sensor battery, 100*V
    (int)temperature+100,              //T   0=null, -50..120 [+100]   temperature, C
    (int)humidity+100,                 //H   0=null, 0..100   [+100]   humidity, %
    (int)0,                            //WL   0=null, 100, 101          water leak, bool  
    (int)0,                            //MD   0=null, 100, 101          motion detector, bool  
    (int)0,(int)0,(int)0                             //ALARM_MODE 0=null 100,101 bool on-off (Im at home or NOT. So, system defenders my house or nor)
  };

  NRF24_sendData(arrayToBase, sizeof(arrayToBase));
}


void NRF24_init() {
  delay(50);
  NRF24_radio.begin();
  delay(100);
  NRF24_radio.powerUp();
  delay(50);
  NRF24_radio.setChannel(0x6D);  //также обнуляет счетчик непринятых сообщений, что хорошо!
  NRF24_radio.setRetries(15, 15);
  NRF24_radio.setDataRate(RF24_1MBPS);
  NRF24_radio.setPALevel(RF24_PA_LOW);
  NRF24_radio.setCRCLength(RF24_CRC_16);

  /*
    ===writeAckPayload===enableDynamicPayloads===
    !  Only three of these can be pending at any time as there are only 3 FIFO buffers.
    !  Dynamic payloads must be enabled.
    !  write an ack payload as soon as startListening() is called
  */
  NRF24_radio.enableDynamicPayloads();//for ALL pipes
  //NRF24_radio.setPayloadSize(32); //32 bytes? Can corrupt "writeAckPayload"?

  NRF24_radio.setAutoAck(true);////allow RX send answer(acknoledgement) to TX (for ALL pipes?)
  NRF24_radio.enableAckPayload(); // Разрешение отправки нетипового ответа передатчику;
  ////NRF24_radio.enableDynamicAck(); //for ALL pipes? Чтобы можно было вкл\выкл получение обратного ответа ACK?

  NRF24_radio.stopListening();// flush_tx() & flush_tx()  inside this function   

  NRF24_radio.openWritingPipe(pipes[IM_SENSOR_NUM]); //use pipe 2.  pipe0 is SYSTEM_pipe, no reading

  delay(50);
  NRF24_radio.powerDown();
  delay(50);
}

void NRF24_sendData(int16_t* arrayToBase, uint8_t sizeofArrayToBase) { 
  NRF24_radio.powerUp();
  delay(50);

  //Stop listening for incoming messages, and switch to transmit mode.
  //Do this before calling write().
  NRF24_radio.stopListening(); // flush_tx() & flush_tx()  inside this function

  /*
    PLOS_CNT - четыре бита, значение которых увеличивается, вплоть до достижения 15, при каждой отправке,
    на которую НЕ получено ожидаемое подтверждение.
    Значение сбрасывается при записи в регистр RF_CH.
    RF_CH - это номер канала. И тут у меня что-то щелкнуло. Иногда первые 15 пакетов (или меньше),
    посылаемые с высокой частотой, доходили без сбоев. Попробовал записывать номер канала перед передачей чтобы этот регистр обнулить.
    В результате удача. Посылки идут с частотой 200 Гц и все работает без ошибок.
  */
  NRF24_radio.setChannel(0x6D);  //также обнуляет счетчик непринятых сообщений, что хорошо!

  NRF24_radio.write( arrayToBase, sizeofArrayToBase);
  //& не надо, в ф-ю уже передал указатель, а не сам массив

  uint8_t answerFromBase; //2^8 - 1   [0,255]  I send answerACK some 0..255 from BASE просто так)))
  if ( NRF24_radio.isAckPayloadAvailable() ) {
    NRF24_radio.read(&answerFromBase, sizeof(answerFromBase)); //приемник принял и ответил
  }

  delay(50);
  NRF24_radio.powerDown();
  delay(10);
}
