import controlP5.*;
import processing.serial.*;

Serial port;

ControlP5 allOnButton;
ControlP5 allOffButton;
ControlP5 increment;
ControlP5 decrement;
ControlP5 incrementSeg;
ControlP5 decrementSeg;
ControlP5 diagnosticTest1;
ControlP5 diagnosticTest2;
ControlP5 diagnosticTest3;


ControlP5 segmentA;
ControlP5 segmentB;
ControlP5 segmentC;
ControlP5 segmentD;
PFont font;

void setup()
{
  size(1200, 900);
  
  printArray(Serial.list());
  
  // Open the port you are using at the rate you want:
  port = new Serial(this, Serial.list()[0], 9600);
  
 
  
  allOnButton = new ControlP5(this);
  allOffButton = new ControlP5(this);
  increment = new ControlP5(this);
  decrement = new ControlP5(this);
  incrementSeg = new ControlP5(this);
  decrementSeg = new ControlP5(this);
  diagnosticTest1 = new ControlP5(this);
  diagnosticTest2 = new ControlP5(this);
  diagnosticTest3 = new ControlP5(this);
  
  segmentA = new ControlP5(this);
  segmentB = new ControlP5(this);
  segmentC = new ControlP5(this);
  segmentD = new ControlP5(this);
  font = createFont("calibri light", 11);
  
  drawDecrementSeg(200, 400, 100, 150);
  drawIncrementSeg(400, 400, 100, 150);
  
  drawDecrement(200, 525, 100, 150);
  drawIncrement(400, 525, 100, 150);
  
  drawAllOnButton(200, 650, 100, 150);
  drawAllOffButton(400, 650, 100, 150);
  
  drawDiagnosticTest1(600, 400, 100, 150);
  drawDiagnosticTest2(600, 525, 100, 150);
  drawDiagnosticTest3(600, 650, 100, 150);
  
  
  draw7segDp_1(920, 100, 80, 40, 80, 40);
  draw7segDp_2(680, 100, 80, 40, 80, 40);
  draw7segDp_3(440, 100, 80, 40, 80, 40);
  draw7segDp_4(200, 100, 80, 40, 80, 40);
  

}


// The simple method variableEllipse() was created specifically 
// for this program. It calculates the speed of the mouse
// and draws a small ellipse if the mouse is moving slowly
// and draws a large ellipse if the mouse is moving quickly 

void variableEllipse(int x, int y, int px, int py) {
  float speed = abs(x-px) + abs(y-py);
  stroke(speed);
  ellipse(x, y, speed, speed);
}









void allOn()
{
   port.write(32); 
}
void allOff()
{
   port.write(33); 
}

void diagnosticTest1()
{
  port.write(34); 
}
void diagnosticTest2()
{
  port.write(35); 
}
void diagnosticTest3()
{
  port.write(36); 
}

void increment()
{
  port.write(37);
}
void decrement()
{
  port.write(38);
}

void incrementSeg()
{
  port.write(39);
}
void decrementSeg()
{
  port.write(40);
}








void aa()
{
  port.write('a');
}
void ab()
{
  port.write('b');
}
void ac()
{
  port.write('c');
}
void ad()
{
  port.write('d');
}
void ae()
{
  port.write('e');
}
void af()
{
  port.write('f');
}
void ag()
{
  port.write('g');
}
void ap()
{
  port.write('h');
}


void ba()
{
  port.write('i');
}
void bb()
{
  port.write('j');
}
void bc()
{
  port.write('k');
}
void bd()
{
  port.write('l');
}
void be()
{
  port.write('m');
}
void bf()
{
  port.write('n');
}
void bg()
{
  port.write('o');
}
void bp()
{
  port.write('p');
}



void ca()
{
  port.write('q');
}
void cb()
{
  port.write('r');
}
void cc()
{
  port.write('s');
}
void cd()
{
  port.write('t');
}
void ce()
{
  port.write('u');
}
void cf()
{
  port.write('v');
}
void cg()
{
  port.write('w');
}
void cp()
{
  port.write('x');
}



void da()
{
  port.write('y');
}
void db()
{
  port.write('z');
}
void dc()
{
  port.write('{');
}
void dd()
{
  port.write('|');
}
void de()
{
  port.write('}');
}
void df()
{
  port.write('~');
}
void dg()
{
  port.write('^');
}
void dp()
{
  port.write('_');
} //<>//

void draw()
{
  background(200, 100, 200); // r g b
  fill(0, 255, 0);              // text color
  textFont(font);
  text(Serial.list()[0], 100, 30);
  variableEllipse(mouseX, mouseY, pmouseX, pmouseY);
  
}


void drawAllOnButton(int xStart, int yStart, int height, int width)
{
  allOnButton.addButton("allOn")
    .setPosition(xStart, yStart)
    .setSize(width, height)
    .setFont(font)
    ;
}

void drawAllOffButton(int xStart, int yStart, int height, int width)
{
  allOnButton.addButton("allOff")
    .setPosition(xStart, yStart)
    .setSize(width, height)
    .setFont(font)
    ;
}

void drawIncrement(int xStart, int yStart, int height, int width)
{
  allOnButton.addButton("increment")
    .setPosition(xStart, yStart)
    .setSize(width, height)
    .setFont(font)
    ;
}
void drawDecrement(int xStart, int yStart, int height, int width)
{
  allOnButton.addButton("decrement")
    .setPosition(xStart, yStart)
    .setSize(width, height)
    .setFont(font)
    ;
}

void drawIncrementSeg(int xStart, int yStart, int height, int width)
{
  allOnButton.addButton("incrementSeg")
    .setPosition(xStart, yStart)
    .setSize(width, height)
    .setFont(font)
    ;
}
void drawDecrementSeg(int xStart, int yStart, int height, int width)
{
  allOnButton.addButton("decrementSeg")
    .setPosition(xStart, yStart)
    .setSize(width, height)
    .setFont(font)
    ;
}


void drawDiagnosticTest1(int xStart, int yStart, int height, int width)
{
  allOnButton.addButton("diagnosticTest1")
    .setPosition(xStart, yStart)
    .setSize(width, height)
    .setFont(font)
    ;
}
void drawDiagnosticTest2(int xStart, int yStart, int height, int width)
{
  allOnButton.addButton("diagnosticTest2")
    .setPosition(xStart, yStart)
    .setSize(width, height)
    .setFont(font)
    ;
}


void drawDiagnosticTest3(int xStart, int yStart, int height, int width)
{
  allOnButton.addButton("diagnosticTest3")
    .setPosition(xStart, yStart)
    .setSize(width, height)
    .setFont(font)
    ;
}






void draw7segDp_1(int xStart, int yStart, int height, int width, int xOffset, int yOffset)
{
  segmentA.addButton("aa")
    .setPosition(xStart, yStart)
    .setSize(height, width)
    .setFont(font)
    ;
   segmentA.addButton("ab")
    .setPosition(xStart+xOffset, yStart+(yOffset))
    .setSize(width, height)
    .setFont(font)
    ;
   segmentA.addButton("ac")
    .setPosition(xStart+xOffset, yStart+(yOffset*4))
    .setSize(width, height)
    .setFont(font)
    ;
  segmentA.addButton("ad")
    .setPosition(xStart, (yStart+240))
    .setSize(height, width)
    .setFont(font)
    ;
    
   segmentA.addButton("ae")
    .setPosition(xStart-40, yStart+(yOffset*4))
    .setSize(width, height)
    .setFont(font)
    ;
   segmentA.addButton("af")
    .setPosition(xStart-40, yStart+(yOffset))
    .setSize(width, height)
    .setFont(font)
    ;    
  segmentA.addButton("ag")
    .setPosition(xStart, (yStart+120))
    .setSize(height, width)
    .setFont(font)
    ;
  segmentA.addButton("ap")
    .setPosition(xStart+120, (yStart+240))
    .setSize(width, width)
    .setFont(font)
    ;
}

void draw7segDp_2(int xStart, int yStart, int height, int width, int xOffset, int yOffset)
{
  segmentB.addButton("ba")
    .setPosition(xStart, yStart)
    .setSize(height, width)
    .setFont(font)
    ;
   segmentB.addButton("bb")
    .setPosition(xStart+xOffset, yStart+(yOffset))
    .setSize(width, height)
    .setFont(font)
    ;
   segmentB.addButton("bc")
    .setPosition(xStart+xOffset, yStart+(yOffset*4))
    .setSize(width, height)
    .setFont(font)
    ;
  segmentB.addButton("bd")
    .setPosition(xStart, (yStart+240))
    .setSize(height, width)
    .setFont(font)
    ;
    
   segmentB.addButton("be")
    .setPosition(xStart-40, yStart+(yOffset*4))
    .setSize(width, height)
    .setFont(font)
    ;
   segmentB.addButton("bf")
    .setPosition(xStart-40, yStart+(yOffset))
    .setSize(width, height)
    .setFont(font)
    ;    
  segmentB.addButton("bg")
    .setPosition(xStart, (yStart+120))
    .setSize(height, width)
    .setFont(font)
    ;
  segmentB.addButton("bp")
    .setPosition(xStart+120, (yStart+240))
    .setSize(width, width)
    .setFont(font)
    ;
}

void draw7segDp_3(int xStart, int yStart, int height, int width, int xOffset, int yOffset)
{
  segmentC.addButton("ca")
    .setPosition(xStart, yStart)
    .setSize(height, width)
    .setFont(font)
    ;
   segmentC.addButton("cb")
    .setPosition(xStart+xOffset, yStart+(yOffset))
    .setSize(width, height)
    .setFont(font)
    ;
   segmentC.addButton("cc")
    .setPosition(xStart+xOffset, yStart+(yOffset*4))
    .setSize(width, height)
    .setFont(font)
    ;
  segmentC.addButton("cd")
    .setPosition(xStart, (yStart+240))
    .setSize(height, width)
    .setFont(font)
    ;
    
   segmentC.addButton("ce")
    .setPosition(xStart-40, yStart+(yOffset*4))
    .setSize(width, height)
    .setFont(font)
    ;
   segmentC.addButton("cf")
    .setPosition(xStart-40, yStart+(yOffset))
    .setSize(width, height)
    .setFont(font)
    ;    
  segmentC.addButton("cg")
    .setPosition(xStart, (yStart+120))
    .setSize(height, width)
    .setFont(font)
    ;
  segmentC.addButton("cp")
    .setPosition(xStart+120, (yStart+240))
    .setSize(width, width)
    .setFont(font)
    ;
}

void draw7segDp_4(int xStart, int yStart, int height, int width, int xOffset, int yOffset)
{
  segmentD.addButton("da")
    .setPosition(xStart, yStart)
    .setSize(height, width)
    .setFont(font)
    ;
   segmentD.addButton("db")
    .setPosition(xStart+xOffset, yStart+(yOffset))
    .setSize(width, height)
    .setFont(font)
    ;
   segmentD.addButton("dc")
    .setPosition(xStart+xOffset, yStart+(yOffset*4))
    .setSize(width, height)
    .setFont(font)
    ;
  segmentD.addButton("dd")
    .setPosition(xStart, (yStart+240))
    .setSize(height, width)
    .setFont(font)
    ;
    
   segmentD.addButton("de")
    .setPosition(xStart-40, yStart+(yOffset*4))
    .setSize(width, height)
    .setFont(font)
    ;
   segmentD.addButton("df")
    .setPosition(xStart-40, yStart+(yOffset))
    .setSize(width, height)
    .setFont(font)
    ;    
  segmentD.addButton("dg")
    .setPosition(xStart, (yStart+120))
    .setSize(height, width)
    .setFont(font)
    ;
  segmentD.addButton("dp")
    .setPosition(xStart+120, (yStart+240))
    .setSize(width, width)
    .setFont(font)
    ;
}
