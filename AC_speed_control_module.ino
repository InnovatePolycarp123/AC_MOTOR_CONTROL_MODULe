volatile bool ZC = false;
#define OUTPUT_CONTROL A0
#define INTERRUPT_PIN 2
#define MAXIMUM_VALUE 3000
#define MINIMUM_VALUE 300
#define TRIAC_CONTROL 5
int frequency_counter;
uint32_t previousmillis = 0;


void zeroCrossingInterruptHandler() {
  ZC = true;
}


int frequencyCounter() {

  if (ZC) {
    frequency_counter++;
    ZC = false;
  }

  if ((millis() - previousmillis) > 1000) {
    int frequency = frequency_counter / 2;
    frequency_counter = 0;
    previousmillis = millis();
    return frequency;
  }

  return -1;
}

uint16_t outputControlKnob() {

 uint16_t   read_from_analog;

read_from_analog = analogRead(OUTPUT_CONTROL);


  // Serial.println( read_from_analog);

  uint16_t calibrated_value = map(read_from_analog, 0, 872, MINIMUM_VALUE, MAXIMUM_VALUE);

    calibrated_value = constrain(calibrated_value, 300, 4000);

  return calibrated_value;
}

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT_CONTROL, INPUT);
  pinMode(TRIAC_CONTROL, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), zeroCrossingInterruptHandler, RISING);  // Attach interrupt for pin 2

  Serial.println("Setup complete, waiting for zero-crossing...");
}

void loop() {

  uint16_t motor_control = outputControlKnob();
  

   if (ZC) {
     Serial.println("zero-crossing Occured...");
    delayMicroseconds(motor_control);
    digitalWrite(TRIAC_CONTROL, HIGH);
    delayMicroseconds(250);
    digitalWrite(TRIAC_CONTROL, LOW);
    ZC = false;
  }




















}
