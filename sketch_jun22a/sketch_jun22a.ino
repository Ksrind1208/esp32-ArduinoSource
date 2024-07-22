#include <Servo.h>  
#define pi 3.14
#define l3 8
#define l2 8

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

int servoPin1 = 9;  
int servoPin2 = 10;
int servoPin3 = 11;
int servoPin4 = 12;
String tq1;
String tq2;
String tq3;
String ttaygap;
float q1;
float q2;
float q3;
float taygap;


float l1;
String tx;
String ty;
String tz;
float x;
float y;
float z;

String txa;
String tya;
String tza;
float xa;
float ya;
float za;

String txb;
String tyb;
String tzb;
float xb;
float yb;
float zb;
String txc;
String tyc;
String tzc;
float xc;
float yc;
float zc;
String lenh;

const int trigPin = 8;
const int echoPin = 2;
volatile bool objectDetected = false;

String tx1,ty1,tz1;
String tx2,ty2,tz2;
float x1,y1,z1,x2,y2,z2;

String autoString;
float autoStorage[99];
int countAuto=0;

class Robot{
  public:
    float l22;
    float l33;
  public:
    Robot(float l22,float l33){
      this->l22=l22;
      this->l33=l33;
    }
    void ngat() {
    Serial.println("YAMETE KUDASAIIIIIII!!!!!");
    }
    void qkg() {
    int q1 = myservo1.read();
    int currentAngle2 = myservo2.read();
    int currentAngle3 = myservo3.read();
    int currentAngle4 = myservo4.read();
    q2 = 90 - currentAngle2;
    q3 = -(125 + q2 - currentAngle3);
    x = (l2 * cos(q2 * pi / 180) + l3 * cos((q2 + q3) * pi / 180)) * cos(q1 * pi / 180);
    y = (l2 * cos(q2 * pi / 180) + l3 * cos((q2 + q3) * pi / 180)) * sin(q1 * pi / 180);
    z = l2 * sin(q2 * pi / 180) + l3 * sin((q2 + q3) * pi / 180) + 6.5;
    Serial.print("x  la: ");
    Serial.println(x);
    Serial.print("y la: ");
    Serial.println(y);
    Serial.print("z la: ");
    Serial.println(z);
    }
    void movex() {
    int q1 = myservo1.read();
    int currentAngle2 = myservo2.read();
    int currentAngle3 = myservo3.read();
    int currentAngle4 = myservo4.read();
    q2 = 90 - currentAngle2;
    q3 = -(125 + q2 - currentAngle3);
    x = (l2 * cos(q2 * pi / 180) + l3 * cos((q2 + q3) * pi / 180)) * cos(q1 * pi / 180);
    y = (l2 * cos(q2 * pi / 180) + l3 * cos((q2 + q3) * pi / 180)) * sin(q1 * pi / 180);
    z = l2 * sin(q2 * pi / 180) + l3 * sin((q2 + q3) * pi / 180) + 6.5;
    kg(x + 0.1, y, z);
    }
    void movey() {
    int q1 = myservo1.read();
    int currentAngle2 = myservo2.read();
    int currentAngle3 = myservo3.read();
    int currentAngle4 = myservo4.read();
    q2 = 90 - currentAngle2;
    q3 = -(125 + q2 - currentAngle3);
    x = (l2 * cos(q2 * pi / 180) + l3 * cos((q2 + q3) * pi / 180)) * cos(q1 * pi / 180);
    y = (l2 * cos(q2 * pi / 180) + l3 * cos((q2 + q3) * pi / 180)) * sin(q1 * pi / 180);
    z = l2 * sin(q2 * pi / 180) + l3 * sin((q2 + q3) * pi / 180) + 6.5;
    kg(x, y + 0.1, z);
    }
    void movez() {
    int q1 = myservo1.read();
    int currentAngle2 = myservo2.read();
    int currentAngle3 = myservo3.read();
    int currentAngle4 = myservo4.read();
    q2 = 90 - currentAngle2;
    q3 = -(125 + q2 - currentAngle3);
    x = (l2 * cos(q2 * pi / 180) + l3 * cos((q2 + q3) * pi / 180)) * cos(q1 * pi / 180);
    y = (l2 * cos(q2 * pi / 180) + l3 * cos((q2 + q3) * pi / 180)) * sin(q1 * pi / 180);
    z = l2 * sin(q2 * pi / 180) + l3 * sin((q2 + q3) * pi / 180) + 6.5;
    kg(x, y, z + 0.1);
    }
    void home() {
    myservo1.write(90);
    myservo2.write(20);
    myservo3.write(70);
    myservo4.write(5);
    }
    void moveServo(Servo &servo, int currentAngle, int targetAngle) {
    if (currentAngle < targetAngle) {
      for (int angle = currentAngle; angle <= targetAngle; angle++) {
        servo.write(angle);
        delay(5);  
      }
    } else {
      for (int angle = currentAngle; angle >= targetAngle; angle--) {
        servo.write(angle);
        delay(5);  
      }
    }
    }

    void q(float q1, float q2, float q3, float taygap) {
    if ((q1 > 180 || q1 < 0) || (q2 < 0 || q2 > 90) || (q3 < 55 || q3 > 180) || (taygap < 5 || taygap > 90)) {
      Serial.println("sai goc nhap");
    } else {
      int currentAngle1 = myservo1.read();
      int currentAngle2 = myservo2.read();
      int currentAngle3 = myservo3.read();
      int currentAngle4 = myservo4.read();

      moveServo(myservo1, currentAngle1, q1);
      moveServo(myservo2, currentAngle2, q2);
      moveServo(myservo3, currentAngle3, q3);
      moveServo(myservo4, currentAngle4, taygap);

      delay(10);
    }
    }
    void kg(float a, float b, float c) {
    c=c-6.5;
    float q1 = atan2(b, a);
    float x2 = sqrt(a*a+b*b);
    float r = sqrt(sq(x2) + sq(c));
    float D = (sq(r) - sq(l3) - sq(l2)) / (2 * l3 * l2);
    double q3=-abs(acos(D));
    float q2 = atan(c/ x2) - q3 / 2;

    q1 = q1 * 180 / pi;
    q3 = 125 + q2 * 180 / pi + q3 * 180 / pi;
    q2 = 90 - q2 * 180 / pi;

    q(round(q1), round(q2), round(q3), myservo4.read());
    }
    void kgCircle(float a, float b, float c) {
    float q1 = atan2(b, a);
    float x2 = sqrt(a*a+b*b);
    float r = sqrt(sq(x2) + sq(c));
    float D = (sq(r) - sq(l3) - sq(l2)) / (2 * l3 * l2);
    double q3=-abs(acos(D));
    float q2 = atan(c/ x2) - q3 / 2;

    q1 = q1 * 180 / pi;
    q3 = 125 + q2 * 180 / pi + q3 * 180 / pi;
    q2 = 90 - q2 * 180 / pi;

    q(round(q1), round(q2), round(q3), myservo4.read());
    }
    void line(float xa, float ya, float za, float xb, float yb, float zb) {
      kg(xa, ya, za);
      delay(500);
      for (float t = 0; t <= 1.0; t += 0.1) {  
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        float duration = pulseIn(echoPin, HIGH);
        float distance = (duration / 2) * 0.0343;
        if (distance < 10.0) {
          stopAllServos();
          ngat();
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          duration = pulseIn(echoPin, HIGH);
          distance = (duration / 2) * 0.0343;
        } else {
          resumeAllServos();
        }
        float xt = xa + (xb - xa) * t;
        float yt = ya + (yb - ya) * t;
        float zt = za + (zb - za) * t;
        kg(xt, yt, zt);
        delay(10);
      }
      kg(xb,yb,zb);
      delay(10);
      
    }

    void circle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    z1=z1-6.5;
    z2=z2-6.5;
    z3=z3-6.5;

    float a = x2 - x1;
    float b = y2 - y1;
    float c = z2 - z1;
    float d = x3 - x1;
    float e = y3 - y1;
    float f = z3 - z1;

    float A = b * f - c * e;
    float B = c * d - a * f;
    float C = a * e - b * d;
    float D = A * x1 + B * y1 + C * z1;

    float xab = (x1 + x2) / 2;
    float yab = (y1 + y2) / 2;
    float zab = (z1 + z2) / 2;
    float xac = (x1 + x3) / 2;
    float yac = (y1 + y3) / 2;
    float zac = (z1 + z3) / 2;

    float nx = x2 - x1;
    float ny = y2 - y1;
    float nz = z2 - z1;

    float cosineTheta = C / sqrt(A * A + B * B + C * C);

 
    float H = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
    float xc = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / H;
    float yc = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / H;
    float zc = sqrt(xc * xc + yc * yc) * sin(acos(cosineTheta));
    float r = sqrt((x1 - xc) * (x1 - xc) + (y1 - yc) * (y1 - yc) + (z1 - zc) * (z1 - zc));


    float angleAtoC = atan((y3 - yc) / (x3 - xc));
    float angleAtoB = atan((y2 - yc) / (x2 - xc));


    if (angleAtoB < angleAtoC) {
      angleAtoB += 2* PI;
    }

    for (int i = 0; i <= 100; i++) {
      float theta = angleAtoC + (angleAtoB - angleAtoC) * (i / 100.0);
      float px = xc + r * cos(theta) ;
      float py = yc + r * sin(theta) ;
      float pz = (D - A * px - B * py) / C;  

      kg(px, py, pz+6.5);
    }
    kg(x3,y3,z3+6.5);
    }
    void gap(float x1,float y1,float z1,float x2,float y2, float z2){
    home();
    delay(2000);
    kg(x1,y1,z1);
    delay(2000);
    q(myservo1.read(),myservo2.read(),myservo3.read(),90);
    delay(2000);
    kg(x1,y1,z1-2);
    delay(2000);
    q(myservo1.read(),myservo2.read(),myservo3.read(),5);
    delay(2000);
    kg(x2,y2,z2);
    delay(2000);
    q(myservo1.read(),myservo2.read(),myservo3.read(),90);
    delay(2000);
    home();

    }

    void stopAllServos() {
    myservo1.detach();
    myservo2.detach();
    myservo3.detach();
    myservo4.detach();
    }

    void resumeAllServos() {
    myservo1.attach(servoPin1);
    myservo2.attach(servoPin2);
    myservo3.attach(servoPin3);
    myservo4.attach(servoPin4);
    }
    void drawArc(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {

    float mid1X = (x1 + x2) / 2;
    float mid1Y = (y1 + y2) / 2;
    float mid1Z = (z1 + z2) / 2;

    float mid2X = (x2 + x3) / 2;
    float mid2Y = (y2 + y3) / 2;
    float mid2Z = (z2 + z3) / 2;

   
    float radius = sqrt(pow(mid2X - mid1X, 2) + pow(mid2Y - mid1Y, 2) + pow(mid2Z - mid1Z, 2));
    for (float t = 0; t <= 1.0; t += 0.1) {  
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      float duration = pulseIn(echoPin, HIGH);
      float distance = (duration / 2) * 0.0343;
      while (distance < 10.0) {
        stopAllServos();
        ngat();
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        distance = (duration / 2) * 0.0343;
      }
      resumeAllServos();

      float xt = (1 - t) * (1 - t) * x1 + 2 * (1 - t) * t * x2 + t * t * x3;
      float yt = (1 - t) * (1 - t) * y1 + 2 * (1 - t) * t * y2 + t * t * y3;
      float zt = (1 - t) * (1 - t) * z1 + 2 * (1 - t) * t * z2 + t * t * z3;

      kg(xt, yt, zt);
      delay(100);
    }
  }
};
Robot myRobot(l2,l3);
void setup() {

  myservo1.attach(servoPin1);
  myservo2.attach(servoPin2);
  myservo3.attach(servoPin3);
  myservo4.attach(servoPin4);
  myRobot.home();
  Serial.begin(9600);  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop() {
 digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration / 2) * 0.0343;
  if (distance < 10.0) {
    myRobot.stopAllServos();
    myRobot.ngat();
  } else {
    myRobot.resumeAllServos();

  }
  while (Serial.available() > 0)  
  {

    lenh = Serial.readStringUntil(':');
    if (lenh == "q1") {
    }
    if (lenh == "home") {
      myRobot.home();
    }
    if (lenh == "q") {
      tq1 = Serial.readStringUntil(';');
      tq2 = Serial.readStringUntil(';');
      tq3 = Serial.readStringUntil(';');
      ttaygap = Serial.readStringUntil('\n');
      q1 = tq1.toInt();
      q2 = tq2.toInt();
      q3 = tq3.toInt();
      taygap = ttaygap.toInt();
      myRobot.q(q1, q2, q3, taygap);
    }
    if (lenh == "kg") {
      tx = Serial.readStringUntil(';');
      ty = Serial.readStringUntil(';');
      tz = Serial.readStringUntil('\n');
      x = tx.toFloat();
      y = ty.toFloat();
      z = tz.toFloat();
      myRobot.kg(x, y, z);
    }
    if (lenh == "line") {
      txa = Serial.readStringUntil(';');
      tya = Serial.readStringUntil(';');
      tza = Serial.readStringUntil(';');
      xa = txa.toFloat();
      ya = tya.toFloat();
      za = tza.toFloat();
      txb = Serial.readStringUntil(';');
      tyb = Serial.readStringUntil(';');
      tzb = Serial.readStringUntil('\n'); 
      xb = txb.toFloat();
      yb = tyb.toFloat();
      zb = tzb.toFloat();
      myRobot.line(xa, ya, za, xb, yb, zb);
    }
    if (lenh == "circle") {
      txa = Serial.readStringUntil(';');
      tya = Serial.readStringUntil(';');
      tza = Serial.readStringUntil(';');
      xa = txa.toFloat();
      ya = tya.toFloat();
      za = tza.toFloat();
      txb = Serial.readStringUntil(';');
      tyb = Serial.readStringUntil(';');
      tzb = Serial.readStringUntil(';');
      xb = txb.toFloat();
      yb = tyb.toFloat();
      zb = tzb.toFloat();
      txc = Serial.readStringUntil(';');
      tyc = Serial.readStringUntil(';');
      tzc = Serial.readStringUntil('\n');
      xc = txc.toFloat();
      yc = tyc.toFloat();
      zc = tzc.toFloat();
      myRobot.circle(xa, ya, za, xb, yb, zb, xc, yc, zc);
    }
    if (lenh == "qkg") {
      myRobot.qkg();
    }
    if(lenh=="gap"){
      tx1=Serial.readStringUntil(';');
      ty1=Serial.readStringUntil(';');
      tz1=Serial.readStringUntil(';');
      tx2=Serial.readStringUntil(';');
      ty2=Serial.readStringUntil(';');
      tz2=Serial.readStringUntil('\n');
      x1=tx1.toFloat();
      y1=ty1.toFloat();
      z1=tz1.toFloat();
      x2=tx2.toFloat();
      y2=ty2.toFloat();
      z2=tz2.toFloat();
      myRobot.gap(x1,y1,z1,x2,y2,z2);
    }
      if (lenh == "teaching") {  
      autoString = Serial.readStringUntil('\n'); // Read the entire command after "teaching:"
      int start = 0;
      int end = autoString.indexOf(';', start);
      while (end != -1) {
        autoStorage[countAuto++] = autoString.substring(start, end).toFloat();
        start = end + 1;
        end = autoString.indexOf(';', start);
      }
      autoStorage[countAuto++] = autoString.substring(start).toFloat();
      Serial.println(countAuto);
      myRobot.home();
      delay(2000);
      while(1){
        for(int i=0;i<countAuto-3;i+=4){
                float duration = pulseIn(echoPin, HIGH);
      float distance = (duration / 2) * 0.0343;
      while (distance < 10.0) {
        myRobot.stopAllServos();
        myRobot.ngat();
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        distance = (duration / 2) * 0.0343;
      }
      myRobot.resumeAllServos();
          myRobot.q(autoStorage[i],autoStorage[i+1],autoStorage[i+2],autoStorage[i+3]);
          delay(1000);
        }
        autoString=Serial.readStringUntil(':');
        if(autoString=="stop"){
          myRobot.home();
          break;
        }
      }
      countAuto=0;
    }

    delay(100);
  }
}