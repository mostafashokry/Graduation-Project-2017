#include <stdio.h>
#include <bcm2835.h>
#include <jni.h>
#include <stdlib.h>
#include "server_ClientThread.h"


JNIEXPORT jintArray JNICALL Java_server_ClientThread_feedbackPosition(JNIEnv *env, jclass obj){
	
	//printf("Recive Position after edit");
	
	    if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root??\n");

    }

    if (!bcm2835_spi_begin())
    {
      printf("bcm2835_spi_begin failed. Are you running as root??\n");
    }
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST);      // The default lsb first
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                   // The default BCM2835_SPI_MODE1 cpol =0 , cpha =1
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // The default 6khz
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default slave select pin 
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    volatile int i =0, j=0;
    int size = 7;
    volatile int Position [size] ;  //Shared variable with java
    
    //uint8_t read_data;
	//read_data = bcm2835_spi_transfer(254);
	delay(1);	

    for(i ;i<size;i++)
    {
		Position[i] = bcm2835_spi_transfer(254);
		printf("P%d= %d   ",i,Position[i]);
		delay(1);	
	}
	
	
		printf("\n");
    
    jintArray result;
	result = (*env)->NewIntArray(env, size);
	
	// fill a temp structure to use to populate the java int array
	jint fill[size];
	for (j = 0; j < size; j++) {
		fill[j] = Position[j]; // put whatever logic you want to populate the values here.
		//printf("F%d= %d   ", j,fill[j]);
		}
//printf("\n");	
	
	// move from the temp structure to the java structure
	(*env)->SetIntArrayRegion(env, result, 0, size, fill);
		bcm2835_spi_end();
    bcm2835_close();
	return result;
    
}
