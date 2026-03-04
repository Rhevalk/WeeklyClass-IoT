int a = 3;

void setup() {
  Serial.begin(115200); 
}

void loop() {
  Serial.println("1"); 
  delay(500);
  Serial.println("2"); 
  delay(500);
  Serial.println(a);
  delay(500);
}

