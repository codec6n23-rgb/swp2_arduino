#define PIN_LED 13
unsigned int count , toggle;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(1000000);
  while(!Serial) {
  }
  // put your main code here, to run repeatedly:
  Serial.println("Hellow World!");
  delay(1000);
  digitalWrite(PIN_LED, toggle);
}
void loop(){
  Serial.println(++count);
  toggle = toggle_state(toggle);
  digitalWrite(PIN_LED, toggle);
  delay(1000);
}
int toggle_state(int toggle) {
  return !toggle;
}
