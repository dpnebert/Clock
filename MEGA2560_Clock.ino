/*
 * This code has a one second timer using
 * the bool variable flag to indicate one
 * second has elapsed, and will continue
 * to do so. 
 * 
 */

/*
 * ============== Making numbers that don't change human readable ===============
 */
// Making pin numbers human readable
#define selectA       50
#define selectB       51  
#define selectC       52  
#define selectD       53

// Making pin numbers human readable
#define button1       21
#define button2       20
#define button3       19
#define button4       18

// Making mode select number human readable
#define INC_MODE      0
#define DEC_MODE      1
#define DIA_MODE      2
#define INT_MODE      3

// For testing, setting this will allow
// you to work on a single componet.
// We'll hook this up to a button also
int MODE = 0;





/*
 * ============== Creating our characters and select line arrays ===============
 */
/*            A
 *          F   B
 *            G
 *          E   C
 *            D   Dp
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





/*
 * ============== Our global variables we want to use the whole life span of the program ===============
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

// The internal interrupt will count to Timer_one_second
// and set the boolean variable 'flag' to true
const int Timer_one_second = 3036;
bool flag;

// We dont want to change Timer_one_second,
// instead we want a variable we can change
// by doing math on Timer_one_second
int tick_rate;

// This is how long each 7 seg display will be on
int pulse_delay_ms;

// We can change the number we count up to by
// changing radixCeiling
int radixCeiling;

// Variables to hold the hexadecimal character being displayed
int ones;
int tens;
int hundreds;
int thousands;

// 1 to increment, -1 to decrement
int incrementDir;

// For use to detect when a button is pressed
bool bButton1;
bool bButton2;
bool bButton3;
bool bButton4;

/*
 * ============== ===============
 */
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
   
  
  
  /*
   * ============== Incrementing from 0 to radixCeiling ===============
   */
  if(MODE == INC_MODE)
  {

    if(bButton1)
    {
      Serial.print("INC_BUTTON - ");
      Serial.println(button1 - 20);

      // Diagnostics mode
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



  /*
   * ============== Decrementing from radixCeiling to 0 ===============
   */ 
  else if(MODE == DEC_MODE)
  {
    // todo
  }








  
   /*
   * ============== Implementing three different diagnostic tests ===============
   * 
   *                You can edit the title and anything in the section below
   */
  else if(MODE == DIA_MODE)
  {
  
    updatePortValues(characters[13]);
    pulseSelectLine(selectB);  
    updatePortValues(0b111001111);
    pulseSelectLine(selectC);  
    updatePortValues(characters[10]);
    pulseSelectLine(selectD);  


    // Go back to INC/DEC (MODE SELECT)
    if(bButton1)
    {
      Serial.print("DIA_BUTTON - ");
      Serial.println(button1 - 20);

      if(incrementDir == 1)
      {
        MODE = 0;
      }
      else
      {
        MODE = 1;
      }
      bButton1 = false;
    }

    // TEST 1
    if(bButton2)
    {
      Serial.print("DIA_BUTTON - ");
      Serial.println(button2 - 20);

      // doStuff
      sequenceOn();

      // Go back to INC/DEC (MODE SELECT)
      if(incrementDir == 1)
      {
        MODE = 0;
      }
      else
      {
        MODE = 1;
      }
      
      bButton2 = false;
    }

    // TEST 2
    if(bButton3)
    {
      Serial.print("DIA_BUTTON - ");
      Serial.println(button3 - 20);

      // doStuff

      // Go back to INC/DEC (MODE SELECT)
      if(incrementDir == 1)
      {
        MODE = 0;
      }
      else
      {
        MODE = 1;
      }
      
      bButton3 = false;
    }

    // TEST 3
    if(bButton4)
    {
      Serial.print("DIA_BUTTON - ");
      Serial.println(button4 - 20);

      // doStuff

      // Go back to INC/DEC (MODE SELECT)
      if(incrementDir == 1)
      {
        MODE = 0;
      }
      else
      {
        MODE = 1;
      }
      
      bButton4 = false;
    }    
  }
   /*
   * ============== Implementing three different diagnostic tests ===============
   * 
   *                End diagnostics block
   */








   /*
   * ============== Interface mode.  Allows display to be completly controlled by serial communication ===============
   */
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

 /*
 * ============== BEGIN Diagnostics Code ===============
 */
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
 /*
 * ============== END Diagnostics Code ===============
 */








 /*
 * ============== Setup Code ===============
 * 
 *     We can review this if you need
 */
void setup() {
  Serial.begin(9600);
  while(!Serial){}

  bButton1 = false;
  bButton2 = false;
  bButton3 = false;
  bButton4 = false;
  
  tick_rate = Timer_one_second;
  
  radixCeiling = 15;

  pulse_delay_ms = 3;
  
  flag = false;
  
  ones = 0;
  tens = 0;
  hundreds = 0;
  thousands = 0;
  
  incrementDir = 1;
  
  initPorts();
  initSelectPins();  
  initButtons();
  initButtonInterrupts();

  // Last, due to it enables global interrupts at the end
  initTimer();
}



// Turn the pin 'select' HIGH for the
// pulse_delay_ms length of time, then 
// turns 'select' back to LOW
//
void pulseSelectLine(int select)
{
  digitalWrite(select, HIGH);
  delay(pulse_delay_ms);
  digitalWrite(select, LOW);
}

// In one, very, very efficient swoop,
// we essentially assign:
//    PORTC[0] = character[0]
//    PORTC[1] = character[1]
//    PORTC[2] = character[2]
//    PORTC[3] = character[3]
//    PORTC[4] = character[4]
//    PORTC[5] = character[5]
//    PORTC[6] = character[6]
//    PORTC[7] = character[7]
void updatePortValues(char character)
{  
  PORTC = character;
}

// Interrupt vector triggered by TIMER1
ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  // Update the tick_rate here incase
  // we speed up or slow down the clock
  TCNT1 = tick_rate;            // preload timer
  flag = true;
}

// Good thing for datasheets and people smarter than I
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

// Now we are only using PORTC to control the segments
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
