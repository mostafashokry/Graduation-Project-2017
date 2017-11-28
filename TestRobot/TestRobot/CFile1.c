/*
 * CFile1.c
 *
 * Created: 06-Mar-17 5:56:33 PM
 *  Author: engah
 */ 





// Timer/counter3 to make control cycle of robot
// ISR(TIMER3_COMPA_vect)
// {
// 	if(g_motor_timer_count > 0) g_motor_timer_count--;
// }

//
//
// // Initialize SPI Slave Device
// void spi_init_slave (void)
// {
// 	DDRB = (1<<6);     //MISO as OUTPUT
// 	SPCR = (1<<SPE);   //Enable SPI
// }
//
// //Function to send and receive data for both master and slave
// unsigned char spi_tranceiver (unsigned char data)
// {
// 	// Load data into the buffer
// 	SPDR = data;
//
// 	//Wait until transmission complete
// 	while(!(SPSR & (1<<SPIF) ));
//
// 	// Return received data
// 	return(SPDR);
// }
//
