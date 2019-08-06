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
#define selectA       5
#define selectB       4  
#define selectC       3  
#define selectD       2

#define inputPinA     6
#define inputPinB     7

#define INC_MODE      0
#define DEC_MODE      1
#define DIA_MODE      2
#define INT_MODE      3
int MODE = 2;

//Aqeel's varialbles

int state = 0;
int push1 = 14;
int push2 = 15;
int push3 = 16;


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

const char inputButtons[] = { inputPinA, inputPinB };
                          
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



void loop() {  
  if(MODE == INC_MODE)
  {
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
      PORTD ^= 0b01000000; 
  
      
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
        
        PORTD ^= 0b10000000; 
       sequenceOn();
      }
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

void button1()
{
  
}












void incrementDisplay(int *ones, int *tens, int *hundreds, int *thousands)
{
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


// ALL THE CODE BELOW HERE IS SETUP
//
// PLEASE DON'T CHANGE ANYTHING
// 
// Just read the comments

// Setup look to call all the initialization methods
void setup() {
  Serial.begin(9600);
  while(!Serial){}

  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  
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

  //pinMode(timerClock, OUTPUT);
  //digitalWrite(timerClock, LOW);
  
  initPorts();
  initSelectPins();  
  initButtons();
  initButtonInterrupts();

  // Last due to it enables global interrupts at the end
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
  PORTB = character & 0b00001111; // lower
  PORTC = (character >> 4); // uppper
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
  cSREG = SREG; /* store SREG value */
  cli();                    // disable all interrupts
  SREG = cSREG; /* restore SREG value (I-bit) */
    
  TCCR1A = 0;               // Zero out the timer control
  TCCR1B = 0;               // registers

  TCNT1 = tick_rate;             // preload timer 65536-16MHz/256/1Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  sei();                    // enable all interrupts

}
void initPorts()
{
  DDRB = 255;
  DDRC = 255;

  PORTB = 255;
  PORTC = 255;

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
  pinMode(inputButtons[0], OUTPUT);
  digitalWrite(inputButtons[0], LOW);
  pinMode(inputButtons[1], OUTPUT);
  digitalWrite(inputButtons[1], LOW);
}
void initButtonInterrupts()
{
  attachInterrupt(digitalPinToInterrupt(2), button1, RISING);
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
    digitalWrite(8, HIGH);
    delay(delaytime);
    digitalWrite(9, HIGH);
    delay(delaytime);
    digitalWrite(10, HIGH);
    delay(delaytime);
    delay(delaytime);
    digitalWrite(11, HIGH);
    delay(delaytime);
    digitalWrite(14, HIGH);
    delay(delaytime);
    digitalWrite(15, HIGH);
    delay(delaytime);
    digitalWrite(16, HIGH);
    delay(delaytime);
    digitalWrite(17, HIGH);
    delay(delaytime);
    
    digitalWrite(8, LOW);
    delay(delaytime);
    digitalWrite(9, LOW);
    delay(delaytime);
    digitalWrite(10, LOW);
    delay(delaytime);
    digitalWrite(11, LOW);
    delay(delaytime);
    digitalWrite(14, LOW);
    delay(delaytime);
    digitalWrite(15, LOW);
    delay(delaytime);
    digitalWrite(16, LOW);
    delay(delaytime);
    digitalWrite(17, LOW);
    delay(delaytime);
    
  }

void sequenceOn()
{
  selectPinsOn();
  portSequence();
  selectPinsOff();
}
