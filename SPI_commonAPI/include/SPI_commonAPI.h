#ifndef SPI_COMMONAPI_H
#define SPI_COMMONAPI_H

/**
 ************************************
 * @file SPI_commonAPI.h
 * @brief Very primitive (simple) emulation of Serial Peripheral Bus
 ************************************
 * 
 */


/**
 * @brief Wrapper SPI response
 */
struct SPI_t;

/**
 * @brief creating SPI API object
 * @param[in] msgSize size of response message
 * @return pointer on Wrapper SPI API object
 */
struct SPI_t* createSPIObj(short msgSize);

/** 
 * @fn int sent(const void* data, struct SPI_t* sObj)
 * @brief senting message
 * @param[in] data data of sentable message
 * @param[in, out] sObj Wrapper SPI API object
 * @return 0 if SPI API worked normal, -1 how as error code
 */
int sent(const void* data, struct SPI_t* sObj);

/**
 * @brief getting pointer on response data from Wrapper SPI API object
 * @param[in] sObj Wrapper SPI API object
 */
const void* SPIData(const struct SPI_t* sObj);

/**
 * @brief deleting SPI API object
 * @param[in] sObjAdr address of pointer on Wrapper SPI API object
 */
void deleteSPIObj(struct SPI_t** sObjAdr);

#endif