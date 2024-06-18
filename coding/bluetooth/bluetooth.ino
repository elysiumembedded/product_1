#define green 13
#define red 12
#include <SoftwareSerial.h>
SoftwareSerial bluetoothSerial(7,8);
char command;
void setup()
{
  Serial.begin(9600);
  bluetoothSerial.begin(9600);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
}

void loop()
{
  if (bluetoothSerial.available() > 0)
  {
    command = bluetoothSerial.read();
    bluetoothSerial.println(command);
  }
  if(command == '1')
  {
    digitalWrite(red, HIGH);
  }
  if(command == '2')
  {
    digitalWrite(red, LOW);
  }
}
