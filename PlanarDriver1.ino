//planar motor in x and y direction, controlled by a joystick:
//in regular intervals, a check is run if a motor should be
//moved to it‘s next position
//andreas pohl 2017

//open points:
//none

//test mode
//prints to serial
const bool testMode = false;


//the 12 possible positions of the planar motor, in sequence
//default: 1 coil, 2 coils, 1 coil, ....
const byte motorState[] = {
  B100000,
  B101000,
  B001000,
  B001010,
  B000010,
  B010010,
  B010000,
  B010100,
  B000100,
  B000101,
  B000001,
  B100001
};

//variant 1: 2 coils, 2 coils, 2 coils
const byte motorState1[] = {
  B101000,
  B101000,
  B001010,
  B001010,
  B010010,
  B010010,
  B010100,
  B010100,
  B000101,
  B000101,
  B100001,
  B100001
};

//variant 2: 2 coils, 3 coils, 2 coils, 3 coils...
const byte motorState2[] = {
  B101000,
  B101010,
  B001010,
  B011010,
  B010010,
  B010110,
  B010100,
  B010101,
  B000101,
  B100101,
  B100001,
  B101001,
};

//motor speed (is not really the speed, but the numbers of ticks until the motor has 
//to be moved again. Means large absolute values are low motor speeds, and vice versa.
//special value: 0 is standstill
int speedX = 0; 
int speedY = 0;

//motor ticks
//means the motor ticks are incremented until - depending in speed - 
//the motor has to be moved to the next position
int ticksX = 0;
int ticksY = 0;

//mapping from joystick position 0..1023 
const int inToSpeed[1024] = 
{
  -4,-5,-5,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-9,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-11,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-13,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-14,-15,-15,-15,-15,-15,-15,-15,-15,-15,-15,-15,-15,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-16,-17,-17,-17,-17,-17,-17,-17,-17,-17,-17,-18,-18,-18,-18,-18,-18,-18,-18,-19,-19,-19,-19,-19,-19,-19,-19,-20,-20,-20,-20,-20,-20,-20,-20,-21,-21,-21,-21,-21,-21,-22,-22,-22,-22,-22,-22,-23,-23,-23,-23,-23,-23,-24,-24,-24,-24,-24,-25,-25,-25,-25,-26,-26,-26,-26,-26,-27,-27,-27,-27,-28,-28,-28,-29,-29,-29,-29,-30,-30,-30,-31,-31,-31,-32,-32,-32,-33,-33,-34,-34,-34,-35,-35,-36,-36,-37,-37,-37,-38,-38,-39,-40,-40,-41,-41,-42,-42,-43,-44,-44,-45,-46,-46,-47,-48,-49,-50,-51,-51,-52,-53,-54,-55,-56,-58,-59,-60,-61,-63,-64,-65,-67,-68,-70,-72,-74,-76,-78,-80,-82,-84,-87,-90,-92,-95,-99,-102,-106,-110,-114,-119,-124,-129,-136,-142,-149,-158,0,0,0,0,0,0,0,0,0,0,0,158,149,142,136,129,124,119,114,110,106,102,99,95,92,90,87,84,82,80,78,76,74,72,70,68,67,65,64,63,61,60,59,58,56,55,54,53,52,51,51,50,49,48,47,46,46,45,44,44,43,42,42,41,41,40,40,39,38,38,37,37,37,36,36,35,35,34,34,34,33,33,32,32,32,31,31,31,30,30,30,29,29,29,29,28,28,28,27,27,27,27,26,26,26,26,26,25,25,25,25,24,24,24,24,24,23,23,23,23,23,23,22,22,22,22,22,22,21,21,21,21,21,21,20,20,20,20,20,20,20,20,19,19,19,19,19,19,19,19,18,18,18,18,18,18,18,18,17,17,17,17,17,17,17,17,17,17,16,16,16,16,16,16,16,16,16,16,16,16,15,15,15,15,15,15,15,15,15,15,15,15,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,5,5,5
};

//baseTick in ms, the base intervall for motor control
//with a baseTick of 1 ms it works fine
const byte baseTick = 1;

//motor position, in 0..11
byte posX = 0; 
byte posY = 0;



void setup()
{
  for (byte pin = 2; pin < 14; pin++) {
    pinMode(pin, OUTPUT);
  }

  if (testMode) {
    Serial.begin(9600);
  }
}



//set the motor with number 0 or 1 to a position in 0..11
//if position is 255, the motor is cut off
void setMotor(byte motor, byte motorPos)
{
  byte pinset = B000000; //motor cutoff as default
  if (motorPos != 255) {
    pinset = motorState[motorPos];
  }
  
  //motor 0 pins are 2..7
  //motor 1 pins are 8..13
  byte pin0 = 2 + (motor * 6);
  byte pin5 = 7 + (motor * 6);
    
  //set pins according to pinset
  byte bitpos = B100000;
  for (byte pin = pin0; pin < (pin5 + 1); pin++) {
    if ((pinset & bitpos) == 0) {
      digitalWrite(pin, LOW);
    } else {
      digitalWrite(pin, HIGH);
    }
    bitpos = bitpos >> 1;
  }
}



//depending on speed direction, calculates the next motor position
byte nextPos(byte pos, int speed)
{
  if (speed > 0) {
    pos++;
  } else {
    pos--;
  }
  
  //handle position rollovers
  if (pos == 255) {pos = 11;}
  if (pos > 11) {pos = 0;}
  
  return pos;
}



//reads the joystick and returns the calculated speed
//motor 0 is the X motor, motor 1 the Y motor
int getSpeed(byte motor)
{
  int in = analogRead(3 + motor); //analog in pin 3 and 4
  
  //convert in range to speed range
  int speed = inToSpeed[in];
    
  return speed;
}



void loop()
{

  speedX = getSpeed(0);
  speedY = getSpeed(1);
  
  ticksX++;
  ticksY++;

  //turn off motor if not moved for some time
  if (ticksX >= 1000) {
    setMotor(0, 255);
  }
  if (ticksY >= 1000) {
    setMotor(1, 255);
  }

  //calculate if X motor has to be moved, and moves it
  if ((speedX != 0) and (ticksX >= abs(speedX))) {
    posX = nextPos(posX, speedX);
    setMotor(0, posX);
    ticksX = 0;
  }
  
  //calculate if Y motor has to be moved, and moves it
  if ((speedY != 0) and (ticksY >= abs(speedY))) {
    posY = nextPos(posY, speedY);
    setMotor(1, posY);
    ticksY = 0;
  }

  if (testMode) {
    Serial.print("posX:");
    Serial.print(posX);
    Serial.print(" posY:");
    Serial.print(posY);
    
    Serial.print(" speedX:");
    Serial.print(speedX);
    Serial.print(" speedY:");
    Serial.println(speedY);
  }
  
  delay(baseTick); // Wait for next tick
}
