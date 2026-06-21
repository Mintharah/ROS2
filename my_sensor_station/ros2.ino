#define TRIG_PIN 9
#define ECHO_PIN 10
#define POT_PIN A0

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

float readDistance(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  float distance_cm = duration * 0.034 / 2.0;

  return distance_cm;
}
void loop() {
  float dist = readDistance();

  int pot = analogRead(POT_PIN);

  float threshold = map(pot, 0, 1023, 0, 200);

  Serial.print(dist);

  Serial.print(',');
  
  Serial.println(threshold);

  delay(10);
}
