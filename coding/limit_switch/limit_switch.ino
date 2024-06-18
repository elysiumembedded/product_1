#define limit_switch 7

void setup() {
  Serial.begin(9600);
  pinMode(limit_switch, INPUT_PULLUP);
}

void loop()
{
  int limit_value = digitalRead(limit_switch);
  Serial.println(limit_value);
  delay(1);
}
