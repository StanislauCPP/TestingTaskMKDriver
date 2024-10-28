#include "SPI_commonAPI.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //if this is maintained in real SPIAPI library

/**
 * @brief Wrapper SPI response
 * @details hided in .c file
 */
struct SPI_t
{
	void* data;								///<response data
	short msgSize;							///<response data size in bytes
};

///very primitive data response in SPI
unsigned int rw(unsigned int MSG)
{
	switch (MSG)
	{
	case 0x00: 			return 0xC10200;
	case 0x81FFFF00:	return 0x1FFFF00;
	case 0x815D75D7:	return 0x15D75D7;
	case 0x835D75D7:	return 0x35D75D7;
	case 0x4000000:		return 0x4514514;
	case 0x5000000:		return 0x5514514;
	
	default:
		return MSG;
	}
}

struct SPI_t* createSPIObj(short msgSize)
{
	struct SPI_t* sObj = calloc(1, sizeof(struct SPI_t));
	if(!sObj)
		exit(1);

	sObj->data = calloc(msgSize, sizeof(char));
	if(!(sObj->data))
	{
		free(sObj);
		sObj = NULL;
		exit(1);
	}

	sObj->msgSize = msgSize;
	return sObj;
}

int sent(const void *data, struct SPI_t* sObj)
{
	unsigned int uData = rw(*((unsigned int*)data));
	memcpy(sObj->data, &uData, sObj->msgSize);

	return 0;

//code error -1
}

const void *SPIData(const struct SPI_t *sObj)
{
	return sObj->data;
}

void deleteSPIObj(struct SPI_t** sObjAdr)
{
	struct SPI_t* sObj = (*sObjAdr);

	free(sObj->data);
	free(sObj);

	sObj = NULL;
}
