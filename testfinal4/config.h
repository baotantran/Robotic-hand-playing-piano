#define pwm1_on_l "0x06"
#define pwm1_on_h "0x07"
#define pwm1_off_l "0x08"
#define pwm1_off_h "0x09"
#define pwm2_on_l "0x0a"
#define pwm2_on_h "0x0b"
#define pwm2_off_l "0x0c"
#define pwm2_off_h "0x0d"
#define pwm3_on_l "0x0e"
#define pwm3_on_h "0x0f"
#define pwm3_off_l "0x10"
#define pwm3_off_h "0x11"
#define pwm4_on_l "0x12"
#define pwm4_on_h "0x13"
#define pwm4_off_l "0x14"
#define pwm4_off_h "0x15"
#define pwm5_on_l "0x16"
#define pwm5_on_h "0x17"
#define pwm5_off_l "0x18"
#define pwm5_off_h "0x19"
#define pwm6_on_l "0x1a"
#define pwm6_on_h "0x1b"
#define pwm6_off_l "0x1c"
#define pwm6_off_h "0x1d"
#define pwm7_on_l "0x1e"
#define pwm7_on_h "0x1f"
#define pwm7_off_l "0x20"
#define pwm7_off_h "0x21"
#define pwm8_on_l "0x22"
#define pwm8_on_h "0x23"
#define pwm8_off_l "0x24"
#define pwm8_off_h "0x25"
#define pwm9_on_l "0x26"
#define pwm9_on_h "0x27"
#define pwm9_off_l "0x28"
#define pwm9_off_h "0x29"
#define pwm10_on_l "0x2a"
#define pwm10_on_h "0x2b"
#define pwm10_off_l "0x2c"
#define pwm10_off_h "0x2d"


void i2cGET(char* addr);

void i2cSET(char addr[], char hex[]);
void init_driver();
void finger_left_general(char* on_l, char* on_h, char* off_l, char* off_h);
void finger_right_general(char* on_l, char* on_h, char* off_l, char* off_h);
void finger_up_general(char* on_l, char* on_h, char* off_l, char* off_h);
void finger_down_general(char* on_l, char* on_h, char* off_l, char* off_h);
void finger_down(int numb);
void finger_up(int numb);
void finger_left(int numb);
void finger_right(int numb);


void ena();

void disable();
