//Universidad del Valle de Guatemala


//Bibliotecas
#include<stdint.h>
#include<stdbool.h>
#include"inc/hw_memmap.h"
#include"inc/tm4c123gh6pm.h"
#include"driverlib/sysctl.h"
#include"driverlib/gpio.h"
#include"driverlib/timer.h"
#include"driverlib/interrupt.h"
#include "driverlib/uart.h"

int encender=false;
int color_led;
char dato_recibido;
int estaEncendido = false;
void timeOut(void);



void TimeOut(){
    TimerIntClear(TIMER0_BASE,  TIMER_TIMA_TIMEOUT);
    if (estaEncendido||!encender){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, color_led);
    }
    estaEncendido = !estaEncendido;
}


int main(void) {
    //Establecer reloj del microcontrolador
    SysCtlClockSet(SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_5);

    //Funcion de configuracion de UART0
    InitUART();


    //Habilitar periférico GPIO F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //Habilitar pines de salida y entrada
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    //Habilitar Temporizador 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    //Configurar temporizador como periódico
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    //Cargar el valor que contará el temporizador
    TimerLoadSet(TIMER0_BASE, TIMER_A, 20000000);
    //Habilitar interrupción por parte del GPTM
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    //Establecer la ISR
    TimerIntRegister(TIMER0_BASE, TIMER_A, TimeOut);
    //Habilitar interrupción por parte del NVIC
    IntEnable(INT_TIMER0A);
    IntMasterEnable();
    //Habilitar temporizador
    TimerEnable(TIMER0_BASE, TIMER_A);


    // Se manda mensajes por UART
    UARTCharPut(UART0_BASE, 'H');
    UARTCharPut(UART0_BASE, 'o');
    UARTCharPut(UART0_BASE, 'l');
    UARTCharPut(UART0_BASE, 'a');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'M');
    UARTCharPut(UART0_BASE, 'u');
    UARTCharPut(UART0_BASE, 'n');
    UARTCharPut(UART0_BASE, 'd');
    UARTCharPut(UART0_BASE, 'o');
    UARTCharPut(UART0_BASE, 10);
    UARTCharPut(UART0_BASE, 13);

    while (1){
        dato_recibido=UARTCharGet(UART0_BASE);

                if (dato_recibido=='r'){
                    encender=!encender;
                    color_led=2;

                }
                else if (dato_recibido=='b'){
                    encender=!encender;
                    color_led=4;

                }
                else if (dato_recibido=='g'){
                    encender=!encender;
                    color_led=8;

                }

                else{
                    color_led=0;

                }



    }

}


void InitUART(void)
{
    /*Enable the GPIO Port A*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    /*Enable the peripheral UART Module 0*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    /* Make the UART pins be peripheral controlled. */
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Sets the configuration of a UART. */
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    //Habilitamos las interrupciones de UART
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}

