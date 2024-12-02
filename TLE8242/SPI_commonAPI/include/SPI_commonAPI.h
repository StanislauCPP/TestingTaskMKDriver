#ifndef SPI_COMMONAPI_H
#define SPI_COMMONAPI_H

#include <stdint.h>

/**
 ************************************
 * @file SPI_commonAPI.h
 * @brief Very primitive (simple) emulation of Serial Peripheral Bus. Created for Infineon TLE8242
 ************************************
 * 
 */


/** 
 * @brief send 1 byte
 * @param[in] sByte sent byte
 * @return 1 byte response
 */
uint8_t send(uint8_t sByte);


#endif