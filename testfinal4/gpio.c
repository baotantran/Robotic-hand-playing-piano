#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "gpio.h"

int initPin (int num)
{
        FILE* f;
        f = fopen("/sys/class/gpio/export","w");
        if(f ==NULL) printf("Pin failed to initialized\n");


        fseek(f,0,SEEK_SET);
        fprintf(f,"%d",num);
        fflush(f);
        fclose(f);

        return 0;

}

void initADC()
{
        FILE* f;
        f = fopen(ADC_PATH, "w");
        if(f == NULL) printf("Pin failed to initialized\n");
        fseek(f, 0, SEEK_SET);
        fprintf(f, "%s", "BB-ADC");
        fflush(f);
        fclose(f);
}

int readVoltage()
{
        FILE* f;
        int number;
        f = fopen("/sys/bus/iio/devices/iio:device0/in_voltage0_raw", "r");
        fseek(f,0,SEEK_SET);
        fscanf(f,"%d",&number);
        fclose(f);
        return number;
}

int setPinDirection(int pinNum, char* dir)
{
        FILE *f;
        char buf[10];
        char buf2[50] = "/sys/class/gpio/gpio";

        //build file path to the direction file
        sprintf(buf,"%i",pinNum);
        strcat(buf2,strcat(buf,"/direction"));

        f = fopen(buf2, "w");
        if(f == NULL) printf("Direction failed to open\n");
        fseek(f,0,SEEK_SET);
        fprintf(f,"%s",dir);
        fflush(f);
        fclose(f);
        return 0;
}

// Set pins value
int setPinValue(int pinNum, int value)
{
        FILE* f;

        char buf[10];
        char buf2[50] = "/sys/class/gpio/gpio";

        //build file path to the value file
        sprintf(buf,"%i",pinNum);
        strcat(buf2,strcat(buf,"/value"));

        f = fopen(buf2, "w");
        if(f == NULL) printf("value failed to open\n");
        fseek(f,0,SEEK_SET);
        fprintf(f,"%d",value);
        fflush(f);
        fclose(f);

        return 0;
}

// Set pins value
int getPinValue(int pinNum)
{
        FILE* f;
        char ch;
        int value;

        char buf[10];
        char buf2[50] = "/sys/class/gpio/gpio";

        //build file path to the value file
        sprintf(buf,"%d",pinNum);
        strcat(buf2,strcat(buf,"/value"));

        f = fopen(buf2, "r");
        if(f == NULL) printf("value failed to open\n");
        fseek(f,0,SEEK_SET);
        ch = fgetc(f);
        value = atoi(&ch);
        fflush(f);
        fclose(f);

        return value;
}

int setPWM(char* ch, int numb, int pwm){
        FILE* f;
        FILE* f1;
        char buf[50] = "/sys/devices/platform/ocp/ocp:";
        char buf1[50];
        char buf2[50] = "/sys/class/pwm/pwmchip";
        char buf3[50];
        sprintf(buf1,"%s", ch);
        strcat(buf, strcat(buf1,"_pinmux/state"));
        //f = fopen("/sys/devices/platform/ocp/ocp:P9_14_pinmux/state", "w");
        f = fopen(buf, "w");
        if (f==NULL) {
                printf("error");
        }
        fseek(f,0,SEEK_SET);
        fprintf(f, "%s", "pwm");

        sprintf(buf3, "%d", numb);
        strcat(buf2, strcat(buf3, "/export"));
        //f1 = fopen("/sys/class/pwm/pwmchip2/export","w");
        f1 = fopen(buf2, "w");
        if (f1==NULL) {
                printf("error");
        }
        fseek(f1,0,SEEK_SET);
        fprintf(f1,"%d", pwm);
        fflush(f);
        fclose(f);
        fflush(f1);
        fclose(f1);
        return 0;
}

int initUART(char* ch)
{
        FILE* f;
        char buf[50] = "/sys/devices/platform/ocp/ocp:";
        char buf1[50];

        sprintf(buf1,"%s", ch);
        strcat(buf, strcat(buf1,"_pinmux/state"));

        f = fopen(buf, "w");
        if (f==NULL) {
                printf("error");
        }
        fseek(f,0,SEEK_SET);
        fprintf(f, "%s", "uart");

        fflush(f);
        fclose(f);

        return 0;
}

int setPWMPeriod(int chip, int a_b, int period){
        FILE* f;
        char buf[50] = "/sys/class/pwm/pwmchip";
        char buf1[50];
        char buf2[50];

        sprintf(buf1, "%d", chip);
        sprintf(buf2, "%d", a_b);
        strcat(buf, strcat(buf1, "/pwm"));
        strcat(buf, buf2);
        strcat(buf, "/period");


        f = fopen(buf, "w");
        if(f==NULL) {
                printf("error");
        }
        fseek(f, 0, SEEK_SET);
        fprintf(f, "%d", period);
        fclose(f);
        return 0;
}

int setPWMDuty(int chip, int a_b, int duty){
        FILE* f;
        char buf[50] = "/sys/class/pwm/pwmchip";
        char buf1[50];
        char buf2[50];

        sprintf(buf1, "%d", chip);
        sprintf(buf2, "%d", a_b);
        strcat(buf, strcat(buf1, "/pwm"));
        strcat(buf, buf2);
        strcat(buf, "/duty_cycle");


        f = fopen(buf, "w");
        if(f==NULL) {
                printf("error");
        }
        fseek(f, 0, SEEK_SET);
        fprintf(f, "%d", duty);
        fclose(f);
        return 0;
}
int setPWMEN(int chip, int a_b, int en){
        FILE* f;
        char buf[50] = "/sys/class/pwm/pwmchip";
        char buf1[50];
        char buf2[50];

        sprintf(buf1, "%d", chip);
        sprintf(buf2, "%d", a_b);
        strcat(buf, strcat(buf1, "/pwm"));
        strcat(buf, buf2);
        strcat(buf, "/enable");


        f = fopen(buf, "w");
        if(f==NULL) {
                printf("error");
        }
        fseek(f, 0, SEEK_SET);
        fprintf(f, "%d", en);
        fclose(f);
        return 0;
}
