#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void delay(int ms){
#ifdef WIN32
  Sleep(ms);
#else
  usleep(ms*1000);
#endif
}

#include "MFRC522.h"
#include <wiringPi.h>
#include <softPwm.h>

byte goodCard[5] = {{} }

int main(int argc, char** argv){
  MFRC522 mfrc;

  mfrc.PCD_Init();

  if (wiringPiSetupGpio() == -1)
	  exit (1) ;
  softPwmCreate(24, atoi(argv[1]), 200);


  while(1){
    // Look for a card
    if(!mfrc.PICC_IsNewCardPresent())
      continue;

    if( !mfrc.PICC_ReadCardSerial())
      continue;

    softPwmWrite(24, atoi(argv[2]));

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
    softPwmWrite(24, atoi(argv[1]));
  }
  return 0;
}
