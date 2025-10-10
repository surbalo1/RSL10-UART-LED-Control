#include <app.h>
#include <stdio.h>
#include <string.h>

/* 
this function initialize the whole system. first it masks all interrupts
to avoid any interference while setting up. it disables all nvics and clears 
pending interrupts. then dio12 is configured as input with pull-up so the 
program can pause for easy re-flashing if needed. vddrf is powered up and 
rf power switches enabled, isolation removed, and the 48 mhz crystal oscillator 
is started. then the 48 mhz is divided down to 8 mhz and the system clock 
switched to this new source. dio6 is set as gpio output for the led and uart 
is initialized with dma for tx/rx. finally, interrupts are unmasked so the 
system can run normally.
*/
void Initialize(void)
{
    __set_PRIMASK(PRIMASK_DISABLE_INTERRUPTS);
    Sys_NVIC_DisableAllInt();
    Sys_NVIC_ClearAllPendingInt();

    DIO->CFG[RECOVERY_DIO] = DIO_MODE_INPUT  | DIO_WEAK_PULL_UP |
                             DIO_LPF_DISABLE | DIO_6X_DRIVE;
    while (DIO_DATA->ALIAS[RECOVERY_DIO] == 0);

    ACS_VDDRF_CTRL->ENABLE_ALIAS = VDDRF_ENABLE_BITBAND;
    ACS_VDDRF_CTRL->CLAMP_ALIAS = VDDRF_DISABLE_HIZ_BITBAND;
    while (ACS_VDDRF_CTRL->READY_ALIAS != VDDRF_READY_BITBAND);

    SYSCTRL_RF_POWER_CFG->RF_POWER_ALIAS = RF_POWER_ENABLE_BITBAND;
    SYSCTRL_RF_ACCESS_CFG->RF_ACCESS_ALIAS = RF_ACCESS_ENABLE_BITBAND;

    RF->XTAL_CTRL = ((RF->XTAL_CTRL & ~XTAL_CTRL_DISABLE_OSCILLATOR) |
                     XTAL_CTRL_REG_VALUE_SEL_INTERNAL);
    RF_REG2F->CK_DIV_1_6_CK_DIV_1_6_BYTE = CK_DIV_1_6_PRESCALE_6_BYTE;
    while (RF_REG39->ANALOG_INFO_CLK_DIG_READY_ALIAS !=
           ANALOG_INFO_CLK_DIG_READY_BITBAND);

    Sys_Clocks_SystemClkConfig(JTCK_PRESCALE_1 |
                               EXTCLK_PRESCALE_1 |
                               SYSCLK_CLKSRC_RFCLK);

    Sys_DIO_Config(LED_DIO, DIO_MODE_GPIO_OUT_0);
    UART_Initialize();

    __set_PRIMASK(PRIMASK_ENABLE_INTERRUPTS);
}

/*
main loop that controls the led via uart. after initialization, a welcome 
message is sent explaining the commands. the loop runs forever, refreshing 
the watchdog each iteration. it polls the uart rx fifo: if data is received 
and is different from the last character, it updates last_char and processes 
the command. '1' turns the led on and sends confirmation, '0' turns it off 
and sends confirmation. a small busy wait avoids spamming messages. if fifo 
is empty last_char is reset so the same command can be received again. the 
system is simple, non-blocking, and uses registers and dma efficiently.
*/
int main(void)
{
    static uint8_t last_char = 0;  /* store last processed character */
    Initialize();

    const char* welcome_msg = "\r\n=== RSL10 UART LED Control ===\r\n"
                              "Commands:\r\n"
                              "  1 = Turn ON LED\r\n"
                              "  0 = Turn OFF LED\r\n"
                              "\r\nReady to receive commands...\r\n\r\n";
    UART_FillTXBuffer(strlen(welcome_msg), (uint8_t*)welcome_msg);

    while (1)
    {
        Sys_Watchdog_Refresh();

        if ((UART->STATUS & 0x1) == 0)
        {
            uint8_t received_char = (uint8_t)(UART->RX_DATA & 0xFF);

            if (received_char != last_char)
            {
                last_char = received_char;

                if (received_char == '1')
                {
                    Sys_GPIO_Set_High(LED_DIO);
                    const char* msg = "LED ON\r\n";
                    UART_FillTXBuffer(strlen(msg), (uint8_t*)msg);
                }
                else if (received_char == '0')
                {
                    Sys_GPIO_Set_Low(LED_DIO);
                    const char* msg = "LED OFF\r\n";
                    UART_FillTXBuffer(strlen(msg), (uint8_t*)msg);
                }
            }

            for(volatile uint32_t i = 0; i < 10000; i++);
        }
        else
        {
            last_char = 0;
        }
    }
}
