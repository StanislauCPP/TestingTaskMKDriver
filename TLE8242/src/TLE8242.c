#include "TLE8242.h"

#include <stdio.h>

unsigned int TLE8242_cast_msg(const void *data)
{
	unsigned int* uData = (unsigned int*)(data);
	return (*uData);
}

int TLE8242_diagnostic_read(const unsigned int channels
							,const unsigned int realResp
							,const unsigned int neededResp)
{
	if((channels & realResp) != channels)
		return -1;												//error code -1

	printf("\nTreating fault channels\n");						//depending on implementation

	return 0;
}
