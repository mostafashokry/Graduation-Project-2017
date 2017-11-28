
#include "Motor.h"
void AvrInit()
{
	// SetExternal memory
	MCUCR = 0x80;
	XMCRA = 0x44;
	XMCRB = 0x80;
}

// Initialization code for using motor
void MotorInit()
{
	LMD18200_OUTPUT =~(0x07);    // Active Low output

	DDRE |= (1<<6);
	PORTE |= (1<<6);    // Activate hardware LM629 reset pin
	_delay_ms(500);
	PORTE &= ~(1<<6);

	// Reset each motor -> Incase reset fail. exception handling code must be applied
	MotorReset(MOTOR1);
	MotorReset(MOTOR2);
	MotorReset(MOTOR3);

	// PID control gain setting
	PIDSetting(MOTOR1, 2000, 2, 500);
	PIDSetting(MOTOR2, 2000, 2, 500);
	PIDSetting(MOTOR3, 2000, 2, 500);
	AccSet(MOTOR1, 100);
	AccSet(MOTOR2, 100);
	AccSet(MOTOR3, 100);

	LMD18200_OUTPUT = 0x07;
}

void WriteData(char motor, unsigned char data)
{
	switch(motor)
	{
		case MOTOR1:    MOTOR1_DATA = data;        break;
		case MOTOR2:    MOTOR2_DATA = data;        break;
		case MOTOR3:    MOTOR3_DATA = data;        break;
	}
}

void WriteCmd(char motor, unsigned char cmd)
{
	BusybitCheck(motor); // Check if it si ready for receiving command
	switch(motor)
	{
		case MOTOR1:    MOTOR1_CMD = cmd;        break;
		case MOTOR2:    MOTOR2_CMD = cmd;        break;
		case MOTOR3:    MOTOR3_CMD = cmd;        break;
	}
}

unsigned char ReadStatus(char motor)
{
	unsigned char data = 0;
	switch(motor)
	{
		case MOTOR1:    data = MOTOR1_CMD;        break;
		case MOTOR2:    data = MOTOR2_CMD;        break;
		case MOTOR3:    data = MOTOR3_CMD;        break;
	}
	return data;
}

void BusybitCheck(char motor)
{
	while((ReadStatus(motor) & 0x01));    // No 0 bit is busy bit and if the corresponding bit becomes 0. check is released
}

void WriteData2(char motor, unsigned int data)
{
	BusybitCheck(motor);

	WriteData(motor, (unsigned char)(data >> 8));
	WriteData(motor, (unsigned char)data);
}

// PID control value setting
void PIDSetting(char motor, unsigned int P, unsigned int I, unsigned int D)
{
	WriteCmd(motor, LFIL);        // Load Filter Parameters
	WriteData2(motor, 0x000e);    // bit3, 2, 1에 게인 데이터 입력
	WriteData2(motor, P);         // Enter gain data in bit 3, 2, 1
	WriteData2(motor, I);
	WriteData2(motor, D);
	WriteCmd(motor, UDF);        // Update the filter value nwely entered
}



// motor reset
void MotorReset(char motor)
{
	unsigned char status;

	status = ReadStatus(motor);

	if(status == 0xc4 || status == 0x84)
	{
		WriteCmd(motor, RSTI);
		WriteData2(motor, 0x0000);
	}
	else
	{
		// Reset failed.
	}

	status = ReadStatus(motor);

	if(status == 0xc0 || status == 0x80)
	{
		// Reset successful
	}
}

// Motor stop. fast acceleration/deceleration stop. immediate stop. stop using acceleration/deceleration value
void MotorStop(unsigned char stop)
{
	int i = 0;

	switch(stop)
	{
		case MOTOR_STOP_FAST:
		for(i = 0; i < 3; i++)
		{
			WriteCmd(i, LTRJ);
			WriteData2(i, 0x0200);
		}
		break;

		case MOTOR_STOP_OFF:
		for(i = 0; i < 3; i++)
		{
			WriteCmd(i, LTRJ);
			WriteData2(i, 0x0100);
		}
		break;

		case MOTOR_STOP_SMOOTH:
		for(i = 0; i < 3; i++)
		{
			WriteCmd(i, LTRJ);
			WriteData2(i, 0x0400);
		}
		break;
	}
	STTMotor();
}

// Set the direction of motor. Direction is set as LTRJ 12bits to declare that it runs with speed mode
void MotorDir(char motor, char dir)
{
	switch(dir)
	{
		case MOTOR_F:
		WriteCmd(motor, LTRJ);
		WriteData2(motor, 0x1800); //1 of 1800 means forward direction of motor
		break;

		case MOTOR_B:
		WriteCmd(motor, LTRJ);
		WriteData2(motor, 0x0800); //0 of 0800 means backward direction of motor
		break;
	}
}

void STTMotor()
{
	WriteCmd(MOTOR1, STT);
	WriteCmd(MOTOR2, STT);
	WriteCmd(MOTOR3, STT);
}

// Set acceleration value
void AccSet(char motor, double accel)
{
	unsigned long acc = (unsigned long)(10.69931 * accel);

	WriteCmd(motor, LTRJ);
	WriteData2(motor, 0x0020);    // bit5 acceleration
	WriteData2(motor, (unsigned int)(acc >> 16));
	WriteData2(motor, (unsigned int)acc);
}

// Set speed value
void VeloSet(char motor, double speed)
{
	unsigned long velo = (unsigned long)(523.98383 * speed);

	WriteCmd(motor, LTRJ);
	WriteData2(motor, 0x0008);    // bit3 speed
	WriteData2(motor, (unsigned int)(velo >> 16));
	WriteData2(motor, (unsigned int)velo);
}

void MotorStart(double *v)    // Acceleration value must be decided together
{
	int i = 0;
	double anglevelo[3] = {0};

	for(i = 0; i < 3; i++)
	{
		anglevelo[i] = v[i] * 3.18302;    // Multiply the lnear speed by 3.1802(3.141592) to convert the lnear speed to angle speed.


		if(v[i] < 0) //note: v set - value means backward direction of motor
		{
			VeloSet(i, -anglevelo[i]);
			MotorDir(i, MOTOR_B);
		}

		else //note: v set + value means forward direction of motor
		{
			VeloSet(i, anglevelo[i]);
			MotorDir(i, MOTOR_F);
		}
	}
	STTMotor();
}
