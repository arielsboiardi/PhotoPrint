int LedStatus = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.write("Starting communications\n");
  Serial.write("Send Y to turn LED ON\n");
  Serial.write("Send X to turn LED OFF\n");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,LedStatus);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN,LedStatus);

  Serial.write("Hello \r\n");

  delay(1000);
}


void serialEvent()  
{
  if (Serial.available() > 0) {
      char comdata = char(Serial.read());
    if (comdata == 'Y') {
      Serial.println("LED is ON");
      LedStatus = HIGH;
    }
    else if (comdata == 'X') {
      Serial.println("LED turn OFF");
      LedStatus = LOW;
    }
  }
}