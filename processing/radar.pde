import processing.serial.*; 
import java.awt.event.KeyEvent; 
import java.io.IOException;

Serial myPort; 
String angle="";
String distance="";
String data="";
float pixsDistance;
int iAngle, iDistance;
int index1=0;

void setup() {
  fullScreen(); 
  smooth();
  

  myPort = new Serial(this, "COM8", 9600); 
  myPort.bufferUntil('.'); 
}

void draw() {
  fill(98,245,31);
  noStroke();
  
  fill(0, 15); 
  rect(0, 0, width, height); 
  
  drawRadar(); 
  drawLine();
  drawObject();
  drawText();
}

void serialEvent (Serial myPort) { 
  try {
    data = myPort.readStringUntil('.');
    if (data != null) {
      data = data.substring(0, data.length()-1);
      index1 = data.indexOf(","); 
      if (index1 > -1) {
        angle = data.substring(0, index1); 
        distance = data.substring(index1+1, data.length()); 
        
        iAngle = int(angle);
        iDistance = int(distance);
      }
    }
  } catch (Exception e) {

  }
}

void drawRadar() {
  pushMatrix();
  translate(width/2, height - height*0.1); 
  noFill();
  strokeWeight(2);
  stroke(98, 245, 31);
  
  arc(0, 0, (width-width*0.1), (width-width*0.1), PI, TWO_PI);
  arc(0, 0, (width-width*0.3), (width-width*0.3), PI, TWO_PI);
  arc(0, 0, (width-width*0.5), (width-width*0.5), PI, TWO_PI);
  arc(0, 0, (width-width*0.7), (width-width*0.7), PI, TWO_PI);
  
  for (int a=30; a<=150; a+=30) {
    line(0, 0, (-width/2)*cos(radians(a)), (-width/2)*sin(radians(a)));
  }
  popMatrix();
}

void drawObject() {
  pushMatrix();
  translate(width/2, height - height*0.1); 
  strokeWeight(10);
  stroke(255, 10, 10);
  
  pixsDistance = iDistance * ((height-height*0.2)*0.025); 
  
  if(iDistance < 40 && iDistance > 1){
    
    line(pixsDistance*cos(radians(iAngle)), -pixsDistance*sin(radians(iAngle)), 
         (width-width*0.505)*cos(radians(iAngle)), -(width-width*0.505)*sin(radians(iAngle)));
         
    if(iDistance < 15) {
       fill(255, 0, 0);
       textSize(60);
       textAlign(CENTER);
       text("TARGET LOCKED!", 0, -height*0.7);
       textSize(30);
       text("WARNING: INTRUDER WITHIN RANGE", 0, -height*0.6);
    }
  }
  popMatrix();
}

void drawLine() {
  pushMatrix();
  strokeWeight(9);
  stroke(30, 250, 60);
  translate(width/2, height - height*0.1); 
  line(0, 0, (height-height*0.15)*cos(radians(iAngle)), -(height-height*0.15)*sin(radians(iAngle))); 
  popMatrix();
}

void drawText() { 
  pushMatrix();
  translate(width/2, height - height*0.05);
  fill(98, 245, 31);
  textSize(25);
  textAlign(CENTER);
  text("Object: " + (iDistance < 40 ? "In Range" : "Out of Range") + " | Angle: " + iAngle + "° | Distance: " + iDistance + " cm", 0, 0);
  popMatrix();
}