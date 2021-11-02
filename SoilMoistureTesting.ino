/*

*/

const int sensorPower = 7;
const int sensorIn = A0;
const int motorCont = 10;
const int above_range = 50;
const int below_range = 50;
float calib_ideal_val;

void setup() {
    pinMode(sensorPower, OUTPUT);
    pinMode(sensorIn, INPUT);
    pinMode(motorCont, OUTPUT);
    Serial.begin(9600);
    calibrate();
}

int readSensor()
{
  digitalWrite(sensorPower, HIGH);
  delay(10);
  int val = analogRead(sensorIn);
  digitalWrite(sensorPower, LOW);
  return val;
}

//dry -- 928.4
//ideal -- 197.1
//wet -- 169.6
void calibrate()
{
  Serial.println("Place sensor in the driest possible soil");
  while(Serial.available()==0)
  {}
  Serial.read();
  
  float dry_val = 0;
  Serial.println("Reading Data");
  for(int i=1;i<=200;i++)
  {
    dry_val += readSensor();
    delay(10);
  }
  dry_val/=200;
  Serial.print("Driest value ");
  Serial.println(dry_val);
  delay(500);
  
  Serial.println("Place sensor in ideal soil conditions");
  while(Serial.available()==0)
  {}
  Serial.read();
  
  float ideal_val = 0;
  Serial.println("Reading Data");
  for(int i=1;i<=200;i++)
    ideal_val += readSensor();
  ideal_val/=200;
  
  calib_ideal_val = ideal_val;
  
  Serial.print("Ideal value ");
  Serial.println(ideal_val);
  delay(500);
  
  Serial.println("Place sensor in the wettest possible soil");
  while(Serial.available()==0)
  {}
  Serial.read();
  
  float wet_val = 0;
  Serial.println("Reading Data");
  for(int i=1;i<=200;i++)
    wet_val += readSensor();
  wet_val/=200;
  
  Serial.print("Wet value ");
  Serial.println(wet_val);
  delay(500);
  
  Serial.println("Calibration complete. Exiting setup");
  delay(500);
  
}

void water_control(float ideal)
{
  int current;
  current = readSensor();
  Serial.println(current);
  // soil is dry -- value is above ideal
  if (current > (ideal + above_range) )
  {
    Serial.println("Soil is too dry");
    
    digitalWrite(motorCont, HIGH);
    
    while(current > (ideal + above_range))
    {
      current = readSensor();
      Serial.println(current);
      delay(50);
    }
    digitalWrite(motorCont, LOW);
  }
  
  //soil is too wet -- value is less than ideal
  if(current < (ideal - below_range))
  {
    Serial.println("Soil is too wet");
    Serial.println(current);
    digitalWrite(motorCont, LOW);
  }
}
void loop() {
    // int val = readSensor();
    // Serial.println(val);
    // delay(1000);
    
    water_control(calib_ideal_val);
    delay(1000);
}
