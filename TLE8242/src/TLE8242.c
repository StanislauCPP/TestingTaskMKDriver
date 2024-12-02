#include "TLE8242.h"

#include "SPI_commonAPI.h"

#include <stdio.h>
#include <stdlib.h>


/**
 * @struct TLE8242Handler_t
 * @var TLE8242Handler_t::resetBPin_State
 * @var TLE8242Handler_t::enablePin_State
 * 
 * In resetBPin_State and enablePin_State we work with two flags:
 * 		highest bit (user setted pin State at least one time)
 * 		lowest bit (activate or disacivate pin)
 * 
 * In diagnosticTimer we work with three flags:
 * 		highest bit (user setted pin State at least one time)
 * 		two lowest bit (settings of diagnostic timer)
 * 
 */
struct TLE8242Handler_t
{
	uint8_t *dataBuffer;			///< data buffer contain last responsed message
	uint32_t buffSize;				///< buffer size
	uint8_t iCManufID;				///< IC Manufacturer ID Number
	uint8_t verNum;					///< Version Number
	uint8_t resetBPin_State;		///< Whether low does trigger FAULT pin (for reset_b pin)
	uint8_t enablePin_State;		///< Whether low does trigger FAULT pin (for enable pin)
	uint8_t diagnosticTimer;		///< Diagnostic Timer settings
};

TLE8242Handler_t* TLE8242_createHandler()
{
	TLE8242Handler_t* h = NULL;

	h = calloc(1, sizeof(TLE8242Handler_t));
	if(!h)
	{
		fprintf(stderr, "Memory for TLE8242Handler_t* hasn't allocated\n");
		exit(1);
	}

	h->buffSize = 4;			///< message size is 32 bits
	h->dataBuffer = calloc(h->buffSize, sizeof(uint8_t));
	if(!h->buffSize)
	{
		fprintf(stderr, "Memory for data buffer TLE8242Handler_t* hasn't allocated\n");
		free(h);
		exit(1);
	}

	h->iCManufID = 0xc1;		///< IC Manufacturer ID Number =  Infineon Technologies
	h->verNum = 0x2;			///< Version Number = B21
	h->resetBPin_State = 0x0;
	h->enablePin_State = 0x0;

	return h;
}

void TLE8242_deleteHandler(TLE8242Handler_t** handlerAdr)
{
	TLE8242Handler_t* h = (*handlerAdr);
	free(h->dataBuffer);
	free(h);
	(*handlerAdr) = NULL;
}

void TLE8242_errorHandler(int errorCode)
{
	switch (errorCode)
	{
	case -1:
		fprintf(stderr, "NULL dereference\n");
		break;

	case -2:
		fprintf(stderr, "Bad TLE8242 answer\n");
		break;

	case -3:
		fprintf(stderr, "Error pin state value\n");
		break;

	case -4:
		fprintf(stderr, "Uncorrected settings of diagnostic timer\n");
		break;

	case -5:
		fprintf(stderr, "reset_b pin or enable pin or diagnostic timer settings not setted\n");
		break;
	
	default:
		fprintf(stderr, "unexpected error\n");
		break;
	}
}

void TLE8242_printMsgData(const char *var, uint8_t sign, const char *val)
{
	printf("%s: ", var);
	if(sign)
	{
		printf("%s\n", val);
		return;
	}

	printf("Uncorrected answer\n");
}

void TLE8242_printChannelsData(const char *tVar, const char *tVal, uint8_t channels, const char *unsetBitData, const char *setBitData)
{
	TLE8242_printMsgData(tVar, 1, tVal);

	uint8_t bitFlag = 128;
	uint8_t chNum = 0;
	for(channels; channels; channels <<= 1, ++chNum)
	{
		printf("\tChannel %u: \t", chNum);
		if(channels & bitFlag)
			printf("%s\n", setBitData);
		else
			printf("%s\n", unsetBitData);
	}
}

void TLE8242_printSingleBitData(uint8_t bitStatus, const char *bitVar, const char *unsetBitData, const char *setBitData)
{
	printf("\n%s: ", bitVar);

	if(bitStatus)
		printf("%s\n", setBitData);
	else
		printf("%s\n", unsetBitData);
}

void TLE8242_msg0(uint8_t* resp)
{
	TLE8242_printMsgData("MSG_ID: 0", !resp[0], "IC Version / Manufacturer");
	TLE8242_printMsgData("IC Manuf ID", resp[1] == 0xc1, "Infineon Technologies");
	TLE8242_printMsgData("Version Number", resp[2] == 0x2, "B21");
}

void TLE8242_msg1(uint8_t* resp)
{
	TLE8242_printMsgData("MSG_ID: 1", resp[0] == 0x1, "Control Method and Fault Mask Configuration");
	TLE8242_printChannelsData("\n\tCMx", "\t\tControl Mode for Channel #x", resp[1], "Current Control", "Direct PWM");
	TLE8242_printChannelsData("\n\tFMx", "\t\tFault Mask for Channel #x", resp[1], " faults dont trigger FAULT pin", "fault triggers FAULT pin");
	TLE8242_printSingleBitData(resp[3] & 128, "Fault Mask for RESET_B pin", "RESET_B=Low doesnt trigger FAULT pin", "RESET_B=Low does trigger FAULT pin");
	TLE8242_printSingleBitData(resp[3] & 64, "Fault Mast for ENABLE pin", "ENABLE=Low doesnt trigger FAULT pin", "ENABLE=Low does trigger FAULT pin");

	printf("\nDiagnostic Timer: ");
	switch ((resp[3] >> 4) & 3)
	{
	case 0:
		printf("divide by 128, nfault = 10 ... 11\n");
		break;
	
	case 1:
		printf("divide by 192, nfault = 10 ... 11\n");
		break;

	case 2:
		printf("divide by 128, nfault = 2... 3\n");
		break;

	case 3:
		printf("divide by 256, nfault = 10 ... 11\n");
		break;		

	default:
		break;
	}
}

int TLE8242_ParsingMsg(TLE8242Handler_t *handler)
{
	if(handler == NULL)
		return -1;

	printf("\n*****Response parser.\tStart*****\n");

	uint8_t *resp = handler->dataBuffer;

	switch (resp[0])
	{
	case 0:
		TLE8242_msg0(resp);
		break;

	case 1:
		TLE8242_msg1(resp);
		break;
	
	default:
		printf("Uncorrected message number\n");
		break;
	}

	printf("*****Response parser.\tEnd*****\n");

	return 0;
}

void TLE8242_fillBuff(TLE8242Handler_t *handler, uint8_t* arr)
{
	for(uint32_t i = 0; i < handler->buffSize; ++i)
		handler->dataBuffer[i] = send(arr[i]);
}

int TLE8242_checkMK(TLE8242Handler_t *handler)
{
	if(handler == NULL)
		return -1;

	uint8_t sendMsg[] = {0, 0, 0, 0};
	TLE8242_fillBuff(handler, sendMsg);

	if(handler->dataBuffer[0] != 0
		|| handler->dataBuffer[1] != handler->iCManufID
		|| handler->dataBuffer[2] != handler->verNum
		|| handler->dataBuffer[3] != 0)
		return -2;

	return 0;
}

int TLE8242_setPin(TLE8242Handler_t *handler, uint8_t *pin, uint8_t pinState)
{
	if(handler == NULL)
		return -1;

	if(pinState != 0 && pinState != 1)
		return -3;
	
	(*pin) = (1 << 7) | pinState;		///< (1 << 7) - set that pinState setted in highest bit; pinState - set that pin activate or disactivate in lowest bit 

	return 0;
}

int TLE8242_setResetBPin(TLE8242Handler_t *handler, uint8_t pinState)
{
	return TLE8242_setPin(handler, &(handler->resetBPin_State), pinState);
}

int TLE8242_setEnablePin(TLE8242Handler_t *handler, uint8_t pinState)
{
	return TLE8242_setPin(handler, &(handler->enablePin_State), pinState);
}

int TLE8242_setDiagnosticTimer(TLE8242Handler_t *handler, enum TLE8242_DiagnosticTimer_t dTimer)
{
	for(uint8_t i = 0; i < 4; ++i)
		if(i == dTimer)
		{
			handler->diagnosticTimer = (1 << 7) | dTimer;	///< (1 << 7) - set that diagnosticTimer setted in highest bit; dTimer - diagnostic timer settings
			return 0;
		}
	
	return -4;
}

int TLE8242_setCMethodFmaskConfig(TLE8242Handler_t *handler, uint8_t channels_CMode, uint8_t channels_FMode)
{
	uint8_t userSettedFlag = 128;

	if(handler == NULL)
		return -1;

	if(!(handler->resetBPin_State & userSettedFlag)||		///< check whether user set of these parametrs 
		!(handler->enablePin_State & userSettedFlag)||
		!(handler->diagnosticTimer & userSettedFlag))
		return -5;
	
	uint8_t unSettings = (handler->resetBPin_State << 7) | (handler->enablePin_State << 6) | (handler->diagnosticTimer << 4);
	uint8_t sendMsg[] = {0x81, channels_CMode, channels_FMode, unSettings};
	TLE8242_fillBuff(handler, sendMsg);

	sendMsg[0] ^= 128;										///< set reading mode in number of message;

	if(*((uint32_t*)sendMsg) != *((uint32_t*)handler->dataBuffer))
		return -2;

	return 0;
}
