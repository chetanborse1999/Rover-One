#ifndef _RPINS_H
#define _RPINS_H

#define CH1 13
#define CH2 12
#define CH3 11
#define CH4 10
#define CH5 9
#define CH6 8

const int servo_pins[6] = {22,24,26,28,30,32}; //{servo pin 1, servo pin 2, ....}
const int motor_pins[6][3] = {{3,37,35},{2,23,25},{4,31,33},{6,27,29},{5,50,48},{7,42,40}}; //{pwm, input1, input2}

#endif