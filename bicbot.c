#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    liftPot,        sensorPotentiometer)
#pragma config(Sensor, in2,    auton_select,   sensorPotentiometer)
#pragma config(Sensor, in3,    line_track1,    sensorLineFollower)
#pragma config(Sensor, in4,    line_track2,    sensorLineFollower)
#pragma config(Sensor, dgtl1,  auton_proceed1, sensorTouch)
#pragma config(Sensor, dgtl2,  auton_proceed2, sensorTouch)
#pragma config(Sensor, dgtl3,  rquad,          sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  lquad,          sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  sonarOut,       sensorSONAR_cm)
#pragma config(Sensor, I2C_1,  arm_enc,        sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           rightIntake,   tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port2,           rightFront,    tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port3,           rightMiddle,   tmotorVex393, PIDControl, reversed, encoder, encoderPort, dgtl3, 1000)
#pragma config(Motor,  port4,           rightBack,     tmotorVex393, openLoop)
#pragma config(Motor,  port5,           rightLift,     tmotorVex393HighSpeed, PIDControl, reversed, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port6,           leftLift,      tmotorVex393HighSpeed, PIDControl, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port7,           leftFront,     tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port8,           leftMiddle,    tmotorVex393, PIDControl, encoder, encoderPort, dgtl5, 1000)
#pragma config(Motor,  port9,           leftBack,      tmotorVex393, openLoop)
#pragma config(Motor,  port10,          leftIntake,    tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

#pragma competitionControl(Competition)
#pragma autonomousDuration(15)
#pragma userControlDuration(105)

#include "Vex_Competition_Includes.c"

#define SIGN(n) ((n > 0) ? 1 : ((n < 0) ? -1 : 0))
#define NORM(n) (127 * n / 10)

#define SPEED0 0
#define SPEED1 25
#define SPEED2 50
#define SPEED3 100
#define SPEED4 127

#define AUTON_SELECT_NUM_OPTS 3

void update_drive_joyvals(int *Y1, int *X1, int *X2, int threshold) {
  if (abs(vexRT[Ch1]) > threshold) {
    *Y1 = vexRT[Ch1];
  } else {
    *Y1 = 0;
  }

  if (abs(vexRT[Ch4]) > threshold) {
    *X1 = -vexRT[Ch4];
  } else {
    *X1 = 0;
  }

  if (abs(vexRT[Ch3]) > threshold) {
    *X2 = vexRT[Ch3];
  } else {
    *X2 = 0;
  }
}

void drive_set(int Y1, int X1, int X2) {
  motor[rightFront] = Y1 - X2 - X1;
  motor[rightMiddle] = X2 - Y1;
  motor[rightBack] =  Y1 - X2 + X1;
  motor[leftFront] = Y1 + X2 - X1;
  motor[leftMiddle] = X2 + Y1;
  motor[leftBack] =  Y1 + X2 + X1;
}

void lift_set(int speed) {
  motor[rightLift] = speed;
  motor[leftLift] = speed;
}

void lift_setPosition(int position) {

}

void intake_set(int speed) {
  motor[rightIntake] = speed;
  motor[leftIntake] = speed;
}

int getopt_auton_select(int num_opts) {
  return (int)(SensorValue[auton_select] / num_opts);
}

task safety() {
  while (true) {
    if (vexRT[Btn7L] && vexRT[Btn8D]) {
      allMotorsOff();
    }
  }
}

task drive() {
  int Y1 = 0, X1 = 0, X2 = 0, threshold = 15;

  while (true) {
    update_drive_joyvals(&Y1, &X1, &X2, threshold);
    drive_set(Y1, X1, X2);
  }
}

task lift() {
  int threshold = 20, wayp_diff = 0;

  while (true) {
    if (vexRT[Btn5U] || vexRT[Ch3Xmtr2] > threshold) {
      lift_set(vexRT[Btn5U] ? SPEED4 : vexRT[Ch3Xmtr2]);
    } else if (vexRT[Btn5D] || vexRT[Ch3Xmtr2] < -threshold) {
      lift_set(vexRT[Btn5D] ? -SPEED4 : vexRT[Ch3Xmtr2]);
    } else if (vexRT[Btn7D] || vexRT[Btn7DXmtr2]) {
      lift_setPosition(SETPOINT_0);
    } else if (vexRT[Btn7R] || vexRT[Btn7RXmtr2]) {
      lift_setPosition(SETPOINT_1);
    } else if (vexRT[Btn7U] || vexRT[Btn7UXmtr2]) {
      lift_setPosition(SETPOINT_2);
    } else {
      lift_set(0);
    }
  }
}

task intake() {
  int threshold = 20;

  while (true) {
    if (vexRT[Btn6U] || vexRT[Ch2Xmtr2] > threshold) {
      intake_set(vexRT[Btn6U] ? SPEED4 : vexRT[Ch2Xmtr2]);
    } else if (vexRT[Btn6D] || vexRT[Ch2Xmtr2] < -threshold) {
      intake_set(vexRT[Btn6D] ? -SPEED4 : vexRT[Ch2Xmtr2]);
    } else {
      intake_set(0);
    }
  }
}

void pre_auton() {
  // ...
}

task autonomous() {
  // ...
}

task usercontrol() {
  StartTask(safety);
  StartTask(drive);
  StartTask(lift); // do you even lift?!?!?!?!?!?!??!
  StartTask(intake);
}
