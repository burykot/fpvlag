// define pins
const int cameraPin = 2;
const int monitorPin = 3;

// timings for sensors
unsigned long cameraTime = 0; // stores camera milis
unsigned long monitorTime = 0; // stores monitor milis
unsigned long timeDifference = 0; // stores actual LAG
unsigned long cameraTimeSet = 0;
unsigned long monitorTimeSet = 0;

// initialize sensor ints
int cameraState = 0;
int monitorState = 0;

void setup() {
// set pins modes
  pinMode(cameraPin, INPUT);
  pinMode(monitorPin, INPUT);

//  open serial port
  Serial.begin(9600);
}

void loop() {
  // read values
  cameraState = digitalRead(cameraPin);
  monitorState = digitalRead(monitorPin);
  
  if (cameraState == 0 && cameraTimeSet != 1) {
    cameraTime = millis(); // set time of camera flash
    cameraTimeSet = 1;
    monitorTimeSet = 0;
    monitorCheck();
    } else if (cameraTimeSet == 1 && monitorTimeSet != 1) {
       monitorCheck();
    } else if (cameraTimeSet == 1 && monitorTimeSet == 1) {
       timeDifference = monitorTime - cameraTime;
       if (timeDifference > 2 && timeDifference < 500) { // 1-2ms is the script lag
          timeDifference = timeDifference -1; // substract script lag, most frequently 1ms
          Serial.print("Time difference: ");
          Serial.print(timeDifference);
          Serial.print("\n");
          reset();
          delay(500);
          Serial.println("Start again!");
       } else {
          reset();
       }
    } else {
       reset();
    }
}

int monitorCheck(){
   if (monitorState == 0 && monitorTimeSet != 1) {
      monitorTime = millis(); // set time of monitor flash
      monitorTimeSet = 1;
      return monitorTime;
      return monitorTimeSet;
      } else {
      monitorTime = 0; //  reset if reading missed
      monitorTimeSet = 0;
      return monitorTime;
      return monitorTimeSet;
   }
   return monitorTime;
   return monitorTimeSet;
}

int reset() {
    timeDifference = 0;
    cameraTimeSet = 0;
    monitorTimeSet = 0;
    cameraTime = 0;
    monitorTime = 0;
    return timeDifference;
    return cameraTimeSet;
    return monitorTimeSet;
    return cameraTime;
    return monitorTime;
}
// measures lag between camera and screen flashes
