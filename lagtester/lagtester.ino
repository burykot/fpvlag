// define pins
const int cameraPin = 0;
const int monitorPin = 1;
const int buttonPin = A2;
const int ledPin = 3;

// timing for LED intervals
unsigned long currentMillisLed = 0;
unsigned long startLedTime = 0;
long OnTime = 250;           // milliseconds of on-time
long OffTime = 1000;          // milliseconds of off-time

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

int debugMode = false; // set debug mode, used to output RAW values to serial
int debugModeLength = 6; // lazy
int debugModeArray[6] = {cameraState,monitorState,buttonState,cameraTime,monitorTime,timeDifference}; // define printed values in debug mode

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
  
  /*
  Serial.print("Camera state ");
  Serial.print(cameraState);
  Serial.print("\t");
  Serial.print("Monitor state" monitorState);
  Serial.print("\n");
  delay(300);
  */
  
  //  run core functions
  keepFlashing();
  measureLag(); 
  if (debugMode)
  {
    printDebugInfo();
  }
}

// checks button state and operates LED / High pin should be set to 5v (100% uptime?), use cap if flickers - check if it slows down the LED. Use longer offTime if needed.
void keepFlashing()
{
  currentMillisLed = millis();
  if (buttonState == HIGH && currentMillisLed - startLedTime >= OnTime)
  {
    startLedTime = currentMillisLed;
    digitalWrite(ledPin, LOW); // if ON time exceeded, disable led
  }
  else if (buttonState == HIGH && currentMillisLed - startLedTime >= OffTime)
  {
    startLedTime = currentMillisLed;
    digitalWrite(ledPin, HIGH); // if OFF time exceeded, enable led
  }
  else if (buttonState == LOW && ledPin == HIGH)
  {
    digitalWrite(ledPin, LOW); // if led is lit and button not pressed, disable led
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
  Serial.print(" || ");
  Serial.print(monitorTime);
  Serial.print(" || ");
  Serial.print(cameraTime);
  Serial.print(" || ");
  Serial.print(timeDifference);
  delay(1250);
  
  if (timeDifference > 0 && timeDifference < 500) // I don't expect more lag, rejecting duration above 500ms. Also prevents output when dimming (below 0).
  {
    Serial.print(timeDifference);
    return timeDifference;
  }
};

// Debug mode: sends sensor information to Serial. Order of values defined in debugModeArray
void printDebugInfo()
{
  for (int i = 0; i <= debugModeLength; i = i++)
  {
    Serial.print( debugModeArray[i] );
    Serial.print("\t");
  }
}
