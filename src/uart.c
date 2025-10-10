#include "uart.h"

/* global pointers for tx data buffer and next rx data address */
uint32_t *gUARTTXData;
uint32_t gNextData;

/* 
this function sets up the uart interface with dma for tx and rx.
we create static buffers for tx and rx, disable dma channels first,
configure uart pins dio5 as tx and dio4 as rx, and enable uart at 115200 baud.
rx dma is set in circular mode, so it keeps receiving into the buffer automatically,
and then the rx channel is enabled to start the reception. basically this makes uart 
ready for dma transfers without blocking the cpu.
*/
void UART_Initialize(void)
{
    static uint32_t UARTTXBuffer[TX_BUFFER_SIZE];
    static uint32_t UARTRXBuffer[RX_BUFFER_SIZE];

    Sys_DMA_ChannelDisable(DMA_TX_NUM);
    Sys_DMA_ChannelDisable(DMA_RX_NUM);

    gUARTTXData = UARTTXBuffer;
    gNextData   = 0;

    Sys_UART_DIOConfig(DIO_6X_DRIVE | DIO_WEAK_PULL_UP | DIO_LPF_ENABLE,
                       UART_TX, UART_RX);

    Sys_UART_Enable(SystemCoreClock, UART_BAUD_RATE, UART_DMA_MODE_ENABLE);

    Sys_DMA_ClearChannelStatus(DMA_RX_NUM);
    Sys_DMA_ChannelConfig(DMA_RX_NUM, DMA_RX_CFG, RX_BUFFER_SIZE, 0,
                          (uint32_t)&UART->RX_DATA, (uint32_t)UARTRXBuffer);

    Sys_DMA_ChannelEnable(DMA_RX_NUM);
}

/* 
this function reads data from the uart rx dma buffer.
it first checks if the dma has started and sets the next read address if needed.
then it checks the current dma write pointer to know how many bytes are available.
if the buffer wrapped around, it handles copying in two steps.
gNextData is updated as we read so next call knows where to continue.
returns the number of bytes copied to the provided data buffer.
*/
uint32_t UART_EmptyRXBuffer(uint8_t *data)
{
    uint32_t size;
    uint32_t temp;
    unsigned int i;

    if (((Sys_DMA_Get_ChannelStatus(DMA_RX_NUM) & DMA_START_INT_STATUS) != 0) &&
        (gNextData == 0))
    {
        gNextData = DMA->DEST_BASE_ADDR[DMA_RX_NUM];
    }

    temp = DMA->NEXT_DEST_ADDR[DMA_RX_NUM];

    if (gNextData == temp)
    {
        return (0);
    }

    if (gNextData < temp)
    {
        size = (temp - gNextData) >> 2;
        for (i = 0; i < size; i++)
        {
            data[i]   = *(uint8_t *)gNextData;
            gNextData = gNextData + 4;
        }
    }
    else
    {
        size = (temp + ((RX_BUFFER_SIZE << 2) - gNextData)) >> 2;
        temp = ((RX_BUFFER_SIZE << 2) - (gNextData - DMA->DEST_BASE_ADDR[DMA_RX_NUM])) >> 2;
        for (i = 0; i < temp; i++)
        {
            data[i]   = *(uint8_t *)gNextData;
            gNextData = gNextData + 4;
        }
        gNextData = DMA->DEST_BASE_ADDR[DMA_RX_NUM];
        if (size - temp > 0)
        {
            for (i = 0; i < (size - temp); i++)
            {
                data[temp + i] = *(uint8_t *)gNextData;
                gNextData = gNextData + 4;
            }
        }
    }

    return (size);
}

/* 
this function fills the uart tx dma buffer with data to send.
first it checks if a dma transfer is already active. if yes, it tries to append 
data to the existing transfer. it temporarily disables interrupts to safely update 
the transfer length. if dma is idle or finished, it copies data into the buffer 
and starts a new dma transfer. returns overflow error if data too big or none if ok.
this allows sending data without blocking, using dma efficiently.
*/
unsigned int UART_FillTXBuffer(uint32_t txSize, uint8_t *data)
{
    uint32_t *temp;
    uint32_t tempMask;
    unsigned int i;

    if (DMA_CTRL0[DMA_TX_NUM].ENABLE_ALIAS == DMA_ENABLE_BITBAND)
    {
        if (DMA_CTRL1[DMA_TX_NUM].TRANSFER_LENGTH_SHORT + txSize > TX_BUFFER_SIZE)
        {
            return (UART_ERRNO_OVERFLOW);
        }

        temp = gUARTTXData + (DMA_CTRL1[DMA_TX_NUM].TRANSFER_LENGTH_SHORT);
        for (i = 0; i < txSize; i++)
        {
            *temp = data[i];
            temp++;
        }

        tempMask = __get_PRIMASK();
        __set_PRIMASK(1);
        DMA_CTRL1[DMA_TX_NUM].TRANSFER_LENGTH_SHORT =
            (DMA_CTRL1[DMA_TX_NUM].TRANSFER_LENGTH_SHORT + txSize);

        if (DMA_CTRL0[DMA_TX_NUM].ENABLE_ALIAS == DMA_ENABLE_BITBAND)
        {
            __set_PRIMASK(tempMask);
            return (UART_ERRNO_NONE);
        }
        __set_PRIMASK(tempMask);
    }

    if (txSize > TX_BUFFER_SIZE)
    {
        return (UART_ERRNO_OVERFLOW);
    }

    temp = gUARTTXData;
    for (i = 0; i < txSize; i++)
    {
        *temp = data[i];
        temp++;
    }

    Sys_DMA_ChannelConfig(DMA_TX_NUM, DMA_TX_CFG, txSize, 0,
                          (uint32_t)gUARTTXData, (uint32_t)&UART->TX_DATA);
    Sys_DMA_ClearChannelStatus(DMA_TX_NUM);
    return (UART_ERRNO_NONE);
}
