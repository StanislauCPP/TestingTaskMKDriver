//#include lib with API for Serial Peripheral Interface

#include <stdio.h>
#include <stdlib.h>
#include "SPI_commonAPI.h"
#include "TLE8242.h"

/**
 * @file main.c
 * @brief TLE8242 driver working imitation
 * 
 * 
 * @pre MSG - sent 								- response
 * @pre 0 	- 00000000000000000000000000000000 	- 00000000110000010000001000000000
 * @pre	1	- 10000001111111111111111100000000	- 00000001111111111111111100000000
 * @pre	2	- 10000001010111010111010111010111	- 00000001010111010111010111010111
 * @pre	3	- 10000011010111010111010111010111	- 00000011010111010111010111010111
 * @pre	4	- 00000100000000000000000000000000	- 00000100010100010100010100010100	Diagnostic
 * @pre	5	- 00000101000000000000000000000000	- 00000101010100010100010100010100	Diagnostic
 *
 * 
 * @pre MSG - description	
 * @pre	0	- IC Version / Manufacturer;
 * @pre	1	- PWM for all channels, all faults variety
 * @pre	2,3 - Short to battery 0.9V Short to Battery Retry Time PWM 16 * 7 - all channels
 * @pre	4,5 - Diagnostic read. Perhaps, for normal working response must have tested = 1, fault = 0 
 * 
 * 
 */

///start settings for MK		MSG	0 (R)		1 (W)		2 (W)		3 (W)		4 (R)		5 (R)
struct TLE8242 TLE8242_mk = {
							{0x0		,0x81FFFF00	,0x815D75D7	,0x835D75D7	,0x4000000	,0x5000000},
							{0xC10200	,0x1FFFF00	,0x15D75D7	,0x35D75D7	,0x4514514	,0x5514514} };

/**
 * @brief error Action
 * @details Printing error message and delete Wrapper SPI API object
 * @param[in] errMsg error message
 * @param[in] SPIObjAdr address of pointer on Wrapper SPI API object
 * @param[in] errCode error code. Error code is must determined from external Interface.
 * @return error code. In this example we return 1 or 2 or 3 only for example.
 */
int errAct(const char* errMsg, struct SPI_t** SPIObjAdr, int errCode)
{
	printf("%s\n", errMsg);
	deleteSPIObj(SPIObjAdr);
	return errCode;
}

/**
 * @brief entry poin of driver program
 * 
 */
int main(void)
{
	struct SPI_t* SPIObj = createSPIObj(4);

	///cycle for preworking settings assignment inside MK
	for(unsigned int i = 0; i < 4; ++i)
	{
		if(sent(&(TLE8242_mk.sMSG[i]), SPIObj) < 0)
			return errAct("SPI API Error", &SPIObj, 1);

		unsigned int response = TLE8242_cast_msg(SPIData(SPIObj));

		if(response != TLE8242_mk.rMSG[i])
			return errAct("TLE8242 bad settings assignment", &SPIObj, 1);
	}

	///cycle for diagnostic checking in working time
	for(unsigned int i = 0; i < 10; ++i)		//Only for demonstration. We have to use endless cycle in real life
	{
		for(unsigned int j = 4; j < 6; ++j)		///< MSG: 4, 5 - for diagnostic
		{
			if(sent(&(TLE8242_mk.sMSG[j]), SPIObj) < 0)
				return errAct("SPI API Error", &SPIObj, 1);

			unsigned int response = TLE8242_cast_msg(SPIData(SPIObj));

			if(response != TLE8242_mk.rMSG[j])
			{
				int badChannels = TLE8242_diagnostic_read(TLE8242_mk.sMSG[j], response, TLE8242_mk.rMSG[j]);

				if(badChannels < 0)
					return errAct("TLE8242 bad diagnostic response", &SPIObj, 3);				
			}
		}
	}

	deleteSPIObj(&SPIObj);
	return 0;
}