#include <stdio.h>
#include <bcm2835.h>
#include <jni.h>
#include <stdlib.h>
#include "server_ClientThread.h"

JNIEXPORT void JNICALL Java_server_ClientThread_sendPath1  (JNIEnv *env, jobject obj, jintArray pathx, jintArray pathy, jint numOfAgent, jint leader){
			printf("hello from c \n");
	jint* x = (*env) -> GetIntArrayElements(env, pathx, 0);
	jint* y = (*env) -> GetIntArrayElements(env, pathy, 0);
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
    
	
	int i =0;					//Shared variable with java
    //char read_data;

	
	if (numOfAgent==1)
	{
		leader=1;
	}
	bcm2835_spi_transfer(numOfAgent);
	delay(1);
	bcm2835_spi_transfer(leader);
		
    while ((x[i]!=255)&&(y[i]!=255))
    {
		//read_data = 
		bcm2835_spi_transfer(x[i]);
		delay(1);
		//read_data = 
		bcm2835_spi_transfer(y[i]);
		delay(1);
		printf("%d    %d\n",x[i],y[i]);
		i++;
	}
	//read_data = 
	bcm2835_spi_transfer(255);
	delay(1);
	//read_data = 
	bcm2835_spi_transfer(255);
    delay(1);
	//read_data = 
	bcm2835_spi_transfer(254);
    bcm2835_spi_end();
    bcm2835_close();
    return ;
	}
