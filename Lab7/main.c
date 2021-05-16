//Universidad del Valle de Guatemala
//Depto de Ingenieria Mecantronica y Electronica
//Curso Digital2
//Prof. Kurt Kellner
//Diego Mencos
//Carne 18300

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


//Variables a usar en el programa
int encender=false;
int color_led;
char dato_recibido=0;
int estaEncendido = false;

//Prototipo de funciones
void Timer0IntHandler(void);
void UARTIntHandler(void);
void InitUART(void);
void colorLED(void);
void cambioColor(void);




int main(void) {
    //Establecer reloj del microcontrolador a 40
    SysCtlClockSet(SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_5);

    //Funcion de configuracion de UART0
    InitUART();


    //Habilitar perif�rico GPIO F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //Habilitar pines de salida y entrada
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    //Habilitar Temporizador 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    //Configurar temporizador como peri�dico
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    //Cargar el valor que contar� el temporizador
    TimerLoadSet(TIMER0_BASE, TIMER_A, 20000000);
    //Habilitar interrupci�n por parte del GPTM
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    //Establecer la ISR
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    //Habilitar interrupci�n por parte del NVIC
    IntEnable(INT_TIMER0A);
    IntEnable(INT_UART0);
    IntMasterEnable();
    //Habilitar temporizador
    TimerEnable(TIMER0_BASE, TIMER_A);


    // Se manda mensajes por UART
    UARTCharPut(UART0_BASE, 'E');
    UARTCharPut(UART0_BASE, 's');
    UARTCharPut(UART0_BASE, 'c');
    UARTCharPut(UART0_BASE, 'r');
    UARTCharPut(UART0_BASE, 'i');
    UARTCharPut(UART0_BASE, 'b');
    UARTCharPut(UART0_BASE, 'a');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'r');
    UARTCharPut(UART0_BASE, ',');
    UARTCharPut(UART0_BASE, 'g');
    UARTCharPut(UART0_BASE, ',');
    UARTCharPut(UART0_BASE, 'b');
    UARTCharPut(UART0_BASE, 10);
    UARTCharPut(UART0_BASE, 13);

    while (1){
       // dato_recibido=UARTCharGet(UART0_BASE);



                colorLED();

    }

}


void InitUART(void){
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

    UARTIntRegister(UART0_BASE ,UARTIntHandler);
}


void Timer0IntHandler(){
    TimerIntClear(TIMER0_BASE,  TIMER_TIMA_TIMEOUT);
    estaEncendido = !estaEncendido;
}


void UARTIntHandler(){
    UARTIntClear (UART0_BASE, UART_INT_RX);
    dato_recibido=UARTCharGet(UART0_BASE);
    cambioColor();
}

void colorLED(void){

    if (estaEncendido|| encender==false){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }else {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, color_led);
    }

}

void cambioColor(void){

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

