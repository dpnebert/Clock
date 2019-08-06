/*
 * This code has a one second timer using
 * the bool variable flag to indicate one
 * second has elapsed, and will continue
 * to do so.
 * 
 * It uses PORTB and PORTC for the display.
 * Four bits to the lower nibble of PORTB and
 * four bits to the lower nibble of PORTC
 * Select pins are 2, 3, 4, 5
 * 
 */
// Can't be used with buttons
//#define timerClock    6

#define selectA       50
#define selectB       51  
#define selectC       52  
#define selectD       53

#define button1       21
#define button2       22
#define button3       23
#define button4       24

#define INC_MODE      0
#define DEC_MODE      1
#define DIA_MODE      2
#define INT_MODE      3
int MODE = 0;

/*            A
 *          F   B
 *            G
 *          E   C
 *            D
 *                Dp
 *          
 *          
 *          
 *          
 *         8  7654321 
 *      0b(Dp)GFEDCBA
 *      
 *      MUST MAKE BIT ZERO
 *      TO TURN ON LED.
 */

// Setting constant for one second
// Doing math on this number can give
// you any timer delay you want
//
// The timer delay is calculated using
// the speed of the microcontroller
// and the programmed prescaler.
// To get more precision or ability
// for longer delays, modify formula:
// 3036 = 65536 - (16MHz/256/1Hz)
// http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
const uint8_t tcnt_one_second = 3036;


                          //   D
                          //   pGFEDCBA
const char characters[16]= { 0b11000000, //0
                             0b11111001, //1
                             0b10100100, //2
                             0b10110000, //3
                             0b10011001, //4
                             0b10010010, //5
                             0b10000010, //6
                             0b11111000, //7
                             0b10000000, //8
                             0b10011000, //9
                             0b10001000, //A
                             0b10000011, //B
                             0b11000110, //C
                             0b10100001, //D
                             0b10000110, //E
                             0b10001110  //F
                           };
                       
const char displays[] = { selectA,
                          selectB,
                          selectC,
                          selectD };

                          
const int Timer_one_second = 3036;
int tick_rate;
int delay_ms;
bool flag;
int radixCeiling;
int ones;
bool onesDecimal;
int tens;
int hundreds;
int thousands;
int incrementDir;
bool counting;

bool bButton1;
bool bButton2;
bool bButton3;
bool bButton4;


void loop()
{

  /*
   * 
   * 
   * Button 1 is connected to pin 21
   * Button 2 is connected to pin 22
   * Button 3 is connected to pin 23
   * Button 4 is connected to pin 24
   * 
   * When the button X goes from HIGH to LOW:
   *  bButtonX becomes true
   * When the button X goes from LOW to HIGH:
   *  bButtonX becomes false
   * 
   * We can copy and paste the below block of code in
   * to  So if
   * you want the buttons to function differently
   * depending on if we are in INCREMENT mode or
   * DIAGNOSTIC mode, we can just specify 
   * 
   */
   
  
  
  
  if(MODE == INC_MODE)
  {

    if(bButton1)
    {
      Serial.print("INC_BUTTON - ");
      Serial.println(button1 - 20);
      
      MODE = 2;
      bButton1 = false;
    }  
    if(bButton2)
    {
      Serial.print("INC_BUTTON - ");
      Serial.println(button2 - 20);
      bButton2 = false;
    }
    if(bButton3)
    {
      Serial.print("INC_BUTTON - ");
      Serial.println(button3 - 20);
      bButton3 = false;
    }
    if(bButton4)
    {
      Serial.print("INC_BUTTON - ");
      Serial.println(button4 - 20);
      bButton4 = false;
    }





    
    updatePortValues(characters[ones]);
    pulseSelectLine(selectD);
     
    if((tens != 0) || (hundreds > 0 || thousands > 0))
    { 
      updatePortValues(characters[tens]);
      pulseSelectLine(selectC);  
    }
    
    if((hundreds > 0) || thousands > 0)
    {
      updatePortValues(characters[hundreds]);
      pulseSelectLine(selectB);  
    }
  
    if(thousands > 0)
    {
      updatePortValues(characters[thousands]);
      pulseSelectLine(selectA);
    }
    
  
    if(flag)
    { 
      // toggle pin for scope to see
      // the timer's clock
      //PORTD ^= 0b01000000; 
  
      
      flag = false;

    
      if(ones == radixCeiling)
      {
        ones = 0;
        tens++;
      }
      else
      {
        ones++;
      }
      if(tens == radixCeiling)
      {
        tens = 0;
        hundreds++;
      }
      if(hundreds == radixCeiling)
      {
        hundreds = 0;
        thousands++;
      }
      if(thousands == radixCeiling)
      {
        thousands = 0;
      }
    }
  }
  else if(MODE == DEC_MODE)
  {
    
  }
  else if(MODE == DIA_MODE)
  {
    if(Serial.available())
    {
      char command = Serial.read();
      if(command == 33)
      {
       sequenceOn();
      }
    }

    
    if(bButton1)
    {
      Serial.print("DIA_BUTTON - ");
      Serial.println(button1 - 20);

      sequenceOn();
      MODE = 0;
      
      bButton1 = false;
    }  
    if(bButton2)
    {
      Serial.print("DIA_BUTTON - ");
      Serial.println(button2 - 20);
      bButton2 = false;
    }
    if(bButton3)
    {
      Serial.print("DIA_BUTTON - ");
      Serial.println(button3 - 20);
      bButton3 = false;
    }
    if(bButton4)
    {
      Serial.print("DIA_BUTTON - ");
      Serial.println(button4 - 20);
      bButton4 = false;
    }



    
  }
  else if(MODE == INT_MODE)
  {
    
    if(Serial.available())
    {
      char command = Serial.read();
      if(command == 33)
      {
       digitalWrite(7, !digitalRead(7));
      }
    }
  }
}


void setup() {
  Serial.begin(9600);
  while(!Serial){}

  bButton1 = false;
  bButton2 = false;
  bButton3 = false;
  bButton4 = false;
  
  tick_rate = Timer_one_second;
  
  radixCeiling = 15;
  delay_ms = 3;
  flag = false;
  ones = 0;
  onesDecimal = true;
  tens = 0;
  hundreds = 0;
  thousands = 0;
  incrementDir = 1;
  counting = true;
  
  initPorts();
  initSelectPins();  
  initButtons();
  initButtonInterrupts();

  // Last, due to it enables global interrupts at the end
  initTimer();
}



// Turn the pin 'select' HIGH for the
// delay_ms length of time, then 
// turns 'select' back to LOW
//
void pulseSelectLine(int select)
{
  digitalWrite(select, HIGH);
  delay(delay_ms);
  digitalWrite(select, LOW);
}

// We give it eight bits, and this
// method reads the lower half into
// PORTB, and the upper in to PORTC
void updatePortValues(char character)
{  
  PORTC = character;
}


ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  TCNT1 = tick_rate;            // preload timer
  flag = true;
}
void initTimer()
{
  // http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
  char cSREG;
  cSREG = SREG;             // store SREG value
  cli();                    // disable all interrupts
  SREG = cSREG;             // restore SREG value (I-bit)
    
  TCCR1A = 0;               // Zero out the timer control
  TCCR1B = 0;               // registers

  TCNT1 = tick_rate;        // preload timer 65536-16MHz/256/1Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  sei();                    // enable all interrupts

}

void initPorts()
{
  DDRC = 0b11111111;
  PORTC = 0b11111111;
}
void initSelectPins()
{
  pinMode(selectD, OUTPUT);
  pinMode(selectC, OUTPUT);
  pinMode(selectB, OUTPUT);
  pinMode(selectA, OUTPUT);
  selectPinsOff();
}
void selectPinsOff()
{
  digitalWrite(selectD, LOW);
  digitalWrite(selectC, LOW);
  digitalWrite(selectB, LOW);
  digitalWrite(selectA, LOW);
}
void selectPinsOn()
{
  digitalWrite(selectD, HIGH);
  digitalWrite(selectC, HIGH);
  digitalWrite(selectB, HIGH);
  digitalWrite(selectA, HIGH);
}
void initButtons()
{ 
  pinMode(button1, INPUT_PULLUP);  
  pinMode(button2, INPUT_PULLUP);  
  pinMode(button3, INPUT_PULLUP);  
  pinMode(button4, INPUT_PULLUP);  
}
void initButtonInterrupts()
{
  attachInterrupt(digitalPinToInterrupt(button1), button1_Pressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(button2), button2_Pressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(button3), button3_Pressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(button4), button4_Pressed, FALLING);
}
void button1_Pressed()
{
  bButton1 = !bButton1;
}
void button2_Pressed()
{
  bButton2 = !bButton2;
}
void button3_Pressed()
{
  bButton3 = !bButton3;
}
void button4_Pressed()
{
  bButton4 = !bButton4;
}

void setNumber(int o, int te, int h, int th)
{
  ones = o;
  tens = te;
  hundreds = h;
  thousands = th;
  flag = true;
}


//Aqeel's code

void portSequence()
  {
    int delaytime = 500;
    digitalWrite(37, HIGH);
    delay(delaytime);
    digitalWrite(36, HIGH);
    delay(delaytime);
    digitalWrite(35, HIGH);
    delay(delaytime);
    digitalWrite(34, HIGH);
    delay(delaytime);
    digitalWrite(33, HIGH);
    delay(delaytime);
    digitalWrite(32, HIGH);
    delay(delaytime);
    digitalWrite(31, HIGH);
    delay(delaytime);
    digitalWrite(30, HIGH);
    delay(delaytime);

    digitalWrite(37, LOW);
    delay(delaytime);
    digitalWrite(36, LOW);
    delay(delaytime);
    digitalWrite(35, LOW);
    delay(delaytime);
    digitalWrite(34, LOW);
    delay(delaytime);
    digitalWrite(33, LOW);
    delay(delaytime);
    digitalWrite(32, LOW);
    delay(delaytime);
    digitalWrite(31, LOW);
    delay(delaytime);
    digitalWrite(30, LOW);
    delay(delaytime);
    
  }

void sequenceOn()
{
  selectPinsOn();
  portSequence();
  selectPinsOff();
}
