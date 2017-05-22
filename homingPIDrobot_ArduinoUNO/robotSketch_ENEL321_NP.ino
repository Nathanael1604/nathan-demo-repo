// ENEL321 Assignment Arduino sketch.
//   Provides functions for reading the sensors and 
//   setting the motor speeds. Contains all necessary 
//   boilerplate code so you can focus on the controller.

#define STOP_CODE 1

// Parameters
long Kp = 5, Ki = 0.5, Kd = 2;
long leftBaseSpeed = 240, rightBaseSpeed = 220;
long prevError = 0, prevDirError = 0, dirErrorCount = 0;
int blackFlag = 0, dirFlag = 0;

// Arduino setup function.
void setup()
{
  motorsInit();
  sensorsInit();
  Serial.begin(9600);
}

// Arduino main loop function.
void loop()
{
  // Get sensor data.
  long l, m, r;
  sensorsRead(&l, &m, &r);
  
#if STOP_CODE  
  if (m < 430)
  {
    blackFlag = 1;
  }
  else
  {
    blackFlag = 0;
  }
#endif
  //Serial.println(r);
  //Serial.println(m);
  //Serial.println(l);

  // ***************************
  // Control algorithm goes here.
  // ***************************
  long error = l - r;
  //long dirError = (l+m+r)/3;
  //Serial.println(error);
  long integral = integral + error; // long integral = integral + error*dt;
  long derivative = error - prevError; // long derivative = (error - prevError)/dt;
  long PID_Output = (Kp*error) + (Ki*integral) + (Kd*derivative);
  prevError = error;
  //Serial.println(PID_Output);
  //Serial.println(error);
  
  /*if (dirError > prevDirError)
  {
    dirErrorCount++;
  }
  else
  {
    dirErrorCount = 0;
  }*/

  long rightMotorSpeed = rightBaseSpeed - PID_Output;
  long leftMotorSpeed = leftBaseSpeed + PID_Output;
  //Serial.println(rightMotorSpeed);
  
  if (rightMotorSpeed > 250)
  {
    rightMotorSpeed = 250;
  }
  else if (rightMotorSpeed < -250)
  {
    rightMotorSpeed = -250;
  }
  
  if (leftMotorSpeed > 250)
  {
    leftMotorSpeed = 250;
  }
  else if (leftMotorSpeed < -250)
  {
    leftMotorSpeed = -250;
  }
  
  if ((rightMotorSpeed < 50) && (leftMotorSpeed < 50))
  {
    rightMotorSpeed = 50;
    leftMotorSpeed = 50;
  }
  
  if (blackFlag == 1)
  {
    motorsWrite(0,0);
  }
  /*else if (dirErrorCount > 200)
  {
    dirErrorCount = 0;
    // Robot reverse manuveure
    reverseManuveure();
  }*/
  else if ((error < 25) && (error > -40))
  {
    motorsWrite(250, 250);
  }
  else
  {
    motorsWrite(leftMotorSpeed, rightMotorSpeed);
  }
  
  // Set the rate of the controller here.
  // The delay function takes a value in milliseconds.
  delay(100);
}

// Initialise the motor pins.
void motorsInit()
{
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void reverseManuveure()
{
    motorsWrite(-250, 250);
    delay(650);
    motorsWrite(0, 0);
    delay(100);
}

// Control the motors.
//   Takes a value between -255 and 255 for each motor and sets the 
//   motor speed and direction based on this value.
void motorsWrite(int lMotorValue, int rMotorValue)
{

  if (lMotorValue >= 0)
  {
    digitalWrite(11, LOW);
    analogWrite(10, lMotorValue);
  }
  else
  {
    digitalWrite(10, LOW);
    analogWrite(11, -lMotorValue);
  }
  
  if (rMotorValue >= 0)
  {
    digitalWrite(3, LOW);
    analogWrite(9, rMotorValue);
  }
  else
  {
    digitalWrite(9, LOW);
    analogWrite(3, -rMotorValue);
  }

}


// Variables for storing calibration data.
long lCalibration;
long mCalibration;
long rCalibration;

// Initialise sensors, collect calibration data.
void sensorsInit()
{
  lCalibration = analogRead(A3);
  mCalibration = analogRead(A4);
  rCalibration = analogRead(A5); 
}

// Read the sensors.
//   Takes a left, middle,and right pointer and writes calibrated
//   sensor data into the variables they point to.
//   The sensors return values from approximately 1000, for white, to less than 100 for black.
void sensorsRead(long* l, long* m, long* r)
{
  long lRaw = analogRead(A3);
  long mRaw = analogRead(A4);
  long rRaw = analogRead(A5);
  
  *l = lRaw * 1000 / lCalibration;
  *m = mRaw * 1000 / mCalibration;
  *r = rRaw * 1000 / rCalibration;
  
}


