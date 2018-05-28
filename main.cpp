#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <softPwm.h>

int main(int argc, char** argv) {
	printf ("Raspberry Pi wiringPi test program\n") ;

	if (wiringPiSetup() == -1)
		exit (1) ;
	softPwmCreate(5, atoi(argv[1]), 200);
	delay(100);
}
