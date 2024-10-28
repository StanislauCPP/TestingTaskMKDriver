#ifndef TLE8242_H
#define TLE8242_H

/**
 ************************************
 * @file TLE8242.h
 * @brief Implementation Wrapping TLE8242 settings and methods of transformation and diagnostic read
 ************************************
 * 
 */


/**
 * @brief Wrapping TLE8242 settings
 * @details Count is 19 by datasheet sent/response message count
 */
struct TLE8242
{
	unsigned sMSG[19];	///< sent messages
	unsigned rMSG[19];	///< response messages we want to receive
};

/**
 * @brief Conversation Wrapper SPI API object data in TLE8242 data
 * @details Without checking NULL data
 * @param[in] data Wrapper SPI API object data
 * @return TLE8242 data
 */
unsigned int TLE8242_cast_msg(const void* data);

/**
 * @brief Diagnostic reading
 * @details It's assumed that treating fault channel is started from this function
 * 
 * @param[in] channels numbers of channels 0x4000000 (0-3)	0x5000000 (4-7)
 * @param[in] realResp real response from SPI API
 * @param[in] neededResp needed response, which is determined before entry point
 * @return 0 if reading reading was successful, -1 how as error code if real response is incorrect
 */
int TLE8242_diagnostic_read(const unsigned int channels
							,const unsigned int realResp
							,const unsigned int neededResp);

#endif