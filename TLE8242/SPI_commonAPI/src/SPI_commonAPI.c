#include "SPI_commonAPI.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //if this is maintained in real SPIAPI library

/**
 * @struct SPI_commonAPI_t
 * @brief Wrapper of response message
 * @details Response message is started from 31'th bit
 */
struct SPI_commonAPI_t
{
	uint8_t resp[5][4];	///< number of row - number of message; number of column - response message part, cause send function receive and response only 1 byte
	uint8_t actMsgNum;	///< actuall message number;
	uint8_t actPartNum;	///< actuall message part number;
};

struct SPI_commonAPI_t SPIobj = { {{0x0, 0xc1, 0x2, 0x0}, {0x1, 0xff, 0xff, 0xd0}}, 0, 0};

uint8_t send(uint8_t sByte)
{
	uint8_t actPNum = SPIobj.actPartNum;
	if(!actPNum)
	{
		if(sByte & 128)					///< if write flag setted 1, we need to set its in 0 for getiing correct message number
			sByte ^= 128;

		SPIobj.actMsgNum = sByte;
	}

	SPIobj.actPartNum = (SPIobj.actPartNum + 1) % 4;

	return SPIobj.resp[SPIobj.actMsgNum][actPNum];
}