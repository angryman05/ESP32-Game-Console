void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);   // LED-ul albastru de pe placă e pe GPIO2
}
void loop() {
  digitalWrite(2, HIGH); delay(500);
  digitalWrite(2, LOW);  delay(500);
  Serial.println("Salut de la ESP32!");
}