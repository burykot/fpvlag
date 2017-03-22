// define pins
const int cameraPin = 0;
const int monitorPin = 1;
const int buttonPin = 2;
const int ledPin = A3;

// timing for LED intervals
unsigned long currentMillisLed = 0;
unsigned long previousMillisLed = 0;
long interval = 500;           // duration of interval

// timings for sensors
unsigned long currentMillisCamera = 0;
unsigned long currentMillisMonitor = 0;
unsigned long cameraTime = 0; // stores camera milis
unsigned long monitorTime = 0; // stores monitor milis
unsigned long timeDifference = 0; // stores actual LAG

// initialize sensor ints, set sensitivity, idea is to grab light when LED is rising, use inverted logic - photodiode signal is pulling down when LED lights up
// /////////////////////// REMEMBER: have to equalize resistance on both sensors, then set both Min & Max values equal or use one pair
int cameraState = 0;
int cameraMin = 600;
int cameraMax = 800;
int monitorState = 0;
int monitorMin = 600;
int monitorMax = 800;
int buttonState; // default button value

// other:
int ledState = 0;

void setup() {
// set pins modes
  pinMode(cameraPin, INPUT);
  pinMode(monitorPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

//  open serial port
  Serial.begin(9600);
}

void loop()
{
  // read values
  buttonState = digitalRead(buttonPin);
  cameraState = analogRead(cameraPin);
  monitorState = analogRead(monitorPin);
  
  //  run core functions
  switch (buttonState){
    case 1:
      flash();
    default:
      digitalWrite(ledPin, 0); // bail out - disable led
  }
  
  measureLag(); 
 
}

// checks button state and operates LED / High pin should be set to 5v (100% uptime?), use cap if flickers - check if it slows down the LED. Use longer offTime if needed.
void flash()
  {
    currentMillisLed = millis();
    if(currentMillisLed - previousMillisLed > interval)
    {
      ledState = digitalRead(ledPin);
      switch(ledState) {
        case 1:
          digitalWrite(ledPin, 0);
          previousMillisLed = millis();
        default:
          digitalWrite(ledPin, 1);
          previousMillisLed = millis();
      }
    }
}

// measures lag between camera and screen flashes
int measureLag()
{
  currentMillisCamera = millis();
  if (cameraState > cameraMin && cameraState < cameraMax && buttonState == HIGH)
  {
    cameraTime = currentMillisCamera; // set time of camera flash
  }
  else
  {
    cameraTime = 0; // reset if reading missed / needed?
  }
  
  currentMillisMonitor = millis();
  if (monitorState > monitorMin && monitorState < monitorMax && buttonState == HIGH)
  {
    monitorTime = currentMillisMonitor; // set time of monitor flash
  }
  else
  {
    monitorTime = 0; //reset if reading missed / needed?
  }
  
  timeDifference = monitorTime - cameraTime;
  /*Serial.print(" || ");
  Serial.print(monitorTime);
  Serial.print(" || ");
  Serial.print(cameraTime);
  Serial.print(" || ");
  Serial.print(timeDifference);
  delay(1250);*/
  
  if (timeDifference > 0 && timeDifference < 500) // I don't expect more lag, rejecting duration above 500ms. Also prevents output when dimming (below 0).
  {
    Serial.print(timeDifference);
    return timeDifference;
  }
};
