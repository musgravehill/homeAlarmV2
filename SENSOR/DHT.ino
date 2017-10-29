void DHT_init() {
  delay(1000);
  dht.setup(DHT22_DATA_PIN, dht.AM2302);
}
