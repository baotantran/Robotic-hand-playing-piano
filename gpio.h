#ifndef GPIO_H
#define GPIO_H
//Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

// Set up Beaglebone Pins
#define MS1 66
#define MS2 69
#define STEP 45
#define DIR 26
#define SLP 46
#define EN 47
#define RST 27
#define TEST1 67
#define TEST2 68

// Step of motor
#define STEPS 200

// Number of White Keys
#define WKEYS 19

// Width of a Key
#define KEYWIDTH 2

//Circumference of Pulley
#define PULLEY 11



#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define ADC_PATH "/sys/devices/platform/bone_capemgr/slots"



int initPin(int pinnum);
int setPinDirection(int pinNum, char* dir);
int setPinValue(int pinnum, int value);
void stepForward();
void initADC();
int readVoltage();
int setPWM(char* ch, int numb, int pwm);
int setPWMPeriod(int chip, int a_b, int period);
int setPWMDuty(int chip, int a_b, int duty);
int setPWMEN(int chip, int a_b, int en);
int getPinValue(int pinNum);
void failed1();
void failed2();
void checkFail();
void reset();
int writeNumb();
int readNumb();
void writeChar();
void readFile();
int initUART(char* ch);
double time_diff(struct timeval x , struct timeval y);





#endif
