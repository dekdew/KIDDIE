#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial barcodeSerial(6, 7); // RX, TX

#define STOP_L 95
#define STOP_R 95

#define FWD_L 360
#define FWD_R 0

#define TRNL_L FWD_R
#define TRNL_R FWD_R

#define TRNR_L FWD_L
#define TRNR_R FWD_L

#define TRN_D 400
#define FWD_D 500

Servo SV_LEFT;
Servo SV_RIGHT;

char i = '0';
char a[6];
int k = 0;
String functionAList = "";
bool isFunctionAStart = false;

void setup() {
  SV_LEFT.attach(8);
  SV_RIGHT.attach(9);
  barcodeSerial.begin(9600);
  stop();
}

void loop() {
  execute(getCode());
}

void execute(String code) {
  if (code != "-1") {
    if (isFunctionAStart) {
      if (code == "0102") {
        endFunction();
        moveForward();
      } else {
        functionAList = functionAList + code;
        moveForward();
      }
    } else {
      if (code == "0001") {
        moveForward();
      } else if (code == "0002") {
        turnLeft();
      } else if (code == "0003") {
        turnRight();
      } else if (code == "0101") {
        startFunction();
        moveForward();
      }  else if (code == "0103") {
        runFunction();
      }
    }
  }

  delay(1000);
}

String getCode() {
  String code = "-1";

  while (barcodeSerial.available()) {
    i = barcodeSerial.read();
    a[k] = i;
    k = k + 1;

    if (k == 6) {
      code = String(a[0]) + String(a[1]) + String(a[2]) + String(a[3]);
      k = 0;
      break;
    }
  }


  return code;
}

void moveForward() {
  SV_LEFT.write(FWD_L);
  SV_RIGHT.write(FWD_R);
  delay(FWD_D);
  stop();
}

void turnLeft() {
  SV_LEFT.write(TRNL_L);
  SV_RIGHT.write(TRNL_R);
  delay(TRN_D);
  stop();
  delay(500);
  moveForward();
}

void turnRight() {
  SV_LEFT.write(TRNR_L);
  SV_RIGHT.write(TRNR_R);
  delay(TRN_D);
  stop();
  delay(500);
  moveForward();
}

void stop() {
  SV_LEFT.write(STOP_L);
  SV_RIGHT.write(STOP_R);
}

void startFunction() {
  functionAList = "";
  isFunctionAStart = true;
}

void endFunction() {
  isFunctionAStart = false;
}

void runFunction() {
  String cmdList = functionAList;
  
  while (cmdList != "") {
    String code = cmdList.substring(0, 4);
    execute(code);
    cmdList.remove(0, 4);
  }
}
