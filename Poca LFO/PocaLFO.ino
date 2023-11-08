// Define Pin Assignments
#define LFO_1_KNOB  A1
#define LFO_2_KNOB  A0
#define LFO_1_RED   11
#define LFO_1_BLUE  10
#define LFO_2_RED   3
#define LFO_2_BLUE  9
#define LFO_1_SW    7
#define LFO_2_SW    8
#define LFO_1_OUT   6
#define LFO_2_OUT   5

// Declare Variables
int lfo_1_val_raw;
int lfo_2_val_raw;
float period1 = 500;
float period2 = 500;
float frequency1 = 1;
float frequency2 = 1;
int buttonState1 = LOW;
int buttonState2 = LOW;
int lastButtonState1 = LOW;
int lastButtonState2 = LOW;
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay = 50;
int waveformSelect1 = 0;
int waveformSelect2 = 0;
int counter1 = 0;
int counter2 = 0;



void setup() {
  // put your setup code here, to run once:
  // Setup pins as inputs or outputs
  Serial.begin(9600);
  pinMode(LFO_1_KNOB, INPUT);
  pinMode(LFO_2_KNOB, INPUT);
  pinMode(LFO_1_SW, INPUT);
  pinMode(LFO_2_SW, INPUT);
  pinMode(LFO_1_OUT, OUTPUT);
  pinMode(LFO_2_OUT, OUTPUT);
  pinMode(LFO_1_BLUE, OUTPUT);
  pinMode(LFO_1_RED, OUTPUT);
  pinMode(LFO_2_BLUE, OUTPUT);
  pinMode(LFO_2_RED, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  // Read in raw potentiometer values
  lfo_1_val_raw = analogRead(LFO_1_KNOB);
  lfo_2_val_raw = analogRead(LFO_2_KNOB);

  // Scale incoming values from 1 - 20hz
  frequency1 = map(lfo_1_val_raw, 0, 1023, 1, 20);
  frequency2 = map(lfo_2_val_raw, 0, 1023, 1, 20);

  // Calculate period of each frequency
  period1 = 1000 / frequency1;
  period2 = 1000 / frequency2;

  // Debouncing for buttons
  int reading1 = digitalRead(LFO_1_SW);
  if (reading1 != lastButtonState1){
    lastDebounceTime1 = millis();
  }
  if ((millis() - lastDebounceTime1) > debounceDelay){
    if (reading1 != buttonState1){
      buttonState1 = reading1;
      if (buttonState1 == HIGH){
        counter1++;
        waveformSelect1 = fmod(counter1, 5);
      }
    }
  }
  lastButtonState1 = reading1;

  int reading2 = digitalRead(LFO_2_SW);
  if (reading2 != lastButtonState2){
    lastDebounceTime2 = millis();
  }
  if ((millis() - lastDebounceTime2) > debounceDelay){
    if (reading2 != buttonState2){
      buttonState2 = reading2;
      if (buttonState2 == HIGH){
        counter2++;
        waveformSelect2 = fmod(counter2, 5);
      }
    }
  }
  lastButtonState2 = reading2;

  // IF statement for waveform selection based on button counter value
  if (waveformSelect1 == 0){
    analogWrite(LFO_1_OUT, sine(period1)*255);
    analogWrite(LFO_1_BLUE, sine(period1)*255);
    analogWrite(LFO_1_RED, 255);
  } else if (waveformSelect1 == 1){
    analogWrite(LFO_1_OUT, saw(period1)*255);
    analogWrite(LFO_1_BLUE, 255);
    analogWrite(LFO_1_RED, invSaw(period1)*255);
  } else if (waveformSelect1 == 2){
    analogWrite(LFO_1_OUT, tri(period1)*255);
    analogWrite(LFO_1_BLUE, tri(period1)*255);
    analogWrite(LFO_1_RED, tri(period1)*255);
  } else if (waveformSelect1 == 3){
    analogWrite(LFO_1_OUT, square(period1)*255);
    analogWrite(LFO_1_BLUE, 255);
    analogWrite(LFO_1_RED, square(period1)*255);
  } else if (waveformSelect1 == 4){
    analogWrite(LFO_1_OUT, 0);
    analogWrite(LFO_1_BLUE, 255);
    analogWrite(LFO_1_RED, 255);
  }

  if (waveformSelect2 == 0){
    analogWrite(LFO_2_OUT, sine(period2)*255);
    analogWrite(LFO_2_BLUE, sine(period2)*255);
    analogWrite(LFO_2_RED, 255);
  } else if (waveformSelect2 == 1){
    analogWrite(LFO_2_OUT, saw(period2)*255);
    analogWrite(LFO_2_BLUE, 255);
    analogWrite(LFO_2_RED, invSaw(period2)*255);
  } else if (waveformSelect2 == 2){
    analogWrite(LFO_2_OUT, tri(period2)*255);
    analogWrite(LFO_2_BLUE, tri(period2)*255);
    analogWrite(LFO_2_RED, tri(period2)*255);
  } else if (waveformSelect2 == 3){
    analogWrite(LFO_2_OUT, square(period2)*255);
    analogWrite(LFO_2_BLUE, 255);
    analogWrite(LFO_2_RED, square(period2)*255);
  } else if (waveformSelect2 == 4){
    analogWrite(LFO_2_OUT, 0);
    analogWrite(LFO_2_BLUE, 255);
    analogWrite(LFO_2_RED, 255);
  }
}


// Initiating unipolar sawtooth waveform
float saw(float period){
  float sawtoothValue = (fmod(((millis() * 255) / period), 255) / 255.0);
  return sawtoothValue;
}

// Calcuating sine wave from sawtooth waveform
float sine(float period){
  float sawValue = saw(period);
  float sineValue = ((sin((sawValue) * 2 * PI) + 1.0) / 2.0);
  return sineValue;
}

// Inverse sawtooth for triangle generation
float invSaw(float period){
  float invSawValue = (saw(period) * -1.0) + 1.0;
  return invSawValue;
}

// Calculating triangle waveform by clipping and adding sawtooth and inverse sawtooth waveform
float tri(float period){
  float triValue = ((constrain(saw(period), 0.5, 1.0) + constrain(invSaw(period), 0.5, 1.0) - 1.0) * 2.0);
  return triValue;
}

// Calcuating square waveform
float square(float period){
  float squareValue;
  float sawValue = saw(period);
  if (sawValue >= 0.5){
    squareValue = 1;
  } else {
    squareValue = 0.0;
  }
  return squareValue;
}
