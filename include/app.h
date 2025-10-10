#ifndef APP_H
#define APP_H

/* ----------------------------------------------------------------------------
 * if using a c++ compiler, make everything here have c linkage
 * this avoids name mangling issues
 * ---------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif    /* ifdef __cplusplus */

/* ----------------------------------------------------------------------------
 * include files
 * - rsl10.h has all the chip specific registers and functions
 * - uart.h has functions to use uart with dma
 * ---------------------------------------------------------------------------- */
#include <rsl10.h>
#include "uart.h"

/* ----------------------------------------------------------------------------
 * defines
 * ---------------------------------------------------------------------------- */

/* led is connected to dio6, main status led */
#define LED_DIO                         6

/* dio used for recovery mode, allows easy reflashing */
#define RECOVERY_DIO                    12

/* ----------------------------------------------------------------------------
 * function prototypes
 * ---------------------------------------------------------------------------- */

/* initialize the system, setup led, uart, etc */
extern void Initialize(void);

/* ----------------------------------------------------------------------------
 * close extern "c" block
 * ---------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* APP_H */
