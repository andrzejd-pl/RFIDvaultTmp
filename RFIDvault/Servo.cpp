#include "Servo.h"

Servo::Servo(int vOpen, int vClose) {
	this->valueOpen = vOpen;
	this->valueClose = vClose;
}

Servo::~Servo() {
	close();
}

void Servo::open() {
	servo.setAngle(this->valueOpen);
}

void Servo::close() {
	servo.setAngle(this->valueClose);
}