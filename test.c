#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "config.h"
#include "gpio.h"
#define pwmchip 2

long int position = 0;
char c[1000];
char map[500] = "f0n g0n a0n b0n c1n d1n e1n f1n g1n a1n b1n c2n d2n e2n f2n g2n a2n b2n c3n f0s g0s a0s     c1s d1s     f1s g1s a1s     c2s d2s     f2s g2s a2s hol";
int n;
char str[15];
int temp = 0;
int text_pipe;
int voltage;
int value1;
int value2;
long int position1;
char musicnote[1000];
int note_length;
char compareString[100];

FILE *fp;

// Rotate stepper motor in clockwise direction
// Time is used to achieve the desire number of rotation of the stepper motor
// Stepper motor is configured to has fix speed
void rotateRight(int key){
        setPinValue(DIR,0);

        struct timeval tv;
        double end = 0;
        gettimeofday(&tv, NULL);

        double begin =
                (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

        setPWMEN(pwmchip,0,1);
        while(end - begin < 100 *key - 2)
        {

                gettimeofday(&tv, NULL);
                end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
                checkFail();
        }

        printf("Execution time %f\n", end - begin);
        setPWMEN(pwmchip,0,0);
}

// Rotate stepper motor in counter clockwise direction
// Time is used to achieve the desire number of rotation of the stepper motor
// Stepper motor is configured to has fix speed
void rotateLeft( int key){

        setPinValue(DIR,1);
        struct timeval tv;
        double end = 0;
        gettimeofday(&tv, NULL);

        double begin =
                (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

        setPWMEN(pwmchip,0,1);
        while(end - begin < 100 *key - 2)
        {

                gettimeofday(&tv, NULL);
                end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
                checkFail();
        }

        printf("Execution time %f\n", end - begin);
        setPWMEN(pwmchip,0,0);

}

// Resest the platform to the middle by rotating stepper clockwise
void failed1()
{
        setPinValue(DIR,0);
        setPWMEN(pwmchip,0,1);
        usleep(1312500);
        setPWMEN(pwmchip,0,0);
        while(1)
        {
                {
                        printf("Failed to find stopper\n");
                        printf("Enter 0 to reset again: \n");

                        scanf("%d", &n);

                        if(n == 0)
                        {
                                reset();
                                temp = 1;
                                break;
                        }
                }
        }
}

// Reset the platfrom to the middle by rotating stepper counter clockwise
void failed2()
{
        setPinValue(DIR,1);
        setPWMEN(pwmchip,0,1);
        usleep(1312500);
        setPWMEN(pwmchip,0,0);
        while(1)
        {

                {
                        printf("Failed. Meet the end of the rail\n");
                        printf("Enter 0 to reset again: \n");

                        scanf("%d", &n);

                        if(n == 0)
                        {
                                reset();
                                temp = 1;
                                break;
                        }
                }
        }
}

// Check if the platform contained the hand in contact with buttons at the edge
// Platform return to a fix postion after contacting the button by calling
// failed1() for left button and failed2() for right button
void checkFail()
{
        value1 = getPinValue(TEST1);
        value2 = getPinValue(TEST2);
        if (value1 == 0)
        {
                failed1();
        }

        if(value2 == 0)
        {
                failed2();
        }
}

// Compare the current position to the next required position and determine
// the rotation of the Stepper to move to the next location
void position_base(int x, int y)
{
        if(x < 0 || y < 0)
        {
                temp = 1;
                writeNumb();
        }
        else
        {
                if(x >= 15 && temp != 15)
                {
                        rotateRight(15 - temp);
                        temp = 15;
                }
                else if( x > 19)
                {

                }
                else if(x > temp && x < 15)
                {
                        rotateRight(x - temp);
                        temp = x;
                }
                else if(x < temp)
                {
                        rotateLeft(temp - x);
                        temp = x;
                }
                else
                {
                        // Play the note again
                }
        }
}

// Read data form text_pipe.txt
int readNumb()
{
        char ch[3];
        int t;

        fp = fopen("text_pipe.txt", "ab+");


        if(fp == NULL)
        {
                printf("Could not open.\n");
                return 1;
        }
        fgets(ch, sizeof(ch), fp);
        t = atoi(ch);

        temp = t;
        fclose(fp);
        return temp;


}

// Write data to text_pipe.txt
int writeNumb()
{

        fp = fopen("text_pipe.txt", "w");

        if(fp == NULL)
        {
                printf("Could not open.\n");
                return 1;
        }

        fprintf(fp, "%d", temp );

        fclose(fp);

        return temp;

}

// reset platform to the begining of the keyboard and all the fingers up left
void reset()
{
        disable();
        for(int i = 1; i <= 5; i++) {
                finger_up(i);
        }
        for(int i = 6; i <= 10; i++) {
                finger_left(i);
        }
        ena();
        usleep(250000);
        disable();
        setPWMPeriod(pwmchip, 0, 2000000);
        setPWMDuty(pwmchip, 0, 1000000);
        setPinValue(DIR,1);
        setPWMEN(pwmchip,0,1);
        while(1)
        {
                voltage = readVoltage();
                value1 = getPinValue(TEST1);
                if (value1 == 0)
                {
                        failed1();
                        break;
                }

                if(voltage > 100)
                {
                        setPinValue(DIR,0);
                        setPWMPeriod(pwmchip, 0, 1000000);
                        setPWMDuty(pwmchip, 0, 500000);
                        //setPWMEN(2,0,1);
                        usleep(170000);
                        setPWMEN(pwmchip,0,0);
                        temp = 1;
                        writeNumb();
                        break;
                }

        }
}

// Initialize I2C connection at the addr pin
void i2cGET(char* addr){
        FILE* f;
        char address[50];
        char get[50] = "i2cget -y 2 0x40 ";
        //printf("i2cget1\n");
        sprintf(address, "%s", addr);
        strcat(get, address);
        if(!(f = popen(get, "r"))) {
                printf("error opening file get\n");
        }
        pclose(f);
        //printf("i2cget2\n");
}

// Set value of the trasnmit by I2C
void i2cSET(char addr[], char hex[]){
        FILE* f;
        char address[50];
        char data[50];
        char set[50] = "i2cset -y -r 2 0x40 ";
        //printf("i2cset1\n");
        sprintf(address,"%s",addr);
        sprintf(data," %s", hex);
        strcat(set, strcat(address, data));

        if(!(f = popen(set, "r"))) {
                printf("failed to set data\n");
        }
        pclose(f);
        //printf("i2cset2\n");
}
void init_driver(){
        i2cSET("0x00", "0x11"); //sleep mode(set mode1)
        i2cGET("0x00");
        i2cSET("0xfe", "0x79"); //set pre_scale(50Hz)
        i2cGET("0xfe");

}
void finger_left_general(char* on_l, char* on_h, char* off_l, char* off_h){
        i2cSET(on_l, "0x99");// On_L
        i2cGET(on_l);
        i2cSET(on_h, "0x01");// On_H
        i2cGET(on_h);
        // on 10% = 409, off time start at 409 + 410 - 1 = 818(0x332)
        i2cSET(off_l,"0x32");// Off_L
        i2cGET(off_l);
        i2cSET(off_h,"0x03");// Off_H
        i2cGET(off_h);
}

void finger_right_general(char* on_l, char* on_h, char* off_l, char* off_h){
        i2cSET(on_l, "0x99");// On_L
        i2cGET(on_l);
        i2cSET(on_h, "0x01");// On_H
        i2cGET(on_h);
        // on 10% = 409, off time start at 409 + 410 - 1 = 818(0x332)
        i2cSET(off_l,"0x1b");// Off_L
        i2cGET(off_l);
        i2cSET(off_h,"0x03");// Off_H
        i2cGET(off_h);
}
void finger_up_general(char* on_l, char* on_h, char* off_l, char* off_h){
        i2cSET(on_l, "0x99");// On_L
        i2cGET(on_l);
        i2cSET(on_h, "0x01");// On_H
        i2cGET(on_h);
        // on 10% = 409, off time start at 409 + 410 - 1 = 818(0x332)
        i2cSET(off_l,"0x32");// Off_L
        i2cGET(off_l);
        i2cSET(off_h,"0x03");// Off_H
        i2cGET(off_h);
}
void finger_down_general(char* on_l, char* on_h, char* off_l, char* off_h){
        i2cSET(on_l, "0x99");// On_L
        i2cGET(on_l);
        i2cSET(on_h, "0x01");// On_H
        i2cGET(on_h);
        // on 10% = 409, off time start at 409 + 410 - 1 = 818(0x332)
        i2cSET(off_l,"0x65");// Off_L
        i2cGET(off_l);
        i2cSET(off_h,"0x02");// Off_H
        i2cGET(off_h);
}
void finger_down(int numb){
        if (numb==1) {
                finger_down_general(pwm1_on_l, pwm1_on_h, pwm1_off_l, pwm1_off_h);

        } else if (numb==2) {
                finger_down_general(pwm2_on_l, pwm2_on_h, pwm2_off_l, pwm2_off_h);

        } else if (numb==3) {
                finger_down_general(pwm3_on_l, pwm3_on_h, pwm3_off_l, pwm3_off_h);

        } else if (numb==4) {
                finger_down_general(pwm4_on_l, pwm4_on_h, pwm4_off_l, pwm4_off_h);

        } else if(numb==5) {
                finger_down_general(pwm5_on_l, pwm5_on_h, pwm5_off_l, pwm5_off_h);

        } else {
                printf("key out of range");
        }
}
void finger_up(int numb){
        if (numb==1) {
                finger_up_general(pwm1_on_l, pwm1_on_h, pwm1_off_l, pwm1_off_h);
        } else if (numb==2) {
                finger_up_general(pwm2_on_l, pwm2_on_h, pwm2_off_l, pwm2_off_h);
        } else if (numb==3) {
                finger_up_general(pwm3_on_l, pwm3_on_h, pwm3_off_l, pwm3_off_h);
        } else if (numb==4) {
                finger_up_general(pwm4_on_l, pwm4_on_h, pwm4_off_l, pwm4_off_h);
        } else if(numb==5) {
                finger_up_general(pwm5_on_l, pwm5_on_h, pwm5_off_l, pwm5_off_h);
        }else {
                printf("key out of range");
        }
}
void finger_left(int numb){
        if (numb==1) {
                finger_left_general(pwm1_on_l, pwm1_on_h, pwm1_off_l, pwm1_off_h);
        } else if (numb==2) {
                finger_left_general(pwm2_on_l, pwm2_on_h, pwm2_off_l, pwm2_off_h);
        } else if (numb==3) {
                finger_left_general(pwm3_on_l, pwm3_on_h, pwm3_off_l, pwm3_off_h);
        } else if (numb==4) {
                finger_left_general(pwm4_on_l, pwm4_on_h, pwm4_off_l, pwm4_off_h);
        } else if(numb==5) {
                finger_left_general(pwm5_on_l, pwm5_on_h, pwm5_off_l, pwm5_off_h);
        } else if(numb==6) {
                finger_left_general(pwm6_on_l, pwm6_on_h, pwm6_off_l, pwm6_off_h);
        } else if(numb==7) {
                finger_left_general(pwm7_on_l, pwm7_on_h, pwm7_off_l, pwm7_off_h);
        } else if(numb==8) {
                finger_left_general(pwm8_on_l, pwm8_on_h, pwm8_off_l, pwm8_off_h);
        } else if(numb==9) {
                finger_left_general(pwm9_on_l, pwm9_on_h, pwm9_off_l, pwm9_off_h);
        } else if(numb==10) {
                finger_left_general(pwm10_on_l, pwm10_on_h, pwm10_off_l, pwm10_off_h);
        } else {
                printf("key out of range");
        }

}
void finger_right(int numb){
        if (numb==1) {
                finger_right_general(pwm1_on_l, pwm1_on_h, pwm1_off_l, pwm1_off_h);
        } else if (numb==2) {
                finger_right_general(pwm2_on_l, pwm2_on_h, pwm2_off_l, pwm2_off_h);
        } else if (numb==3) {
                finger_right_general(pwm3_on_l, pwm3_on_h, pwm3_off_l, pwm3_off_h);
        } else if (numb==4) {
                finger_right_general(pwm4_on_l, pwm4_on_h, pwm4_off_l, pwm4_off_h);
        } else if(numb==5) {
                finger_right_general(pwm5_on_l, pwm5_on_h, pwm5_off_l, pwm5_off_h);
        } else if(numb==6) {
                finger_right_general(pwm6_on_l, pwm6_on_h, pwm6_off_l, pwm6_off_h);
        } else if(numb==7) {
                finger_right_general(pwm7_on_l, pwm7_on_h, pwm7_off_l, pwm7_off_h);
        } else if(numb==8) {
                finger_right_general(pwm8_on_l, pwm8_on_h, pwm8_off_l, pwm8_off_h);
        } else if(numb==9) {
                finger_right_general(pwm9_on_l, pwm9_on_h, pwm9_off_l, pwm9_off_h);
        } else if(numb==10) {
                finger_right_general(pwm10_on_l, pwm10_on_h, pwm10_off_l, pwm10_off_h);
        } else {
                printf("key out of range");
        }

}

void ena(){
        i2cSET("0x00", "0x01"); //enable
        i2cGET("0x00");
}
void disable(){
        i2cSET("0x00", "0x11"); //enable
        i2cGET("0x00");
}

int get_note_general(){
        FILE* f;
        int number_note;

        int i=0;
        if ((f = fopen("music2.txt","r")) == NULL) {
                printf("failed to open music");
        }
        fseek(f, position, SEEK_SET);
        memset(c, '\0', 999);
        while((c[i]=getc(f))!='-') {
                if(feof(f)) {
                        break;
                }
                i++;
                position++;
        }
        position++;
        if(c[i]!=EOF) {
                c[i]=EOF;
        }

        //printf("this is the string: %s\n", c);
        number_note = (i + 1)/4;
        //printf("%d\n",number_note);
        fflush(f);
        fclose(f);
        return number_note;


}
// TO DO: fis this to return a char array as input to play_piano function

int check_pos(int numb_note, int note, int previous){
        //FILE* f;
        //char temp[50];
        //int pos;
        //f=fopen("text.txt","r");
        //if(f==NULL) {
        //  printf("error open file");
        //}
        //fgets(temp, sizeof(temp), f);
        //pos = atoi(temp); //get position convert to int
        // traverse the finger(note at finger = current pos + i(order of finger))
        // if note at finger correspond to note required set pwm for finger_down
        // return the the next finger location for the next run of this function
        // so the finger play doesn't get repeat
        if(note == 37) {
                //printf("holding\n");
                usleep(note_length * 100000);
                return 0;
        }
        if (numb_note > 1 && previous == 0) {//if more the 1 note and this is the first time
                // move the first finger to the first note in the series
                readNumb();
                if(note > 19) {
                        position_base(note - 19, temp);
                        writeNumb();
                        finger_right(note - temp - 19 + 1 + 5); // control rotation of first finger
                        ena();
                        usleep(500);
                        disable();
                        finger_down(note - temp - 19 + 1);
                } else {
                        position_base(note, temp);
                        writeNumb();
                        finger_left(note - temp + 1 + 5); // control rotation of fisrt finger
                        ena();
                        usleep(500);
                        disable();
                        finger_down(note - temp + 1);
                }

                //printf("multi note start at: %d\n", 1);
                //printf("this is note0:%d\n", note);
                //printf("this is current pos0: %d\n", temp);
                return 1;
        }
        for(int i = previous; i <= 4; i++) {

                if(note == temp + i + 19) {
                        //printf("this is note:%d\n", note);
                        //printf("this is current pos: %d\n", temp);
                        //printf("%d\n", i+1);
                        finger_right(i + 1 + 5);
                        ena();
                        usleep(500);
                        disable();
                        finger_down(i+1);
                        return i + 1;        // next finger return
                } else if(note == temp + i) {
                        //printf("this is note:%d\n", note);
                        //printf("this is current pos: %d\n", temp);
                        //printf("%d\n", i+1);
                        finger_left(i + 1 + 5);
                        ena();
                        usleep(500);
                        disable();
                        finger_down(i+1);
                        return i + 1;                // next finger return
                }
        }

        if(note > 19) {
                //printf("this is note2:%d\n", note);
                //printf("this is current pos2: %d\n", temp);
                readNumb();
                //setPinValue(EN,1);
                position_base(note - 19, temp);
                writeNumb();
                finger_right(note - temp - 19 + 1 + 5); // control rotation of first finger
                ena();
                usleep(500);
                disable();
                finger_down(note - temp - 19 + 1); //set pwm for down
                //printf("note after move: %d\n", note);
                return 1;
        }

        readNumb();
        //setPinValue(EN,1);
        position_base(note, temp);
        //printf("this is note2:%d\n", note);
        //printf("this is current pos2: %d\n", temp);
        writeNumb();
        finger_left(note - temp + 1 + 5); // control rotation of first finger
        ena();
        usleep(500);
        disable();
        finger_down(note - temp + 1); //set pwm for down
        //printf("note after move: %d\n", note);
        return 1;
}
void play_piano(int numb_note, int* notes){

        int old = 0;
        int new;
        i2cSET("0x00", "0x11"); //sleep mode(set mode1)
        i2cGET("0x00");
        for(int i = 0; i < numb_note; i++) {
                new = check_pos(numb_note, notes[i], old);
                old = new;
        }
        ena();
        usleep(note_length * 225000);
        disable();
        for(int i = 1; i <= 5; i++) {
                finger_up(i);
        }
        for(int i = 6; i <= 10; i++) {
                finger_left(i);
        }
        ena();
        usleep(500);
}

int* mapping_general(int numb_note, char* notes){
        char note_1[3];
        char note_map[3];
        static int list_index[5];
        memset(note_1, '\0', strlen(note_1));
        memset(note_map, '\0', strlen(note_map));
        int count_char = 0;
        int count_map = 0;
        int count_list = 0;
        for (int i = 0; i < strlen(notes); i++) {
                if (notes[i] != ' ') {
                        if(count_char < 3) {
                                note_1[count_char] = notes[i];
                        }
                        count_char++;
                        if(count_char==4) {
                                note_length = notes[i] - '0';
                                //printf("note length: %d\n", note_length);
                                count_char = 0;
                                //printf("this is the note: %s\n", note_1);
                                for (int j = 0; j < strlen(map); j++) {
                                        if (map[j] != ' ') {
                                                note_map[count_map] = map[j];
                                                //printf("note_map(): %c\n", map[j]);
                                                count_map++;
                                                if(count_map==3) {
                                                        count_map = 0;
                                                        //printf("note_1: %s\n", note_1);
                                                        //printf("note_map: %s\n", note_map);
                                                        if(strcmp(note_1, note_map)==0) {
                                                                list_index[count_list] = j/4 + 1;
                                                                //printf("this is index: %d\n", list_index[count_list]);
                                                                count_list++;
                                                        }
                                                        memset(note_map, '\0', strlen(note_map));
                                                }
                                        }
                                }
                                memset(note_1, '\0', strlen(note_1));
                        }
                }
        }
        count_list = 0;
        return list_index;
}
int* mapping_general2(int numb_note, char* notes){
        char note_1[3];
        char note_2[3];
        char note_map[3];
        static int list_index[5];
        memset(note_1, '\0', strlen(note_1));
        memset(note_map, '\0', strlen(note_map));
        int count_char = 0;
        int count_map = 0;
        int count_list = 0;
        for (int i = 0; i < strlen(notes); i++) {
                if (notes[i] != ' ') {
                        if(count_char < 3) {
                                note_1[count_char] = notes[i];
                        }
                        count_char++;
                        if(count_char==4) {
                                note_length = notes[i] - '0';
                                //printf("note length: %d\n", note_length);
                                count_char = 0;
                                //printf("this is the note: %s\n", note_1);
                                *note_2 = *note_1;
                                for (int j = 0; j < strlen(map); j++) {
                                        if (map[j] != ' ') {
                                                note_map[count_map] = map[j];
                                                //printf("note_map(): %c\n", map[j]);
                                                count_map++;
                                                if(count_map==3) {
                                                        count_map = 0;
                                                        //printf("note_1: %s\n", note_1);
                                                        //printf("note_map: %s\n", note_map);
                                                        if(strcmp(note_1, note_map)==0) {
                                                                list_index[count_list] = j/4 + 1;
                                                                //printf("this is index: %d\n", list_index[count_list]);
                                                                count_list++;
                                                        }
                                                        memset(note_map, '\0', strlen(note_map));
                                                }
                                        }
                                }
                                memset(note_1, '\0', strlen(note_1));
                        }
                }
        }
        count_list = 0;
        return list_index;
}

void checkString()
{
        FILE* f;

        memset(compareString,'\0', 100);
        f = fopen("/dev/ttyO1", "r");
        fseek(f, 0, SEEK_SET);

        if (f == NULL)
        {
                printf("failed to open file f\n");
                exit(1);
        }

        fgets(compareString, sizeof(compareString), f);

        printf("%s", compareString);
}

int getNote()
{
        FILE* file;
        int i = 0;
        memset(musicnote,'\0', 512);
        int number_note;
        file = fopen("music.txt", "w+");
        if (file == NULL)
        {
                printf("failed to open file\n");
                exit(1);
        }

        fprintf(file, "%s", compareString);

        while(!feof(file))
        {

                //i = 0;
                printf("step 3\n");
                fseek(file, position1, SEEK_SET);
                printf("step 4\n");

                while((musicnote[i]=fgetc(file))!='-') {
                        /*  if(feof(f)) {
                                  break;
                           }*/
                        if(musicnote[i] == '\n')
                        {
                                break;
                        }
                        printf("step5 ");
                        if(feof(file)) {
                                break;
                        }
                        i++;
                        position1++;

                }
                if(musicnote[i]!=EOF) {
                        musicnote[i]=EOF;
                }
                /*if(musicnote[i] == '\n')
                   {
                        break;
                   }*/

                printf("step 6\n");
                position1++;
                /*if(musicnote[i] == '\n')
                   {
                        break;
                   }*/


                printf("this is the string: %s\n", musicnote);
                number_note = (i + 1)/4;
                //printf("%d notes\n",number_note);

        }

        position1 = 0;
        fflush(file);
        fclose(file);
        return number_note;
}

int main(){
        initPin(STEP);
        initPin(DIR);
        initPin(EN);
        initPin(TEST1);
        initPin(TEST2);
        initADC();
        initUART("P9_26");
        setPinDirection(MS1,"out");
        setPinDirection(MS2,"out");
        setPinDirection(STEP,"out");
        setPinDirection(DIR,"out");
        setPinDirection(SLP,"out");
        setPinDirection(EN,"out");
        setPinDirection(RST,"out");
        setPinDirection(TEST1,"in");
        setPinDirection(TEST2,"in");
        setPinValue(EN,0);
        setPWM("P9_14", pwmchip, 0);
        setPWMPeriod(pwmchip, 0, 1000000);
        setPWMDuty(pwmchip, 0, 500000);
        init_driver();
        int* list_note;
        int* real_note;
        int numb_note;
        reset();

        while(1) {
                printf("Waiting for Uart\n");
                checkString();
                if(strcmp(compareString, "reset\n") == 0) {
                        reset();
                } else{
                        if(strcmp(compareString, "music\n") == 0)
                        {
                                for(int i = 0; i <= 500; i++) {
                                        numb_note = get_note_general(); // return number of note play in the same time
                                        list_note = mapping_general(numb_note, c); // return list of note to play
                                        printf("this is notes music: %s\n", c);
                                        play_piano(numb_note, list_note);
                                        if(numb_note == 0) {
                                                break;
                                        }

                                }
                        }
                        else
                        {

                                numb_note = getNote();
                                //printf("numb note : %d\n", numb_note);
                                //printf("music note: %s\n", musicnote);
                                real_note = mapping_general2(numb_note, musicnote);
                                play_piano(numb_note, real_note);


                        }
                }





        }
        return 0;
}
