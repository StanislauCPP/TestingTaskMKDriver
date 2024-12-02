#ifndef TLE8242_H
#define TLE8242_H

#include <stdint.h>

/**
 ************************************
 * @file TLE8242.h
 * @brief Implementation Wrapping TLE8242 settings and methods of transformation and diagnostic read
 ************************************
 * 
 */


/**
 * @struct TLE8242Handler_t
 */
struct TLE8242Handler_t;
typedef struct TLE8242Handler_t TLE8242Handler_t;

/**
 * @brief Creating TLE8242 handler object
 * @details Allocation memory for handler. Set: IC Manufacturer ID Number; Version Number - in handler object
 * @return Pointer in handler object
 */
TLE8242Handler_t * TLE8242_createHandler();

/**
 * @brief Deleting TLE8242 handler object
 * @details Free Allocated memory for handler
 * @param [in, out] handlerAdr address of handler object
 */
void TLE8242_deleteHandler(TLE8242Handler_t **handlerAdr);

/**
 * @brief Put description of error code in error log
 * @param [in] errorCode - -1 - NULL dereference; -2 - bad answer from TLE8242; -3 - wrong setted pinState value for reset_b pin or enable pin;
 * -4 - Uncorrected diagnostic timer settings; -5 - reset_b pin or enable pin or diagnostic timer not setted by user;
 */
void TLE8242_errorHandler(int errorCode);

/**
 * @brief Parse last responsed TLE8242 message
 * @param [in] handler - object of TLE8242 Handler
 * @return error state code: 0 if successful, error code (value < 0) if catch error
 */
int TLE8242_ParsingMsg(TLE8242Handler_t *handler);

/**
 * @brief Checking: IC Manufacturer ID Number, Version Number - in response from TLE8242
 * @param [in] handler - object of TLE8242 Handler
 * @return error state code: 0 if successful, error code (value < 0) if catch error
 */
int TLE8242_checkMK(TLE8242Handler_t *handler);

/**
 * @brief set reset_b pin in TLE8242
 * @param [in, out] handler - object of TLE8242 Handler
 * @param [in] pinState - activate (if pinState == 1) or disactivate (if pinState == 0) reset_b pin
 * @return error state code: 0 if successful, error code (value < 0) if catch error
 */
int TLE8242_setResetBPin(TLE8242Handler_t *handler, uint8_t pinState);

/**
 * @brief set enable pin in TLE8242
 * @param [in, out] handler - object of TLE8242 Handler
 * @param [in] pinState - activate (if pinState == 1) or disactivate (if pinState == 0) enable pin
 * @return error state code: 0 if successful, error code (value < 0) if catch error
 */
int TLE8242_setEnablePin(TLE8242Handler_t *handler, uint8_t pinState);

/**
 * @brief number of channel
 * @details numbers go from highest bit to lowest according to TLE8242 datasheet
 */
enum
{
	TLE8242_CH0 = 128,
	TLE8242_CH1 = 64,
	TLE8242_CH2 = 32,
	TLE8242_CH3 = 16,
	TLE8242_CH4 = 8,
	TLE8242_CH5 = 4,
	TLE8242_CH6 = 2,
	TLE8242_CH7 = 1
};

/**
 * @brief settings of diagnostic timer
 */
enum TLE8242_DiagnosticTimer_t
{
	D128N1011,		///< 00 = divide by 128, nfault = 10 ... 11 (RESET value)
	D196N1011,		///< 01 = divide by 192, nfault = 10 ... 11
	D128N23,		///< 10 = divide by 128, nfault = 2... 3
	D256N1011		///< 11 = divide by 256, nfault = 10 ... 11
};

/**
 * @brief set diagnostic timer settings in TLE8242
 * @param [in, out] handler - object of TLE8242 Handler
 * @param [in] dTimer - diagnostic timer settings
 * @return error state code: 0 if successful, error code (value < 0) if catch error
 */
int TLE8242_setDiagnosticTimer(TLE8242Handler_t *handler, enum TLE8242_DiagnosticTimer_t dTimer);

/**
 * @brief set Control Method and Fault Mask Configuration
 * @details Before we call this function must be called: TLE8242_setResetBPin, TLE8242_setEnablePin, TLE8242_setDiagnosticTimer
 * @param [in, out] handler - object of TLE8242 Handler
 * @param [in] channels_CMode - Control Mode for Channels. If we set direct PWM at least for one chanell (for example channels_CMode = CH0),
 * others will have current control (from channel 1 to channel 7).
 * If we need to set current control for all channels set channels_CMode = 0.
 * If we need to set Direct PWM for more than one channel use "|". For example channels_CMode = CH1 | CH5 - Direct PWM setted for channel 1 and channel 5,
 * others channel will have current control.
 * @param [in] channels_FMode - Fault Mask for Channels. If we set  fault triggers FAULT pin at least for one chanell (for example channels_FMode = CH0),
 * others will have faults don’t trigger FAULT pin (from channel 1 to channel 7).
 * If we need to set faults don’t trigger FAULT pin for all channels set channels_FMode = 0.
 * If we need to set fault triggers FAULT pin for more than one channel use "|". For example channels_FMode = CH1 | CH5 - fault triggers FAULT pin setted for channel 1 and channel 5,
 * others channel will have faults don’t trigger FAULT pin.
 * @return error state code: 0 if successful, error code (value < 0) if catch error
 */
int TLE8242_setCMethodFmaskConfig(TLE8242Handler_t *handler, uint8_t channels_CMode, uint8_t channels_FMode);

#endif