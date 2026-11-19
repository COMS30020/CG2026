int subjectX = 0;
int subjectY = 200;
int outOfFocus = 0;
float aperture = 0;
ArrayList<Integer> paintedPoints = new ArrayList<Integer>();
boolean showAllRays = false;

void setup()
{
  size(600, 600);
  colorMode(HSB);
  surface.setTitle("LEFT/RIGHT Pixel  TAB All Pixels  +/- Aperture  UP/DOWN Focus  <x] Reset");
}

void draw()
{
  background(0);
  // Draw the "subject" colour spectrum
  for (int x=0; x<width; x++) {
    stroke(positionToColour(x,255));
    int y = (height/2-subjectY) + outOfFocus;
    line(x, y-2, x, y+2);
  }
  stroke(100);
  strokeWeight(12);
  // Draw the two aperture bars
  line(0, height/2, ((width-aperture)/2)-11, height/2);
  line(width, height/2, width-(((width-aperture)/2)-11), height/2);
  stroke(255);
  strokeWeight(2);
  // Draw the image plane
  line(100,height-100,width-100,height-100);
  // Draw the previously filled points of the "objective" colour spectrum
  noStroke();
  for (Integer x : paintedPoints) {
    fill(positionToColour(width-x.intValue(), int(80+aperture)));
    ellipse(x, height-100, 7, 7);
  }
  if (showAllRays) {
    for (int subjectX=-200; subjectX<=200; subjectX+=7) {
      drawRayLinesForPoint(subjectX, subjectY);
    }
  } else drawRayLinesForPoint(subjectX, subjectY);
  drawLens();
}

void drawLens()
{
  noFill();
  stroke(255);
  strokeWeight(2);
  float centerAngle = -PI/2;
  // Draw the lens
  arc(width/2, height+177, 1500, 1000, centerAngle-0.3, centerAngle+0.3);
  arc(width/2, -177, 1500, 1000, -centerAngle-0.3, -centerAngle+0.3);
}

void drawRayLinesForPoint(int subjectX, int subjectY)
{
  int objectiveX = -subjectX;
  int objectiveY = height/2 + subjectY;
  if(outOfFocus==0) stroke(positionToColour(width/2+subjectX,255),80);
  else stroke(255,0,255,80);
  strokeWeight(2);
  // Draw the ray lines
  for (int x=0; x<=aperture/2; x+=10) {
    // From subject to lens
    line(width/2+subjectX, height/2-subjectY, width/2+x, height/2);
    line(width/2+subjectX, height/2-subjectY, width/2-x, height/2);
    // Extending lines back behind the subject (worked out positions via trial-and-error !)
    if(! showAllRays) {
      line(width/2+subjectX, height/2-subjectY, width+(2*subjectX)-((width/2+x)), (height/2-2*subjectY));
      line(width/2+subjectX, height/2-subjectY, width+(2*subjectX)-((width/2-x)), (height/2-2*subjectY));
    }
    // From lens to objective
    line(width/2+x, height/2, width/2+objectiveX, objectiveY);
    line(width/2-x, height/2, width/2+objectiveX, objectiveY);
  }
  if(! showAllRays) {
    fill(255);
    noStroke();
    ellipse(width/2+subjectX, height/2-subjectY, 10, 10);
    ellipse(width/2+objectiveX, objectiveY, 10, 10);
  }
}

int positionToColour(int position, int brightness)
{
  if(position<100) return color(0);
  if(position>width-100) return color(0);
//  return color((position-100)/1.8, 255, brightness);
  return color((int(position/20)*100)%360, 255, brightness);
}

void keyPressed()
{
  if (keyCode==TAB) showAllRays = !showAllRays;
  if (keyCode==UP) outOfFocus-=4;
  if (keyCode==DOWN) outOfFocus+=4;
  if (keyCode==LEFT) subjectX-=5;
  if (keyCode==RIGHT) subjectX+=5;
  if (subjectX<-200) subjectX=-200;
  if (subjectX>200) subjectX=200;
  if (key=='=') aperture+=40;
  if (key=='-') aperture-=40;
  if (aperture>400) aperture = 400;
  if (aperture<0) aperture = 0;
  if (keyCode==BACKSPACE) {
    paintedPoints = new ArrayList<Integer>();
    outOfFocus = 0;
    subjectX = 0;
  }
  if((keyCode==LEFT) || (keyCode==RIGHT)) paintedPoints.add(new Integer(width/2-subjectX));
}
