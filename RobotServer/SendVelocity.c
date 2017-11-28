#include <bcm2835.h>
#include <jni.h>
#include <stdlib.h>
#include "server_ClientThread.h"

JNIEXPORT void JNICALL Java_server_ClientThread_sendVelocity  (JNIEnv *env, jobject obj, jint Vx, jint Vy){
		    if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root??\n");
      //return 1;
    }

    if (!bcm2835_spi_begin())
    {
      printf("bcm2835_spi_begin failed. Are you running as root??\n");
      //return 1;
    }
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST);      // The default lsb first
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                   // The default BCM2835_SPI_MODE1 cpol =0 , cpha =1
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // The default 6khz
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default slave select pin 
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    

	
	bcm2835_spi_transfer(Vx);
	delay(1); 
	bcm2835_spi_transfer(Vy);
	delay(1); 
	bcm2835_spi_end();
    bcm2835_close();
    return ;
	}
