#include <stdio.h>
#include <stdlib.h>
#include "TLE8242.h"

/**
 ************************************
 * @file main.c
 * @brief TLE8242 driver program
 ************************************
 * 
 */


/**
 * @brief Action if we catch error (errcode < 0);
 * @param [in] handlerAdr - adress of TLE8242 object handler
 * @param [in] errCode - code of catched error
 */
void errorProgramConclusion(TLE8242Handler_t **handlerAdr, int errCode)
{
	TLE8242_errorHandler(errCode);
	TLE8242_deleteHandler(handlerAdr);
	exit(1);
}

/**
 * @brief entry point of driver program
 * 
 */
int main(void)
{
	int errCode;
	TLE8242Handler_t *handler;
	handler = TLE8242_createHandler();

	errCode = TLE8242_checkMK(handler);
	if(errCode < 0)	errorProgramConclusion(&handler, errCode);

	errCode = TLE8242_ParsingMsg(handler);
	if(errCode < 0)	errorProgramConclusion(&handler, errCode);

	errCode = TLE8242_setResetBPin(handler, 1);
	if(errCode < 0)	errorProgramConclusion(&handler, errCode);

	errCode = TLE8242_setEnablePin(handler, 1);
	if(errCode < 0)	errorProgramConclusion(&handler, errCode);

	enum TLE8242_DiagnosticTimer_t dTimer = D196N1011;
	errCode = TLE8242_setDiagnosticTimer(handler, dTimer);
	if(errCode < 0)	errorProgramConclusion(&handler, errCode);

	errCode = TLE8242_setCMethodFmaskConfig(handler, TLE8242_CH0 | TLE8242_CH1 | TLE8242_CH2 | TLE8242_CH3 | TLE8242_CH4 | TLE8242_CH5 | TLE8242_CH6 | TLE8242_CH7,
											TLE8242_CH0 | TLE8242_CH1 | TLE8242_CH2 | TLE8242_CH3 | TLE8242_CH4 | TLE8242_CH5 | TLE8242_CH6 | TLE8242_CH7);
	if(errCode < 0)	errorProgramConclusion(&handler, errCode);

	errCode = TLE8242_ParsingMsg(handler);
	if(errCode < 0)	errorProgramConclusion(&handler, errCode);

	TLE8242_deleteHandler(&handler);
}