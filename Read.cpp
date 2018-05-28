#include <unistd.h>
#include <signal.h>
void delay(int ms){
  usleep(ms*1000);
}

#include "MFRC522.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

class Servo {
private:
	unsigned int pin;
	unsigned int valueOpen;
	unsigned int valueClose;
public:
	Servo(int pin, int vOpen, int vClose) {
		this->pin = pin;
		this->valueOpen = vOpen;
		this->valueClose = vClose;
		softPwmCreate(pin, valueClose, 200);
	}

	void open() {
		softPwmWrite(pin, valueOpen);
	}

	void close() {
		softPwmWrite(pin, valueClose);
	}
};


void terminateProgram(int sig) {
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(0));
	std::cout << std::put_time(std::localtime(&now_c), "%F %T") <<" - End of program"<<std::endl;

	exit(1);
}

int main(int argc, char** argv){
      signal(SIGINT, terminateProgram);
  MFRC522 mfrc;
  mfrc.PCD_Init();

  if (wiringPiSetupGpio() == -1) {
	  std::cout << std::put_time(std::localtime(&now_c), "%F %T") <<" - End of program"<<std::endl;
	  exit (1) ;
  }

  Servo servo(atoi(argv[1], argv[2], argv[3]));
  while(1){
    // Look for a card
    if(!mfrc.PICC_IsNewCardPresent())
      continue;

    if( !mfrc.PICC_ReadCardSerial())
      continue;


    // Print UID
    for(byte i = 0; i < mfrc.uid.size; ++i){
      if(mfrc.uid.uidByte[i] < 0x10){
	printf(" 0");
	printf("%X",mfrc.uid.uidByte[i]);
      }
      else{
	printf(" ");
	printf("%X", mfrc.uid.uidByte[i]);
      }
      
    }
    printf("\n");
    delay(1000);
  }
  return 0;
}
