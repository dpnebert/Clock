/*
 * This code has a one second timer using
 * the bool variable flag to indicate one
 * second has elapsed, and will continue
 * to do so. 
 * 
 */

/*
 * Pin mappings
 *  
 
L1    - 50
L2    - 51
L3    - 52
L4    - 53
A     - 37
B     - 36
C     - 35
D     - 34
E     - 33
F     - 32
G     - 31
Dp    - 30
button1 - 21
button2 - 20
button3 - 19
button4 - 18

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
static int MODE = 0;





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
const char characters[16]= { (char)0b11000000, //0
                             (char)0b11111001, //1
                             (char)0b10100100, //2
                             (char)0b10110000, //3
                             (char)0b10011001, //4
                             (char)0b10010010, //5
                             (char)0b10000010, //6
                             (char)0b11111000, //7
                             (char)0b10000000, //8
                             (char)0b10010000, //9
                             (char)0b10001000, //A
                             (char)0b10000011, //B
                             (char)0b11000110, //C
                             (char)0b10100001, //D
                             (char)0b10000110, //E
                             (char)0b10001110  //F
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
const int tcnt_one_second = 3036;

// The internal interrupt will count to Timer_one_second
// and set the boolean variable 'flag' to true
const int Timer_one_second = tcnt_one_second;
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

// Are we currently counting?
bool bCounting;


// Variables to hold the bit patterns to use in interface mode
int i_ones;
int i_tens;
int i_hundreds;
int i_thousands;

// For use to detect when a button is pressed
bool bButton1;
bool bButton2;
bool bButton3;
bool bButton4;






// Aqeel's code
int A = 37;    //          __A__
int B = 36;    //         |     |
int C = 35;    //         F     B
int D = 34;    //         |__G__|
int E = 33;   //          |     |
int F = 32;   //          E     C
int G = 31;   //          |__D__|
int DP = 30;  //                 *DP

int state = 0;
int delaytime = 500;
// Aqeel's code










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

      bCounting = !bCounting;
      
      bButton2 = false;
    }
    if(bButton3)
    {
      Serial.print("INC_BUTTON - ");
      Serial.println(button3 - 20);

      ones = 0;
      tens = 0;
      hundreds = 0;
      thousands = 0;
      
      bButton3 = false;
    }
    if(bButton4)
    {
      Serial.print("INC_BUTTON - ");
      Serial.println(button4 - 20);

      
      MODE = DEC_MODE;
      
      bButton4 = false;
    }


    updatePortValues(characters[ones]);
    pulseSelectLine(selectD, pulse_delay_ms);
     
    if((tens != 0) || (hundreds > 0 || thousands > 0))
    { 
      updatePortValues(characters[tens]);
      pulseSelectLine(selectC, pulse_delay_ms);  
    }
    
    if((hundreds > 0) || thousands > 0)
    {
      updatePortValues(characters[hundreds]);
      pulseSelectLine(selectB, pulse_delay_ms);  
    }
  
    if(thousands > 0)
    {
      updatePortValues(characters[thousands]);
      pulseSelectLine(selectA, pulse_delay_ms);
    }
    
  
    if(flag)
    {
      if(bCounting)
      {
               
      
        flag = false;
  
        ones++;
      
        if(ones > radixCeiling)
        {
          ones = 0;
          tens++;
        }
        if(tens > radixCeiling)
        {
          tens = 0;
          hundreds++;
        }
        if(hundreds > radixCeiling)
        {
          hundreds = 0;
          thousands++;
        }
        if(thousands > radixCeiling)
        {
          thousands = 0;
        }
      }
    }
  }



  /*
   * ============== Decrementing from radixCeiling to 0 ===============
   */ 
  else if(MODE == DEC_MODE)
  {
    if(bButton1)
    {
      Serial.print("DEC_BUTTON - ");
      Serial.println(button1 - 20);

      // Diagnostics mode
      MODE = 2;
      bButton1 = false;
    }  
    if(bButton2)
    {
      Serial.print("DEC_BUTTON - ");
      Serial.println(button2 - 20);

      bCounting = !bCounting;
      
      bButton2 = false;
    }
    if(bButton3)
    {
      Serial.print("DEC_BUTTON - ");
      Serial.println(button3 - 20);

      
      ones = 0;
      tens = 0;
      hundreds = 0;
      thousands = 0;
      
      bButton3 = false;
    }
    if(bButton4)
    {
      Serial.print("DEC_BUTTON - ");
      Serial.println(button4 - 20);

      MODE = INC_MODE;
      
      bButton4 = false;
    }


    updatePortValues(characters[ones]);
    pulseSelectLine(selectD, pulse_delay_ms);
     
    if((tens != 0) || (hundreds > 0 || thousands > 0))
    { 
      updatePortValues(characters[tens]);
      pulseSelectLine(selectC, pulse_delay_ms);  
    }
    
    if((hundreds > 0) || thousands > 0)
    {
      updatePortValues(characters[hundreds]);
      pulseSelectLine(selectB, pulse_delay_ms);  
    }
  
    if(thousands > 0)
    {
      updatePortValues(characters[thousands]);
      pulseSelectLine(selectA, pulse_delay_ms);
    }
    
  
    if(flag)
    {      
      if(bCounting)
      {
        
      
        flag = false;
      
        if(ones == 0)
        {
          ones = radixCeiling;
          tens--;
        }
        else
        {
          ones--;
        }
        if(tens < 0)
        {
          tens = radixCeiling;
          hundreds--;
        }
        if(hundreds < 0)
        {
          hundreds = radixCeiling;
          thousands--;
        }
        if(thousands < 0)
        {
          thousands = radixCeiling;
        }
      }
    }
  }

  


  
   /*
   * ============== Implementing three different diagnostic tests ===============
   * 
   *                You can edit the title and anything in the section below
   */
  else if(MODE == DIA_MODE)
  {
    
  
    updatePortValues(characters[13]);
    pulseSelectLine(selectB, pulse_delay_ms);  
    updatePortValues(0b11101111);
    pulseSelectLine(selectC, pulse_delay_ms);  
    updatePortValues(characters[10]);
    pulseSelectLine(selectD, pulse_delay_ms);  


    // Go back to INC/DEC (MODE SELECT)
    if(bButton1)
    {
      Serial.print("DIA_BUTTON - ");
      Serial.println(button1 - 20);

      if(MODE == 1)
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
      diag1();


      
      // Go back to INC/DEC (MODE SELECT)
      if(MODE == 1)
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
      diag2();

      // Go back to INC/DEC (MODE SELECT)
      if(MODE == 1)
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
      diag3();

      // Go back to INC/DEC (MODE SELECT)
      if(MODE == 1)
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
    updatePortValues(i_ones);
    pulseSelectLine(selectD, pulse_delay_ms);
    updatePortValues(i_tens);
    pulseSelectLine(selectC, pulse_delay_ms);
    updatePortValues(i_hundreds);
    pulseSelectLine(selectB, pulse_delay_ms);
    updatePortValues(i_thousands);
    pulseSelectLine(selectA, pulse_delay_ms);
  }  

  // Let's check to see if we have a interface command
  checkInterface();

}



char segA = 0b11111110;
char segB = 0b11111101;
char segC = 0b11111011;
char segD = 0b11110111;
char segE = 0b11101111;
char segF = 0b11011111;
char segG = 0b10111111;
char segDp= 0b01111111;

// Interface command parsing
void checkInterface()
{
  if(Serial.available())
  {
    selectPinsOff();
    allSegmentsOff();
    char command = Serial.read();
    if(command == 32)
    {
      MODE = INT_MODE;
      i_ones = 0;
      i_tens = 0;
      i_hundreds = 0;
      i_thousands = 0;
    }
    else if(command == 33)
    {
      MODE = INT_MODE;
      i_ones = 255;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = 255;
    }
    else if(command == 34)
    {
      diag1();
    }
    else if(command == 35)
    {
      diag2();
    }
    else if(command == 36)
    {
      diag3();
    }
    else if(command == 37)
    {      
      MODE = INC_MODE;
    }
    else if(command == 38)
    {
      MODE = DEC_MODE;
    }    
    else if(command == 39)
    {
      bCounting = !bCounting;
    }
    
    else if(command == 40)
    {
      ones = 0;
      tens = 0;
      hundreds = 0;
      thousands = 0;
    }
    
    else if(command == 'a')
    {
      i_ones = segA;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'b')
    {
      i_ones = segB;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'c')
    {
      i_ones = segC;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'd')
    {
      i_ones = segD;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'e')
    {
      i_ones = segE;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'f')
    {
      i_ones = segF;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'g')
    {
      i_ones = segG;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'h')
    {
      i_ones = segDp;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'i')
    {
      i_ones = 255;
      i_tens = segA;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'j')
    {
      i_ones = 255;
      i_tens = segB;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'k')
    {
      i_ones = 255;
      i_tens = segC;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'l')
    {
      i_ones = 255;
      i_tens = segD;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'm')
    {
      i_ones = 255;
      i_tens = segE;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'n')
    {
      i_ones = 255;
      i_tens = segF;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'o')
    {
      i_ones = 255;
      i_tens = segG;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'p')
    {
      i_ones = 255;
      i_tens = segDp;
      i_hundreds = 255;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'q')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = segA;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'r')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = segB;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 's')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = segC;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 't')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = segD;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'u')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = segE;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'v')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = segF;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'w')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = segG;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'x')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = segDp;
      i_thousands = 255;
      MODE = INT_MODE;
    }
    else if(command == 'y')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = segA;
      MODE = INT_MODE;
    }
    else if(command == 'z')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = segB;
      MODE = INT_MODE;
    }
    else if(command == '{')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = segC;
      MODE = INT_MODE;
    }
    else if(command == '|')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = segD;
      MODE = INT_MODE;
    }
    else if(command == '}')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = segE;
      MODE = INT_MODE;
    }
    else if(command == '~')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = segF;
      MODE = INT_MODE;
    }
    else if(command == '^')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = segG;
      MODE = INT_MODE;
    }
    else if(command == '_')
    {
      i_ones = 255;
      i_tens = 255;
      i_hundreds = 255;
      i_thousands = segDp;
      MODE = INT_MODE;
    }
  }
}








 /*
 * ============== Setup Code ===============
 * 
 *     We can review this if you need
 */
void setup() {
  Serial.begin(9600);
  while(!Serial){}

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

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

  bCounting = true;
  
  i_ones = 0b11111110;
  i_tens = 255;
  i_hundreds = 255;
  i_thousands = 255;
  
  initPorts();
  initSelectPins();  
  initButtons();
  initButtonInterrupts();

  MODE = INC_MODE;
  
  // Last, due to it enables global interrupts at the end
  initTimer();
}






// Turn the pin 'select' HIGH for the
// pulse_delay_ms length of time, then 
// turns 'select' back to LOW
//
void pulseSelectLine(int select, int ms)
{
  digitalWrite(select, HIGH);
  delay(ms);
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

void setNumber(int th, int h, int te, int o)
{
  ones = o;
  tens = te;
  hundreds = h;
  thousands = th;
  flag = true;
}



















void allSegmentsOff()
{
  PORTC = 255;
}

 /*
 * ============== BEGIN Diagnostics Code ===============
 */
//Aqeel's code
void diag1()
{

    selectPinsOn();
    allSegmentsOff();
    for (int i = 37 ; i > 29 ; i--)
    {
      digitalWrite(i, LOW);
      delay(delaytime);
    }
    for (int i = 37 ; i > 29 ; i--)
    {
      digitalWrite(i, HIGH);
      delay(delaytime);
    }  
    selectPinsOff();
}

void diag2()
{
    selectPinsOn();
    allSegmentsOff();
    for (int i = 0 ; i < 17 ; i++)
    {
      pickDigit(i);
      delay(delaytime);
    }
    for (int j = 16 ; j >= -1 ; j--)
    {
      pickDigit(j);
      delay(delaytime);
    }
    selectPinsOff();  
}

void diag3()
{ 
    selectPinsOn();
    pickDigit(17);
    delay(delaytime);
    pickDigit(16);
    delay(delaytime);
    pickDigit(17);
    delay(delaytime);
    pickDigit(16);
    delay(delaytime);
    pickDigit(17);
    delay(delaytime);
    pickDigit(16);
    delay(delaytime); 
    selectPinsOff();
}

void pickDigit(int digit)    //function to takes in a digit and turn on segments according to the number
{
  switch (digit)
  {
    case 0:
      digitalWrite(A, LOW);   // Turning segment A on
      digitalWrite(B, LOW);   // Turning segment B on
      digitalWrite(C, LOW);   // Turning segment C on
      digitalWrite(D, LOW);   // Turning segment D on
      digitalWrite(E, LOW);   // Turning segment E on
      digitalWrite(F, LOW);   // Turning segment F on
      digitalWrite(G, HIGH);  // Turning segment G OFF
      digitalWrite(DP, HIGH);  // Turning segment DP OFF
      break;

    case 1:
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      digitalWrite(DP, HIGH);
      break;

    case 2:
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, HIGH);
      digitalWrite(G, LOW);
      digitalWrite(DP, HIGH);
      break;

    case 3:
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      digitalWrite(E, HIGH);
      digitalWrite(F, HIGH);
      digitalWrite(G, LOW);
      digitalWrite(DP, HIGH);
      break;

    case 4:
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(DP, HIGH);

      break;

    case 5:
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      digitalWrite(E, HIGH);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(DP, HIGH);
      break;

    case 6:
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(DP, HIGH);
      break;

    case 7:
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      digitalWrite(DP, HIGH);
      break;

    case 8:
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(DP, HIGH);
      break;

    case 9:
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      digitalWrite(E, HIGH);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(DP, HIGH);
      break;

    case 10: // A
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      digitalWrite(E, LOW);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(DP, LOW);
      break;

    case 11: // B
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(DP, LOW);
      break;

    case 12: // C
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, LOW);
      digitalWrite(G, HIGH);
      digitalWrite(DP, LOW);
      break;

    case 13: // D
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, HIGH);
      digitalWrite(G, LOW);
      digitalWrite(DP, LOW);
      break;

    case 14: // E
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(DP, LOW);
      break;

    case 15: // F
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, LOW);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(DP, LOW);
      break;

    case 16: // all off
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      digitalWrite(DP, HIGH);
      break;

    case 17: // all on
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      digitalWrite(E, LOW);
      digitalWrite(F, LOW);
      digitalWrite(G, LOW);
      digitalWrite(DP, LOW);
      break;

      default:
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, HIGH);
      digitalWrite(E, HIGH);
      digitalWrite(F, HIGH);
      digitalWrite(G, HIGH);
      digitalWrite(DP, HIGH);
      break;

  }
}




 /*
 * ============== END Diagnostics Code ===============
 */
