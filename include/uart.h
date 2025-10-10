#ifndef UART_H
#define UART_H

/* ----------------------------------------------------------------------------
 * if using a c++ compiler, make everything here have c linkage
 * avoids name mangling issues
 * ---------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif    /* ifdef __cplusplus */

/* ----------------------------------------------------------------------------
 * include files
 * - rsl10.h has all the chip specific registers and low-level functions
 * ---------------------------------------------------------------------------- */
#include <rsl10.h>

/* ----------------------------------------------------------------------------
 * defines
 * ---------------------------------------------------------------------------- */

/* uart pins */
#define UART_TX                         5   // transmit pin
#define UART_RX                         4   // receive pin
#define UART_BAUD_RATE                  115200 // default baud rate
#define RX_BUFFER_SIZE                  0x100  // receive buffer size
#define TX_BUFFER_SIZE                  0x200  // transmit buffer size

/* dma configuration for rx */
#define DMA_RX_NUM                      1
#define DMA_RX_CFG                      (DMA_ENABLE                 | \
                                         DMA_ADDR_CIRC              | \
                                         DMA_TRANSFER_P_TO_M        | \
                                         DMA_PRIORITY_0             | \
                                         DMA_DISABLE_INT_DISABLE    | \
                                         DMA_ERROR_INT_DISABLE      | \
                                         DMA_COMPLETE_INT_DISABLE   | \
                                         DMA_COUNTER_INT_DISABLE    | \
                                         DMA_START_INT_ENABLE       | \
                                         DMA_LITTLE_ENDIAN          | \
                                         DMA_SRC_ADDR_STATIC        | \
                                         DMA_SRC_WORD_SIZE_32       | \
                                         DMA_SRC_UART               | \
                                         DMA_DEST_ADDR_INC          | \
                                         DMA_DEST_ADDR_STEP_SIZE_1  | \
                                         DMA_DEST_WORD_SIZE_32)

/* dma configuration for tx */
#define DMA_TX_NUM                      0
#define DMA_TX_CFG                      (DMA_ENABLE                 | \
                                         DMA_ADDR_LIN               | \
                                         DMA_TRANSFER_M_TO_P        | \
                                         DMA_PRIORITY_0             | \
                                         DMA_DISABLE_INT_DISABLE    | \
                                         DMA_ERROR_INT_DISABLE      | \
                                         DMA_COMPLETE_INT_DISABLE   | \
                                         DMA_COUNTER_INT_DISABLE    | \
                                         DMA_START_INT_DISABLE      | \
                                         DMA_LITTLE_ENDIAN          | \
                                         DMA_SRC_ADDR_INC           | \
                                         DMA_SRC_WORD_SIZE_32       | \
                                         DMA_SRC_ADDR_STEP_SIZE_1   | \
                                         DMA_DEST_ADDR_STATIC       | \
                                         DMA_DEST_UART              | \
                                         DMA_DEST_WORD_SIZE_32)

/* uart error codes */
#define UART_ERRNO_NONE                 0   // no error
#define UART_ERRNO_OVERFLOW             1   // rx buffer overflow

/* ----------------------------------------------------------------------------
 * function prototypes
 * ---------------------------------------------------------------------------- */

/* initialize uart and dma channels */
extern void UART_Initialize(void);

/* clear the rx buffer and optionally copy data out */
extern uint32_t UART_EmptyRXBuffer(uint8_t *data);

/* fill tx buffer and start dma transfer */
extern unsigned int UART_FillTXBuffer(uint32_t txSize, uint8_t *data);

/* ----------------------------------------------------------------------------
 * close extern "c" block
 * ---------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* UART_H */
