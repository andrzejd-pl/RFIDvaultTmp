#pragma once
#include "rpiServo.h"

class Servo {
private:
	rpiServo servo;
	unsigned int valueOpen;
	unsigned int valueClose;
public:
	Servo(int, int);

	virtual ~Servo();

	void open();

	void close();
};