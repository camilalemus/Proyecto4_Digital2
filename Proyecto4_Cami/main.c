//**************************************************************************
// Camila Lemus Lone
// 18272
// Universidad del Valle de Guatemala
//**************************************************************************

#include<stdint.h>
#include<stdbool.h>
#include"inc/hw_memmap.h"
#include"inc/hw_types.h"
#include"inc/tm4c123gh6pm.h"
#include"driverlib/sysctl.h"
#include"driverlib/gpio.h"
#include"driverlib/timer.h"
#include"driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"

//**********************VARIABLES*******************************************

bool flag = false;
int led;
char letra;
int parqueo;
int parqueo1;
int parqueo2;
int parqueo3;
int parqueo4;
int contador;
int suma;
int cont[] = {0,0,0,0};

int pushP[] = { GPIO_PORTF_BASE, GPIO_PORTF_BASE, GPIO_PORTB_BASE,
GPIO_PORTC_BASE };
int pushV[] = { GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_3, GPIO_PIN_4 };
int state[] = { 0, 0, 0, 0 };
int stateantes[] = { 1, 1, 1, 1 };

//GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2); //Lee push 1
//GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3); //Lee push 2
//GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3); //Lee push 3
//GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4); //Lee push 4

#define RED GPIO_PIN_1
#define GREEN GPIO_PIN_3
#define BLUE GPIO_PIN_2
#define OFF 0

// Display
//-----------
// PB5 - g
// PB0 - f
// PB1 - a
// PE4 - b
// PE5 - e
// PB4 - d
// PA5 - c

// Leds azules
//------------
// PD0
// PD1
// PD2
// PD3

// Leds rojos
//------------
// PE1
// PE2
// PE3
// PF1

//Pushes
//------------
// PF2
// PF3
// PB3
// PC4

//************************INSTANCIAR FUNCIONES*********************************
void UART_Config(void);
void Parqueos(void);
void Display(void);

/**
 * main.c
 */

//*****************************************************************************
int main(void)
{
    SysCtlClockSet(
    SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // Clk a 40MHz
    //UART_Config();

    //Peripheral Enable
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //PUSH BUTTONS
    GPIOPinConfigure(GPIO_PF2_T1CCP0);
    GPIOPinConfigure(GPIO_PF3_T1CCP1);    //2
    GPIOPinConfigure(GPIO_PB3_T3CCP1);    //3
    GPIOPinConfigure(GPIO_PC4_WT0CCP0);    //4

    //LEDS

    GPIOPinConfigure(GPIO_PD0_WT2CCP0); //Parque 1 verde D0
//    GPIOPinConfigure(GPIO_PE1_U7TX); //Parque 1 roja E1
    GPIOPinConfigure(GPIO_PD1_WT2CCP1); //Parque 2 verde D1
    //  GPIOPinConfigure(GPIO_PD0_WT2CCP0); //Parque 2 roja E2
    GPIOPinConfigure(GPIO_PD2_WT3CCP0); //Parque 3 verde D2
    //   GPIOPinConfigure(GPIO_PD0_WT2CCP0); //Parque 3 roja E3
    GPIOPinConfigure(GPIO_PD3_WT3CCP1); //Parque 4 verde D3
    GPIOPinConfigure(GPIO_PF1_TRD1); //Parque 4 roja F1

    //Salidas y entradas
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5); //A
    GPIOPinTypeGPIOOutput(
            GPIO_PORTB_BASE,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5); //B
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); //D
    GPIOPinTypeGPIOOutput(
            GPIO_PORTE_BASE,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
                    | GPIO_PIN_5); //E
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); //F

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3); //B
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4); //C
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3); //F

    //Push Button - Pullup
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU);

    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0); //Led verde parqueo 1 encendida
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1); //Led verde parqueo 2 encendida
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2); //Led verde parqueo 3 encendida
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3); //Led verde parqueo 4 encendida

    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0); //Led roja parqueo 1 OFF
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0); //Led roja parqueo 2 OFF
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0); //Led roja parqueo 3 OFF
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); //Led roja parqueo 4 OFF


    int n = 0;

    while (1)
    {

        //  contador = parqueo1 + parqueo2 + parqueo3 + parqueo4;
        state[n] = GPIOPinRead(pushP[n], pushV[n]); //Lee el push que apachó
        if (state[n] != stateantes[n]) //Si cambia de estado
        {
            stateantes[n] = state[n];
            parqueo = n + 1;
            Parqueos();
        }
        else
        {

        }

        if (n < 4)
        {
            n++;
        }

        else
        {
            n = 0;
        }

        Display();
    }
}

//******************************* UART *******************************************

void UART_Config(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //Peripheral del UART enable
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //Peripheral del puerto A enable
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); //Los pines TX y RX se ponen en modo UART
    //Inicializar el modulo UART con: 115200, 8 data bits, 1 stop bit, None parity.
    UARTConfigSetExpClk(
            UART0_BASE, SysCtlClockGet(), 115200,
            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
}

//*************************** LEDS PARQUEO ***************************************
void Parqueos(void)
{
    switch (parqueo)
    {
    case 1:

        if (state[0] == 0)
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0); //Led verde parqueo 1 OFF
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1); //Led roja parqueo 1 ON

            //cont[0] = 1;
        }

        else
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0); //Led verde parqueo 1 ON
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0); //Led roja parqueo 1 OFF
           // cont[0] = 0;
        }
        break;
    case 2:

        if (state[1] == 0)
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2); //Led verde parqueo 2
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0); //Led ROJO parqueo 2 encendida
            //cont[1] = 1;

        }

        else
        {

            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0); //Led ROJO parqueo 2 encendida
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1); //Led ROJO parqueo 2 encendida
            //cont[0] = 0;
        }
        break;
    case 3:
        if (state[2] == 0)
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0); //Led ROJO parqueo 2 encendida
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3); //Led ROJO parqueo 2 encendida
            //cont[2] = 1;

        }

        else
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2); //Led ROJO parqueo 2 encendida
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0); //Led ROJO parqueo 2 encendida
           // cont[2] = 0;
        }
        break;
    case 4:
        if (state[3] == 0)
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0); //Led ROJO parqueo 2 encendida
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); //Led ROJO parqueo 2 encendida
           // cont[3] = 1;

        }

        else
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3); //Led ROJO parqueo 2 encendida
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); //Led ROJO parqueo 2 encendida
           // cont[0] = 0;
        }
        break;

    }

}


//*************************** DISPLAY ***************************************

void Display(void)
{
    suma = 4;
    int i;
    for(i=0; i<4; i++){
        if (state[i] == 0){
            suma--;
        }
    }

    switch (suma)
    {
    case 0:
        //CERO
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0); //A
        GPIOPinWrite(GPIO_PORTB_BASE,
                     GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5,
                     GPIO_PIN_5); //B
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5, 0); //E
        break;
    case 1:
        //UNO
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, OFF); //A
        GPIOPinWrite(GPIO_PORTB_BASE,
                     GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5,
                     GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5); //B
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_5); //E
        break;
    case 2:
        //DOS
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5); //A
        GPIOPinWrite(GPIO_PORTB_BASE,
                     GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5,
                     GPIO_PIN_0); //B
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5, OFF); //E
        break;
    case 3:
        //TRES
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, OFF); //A
        GPIOPinWrite(GPIO_PORTB_BASE,
                     GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5,
                     GPIO_PIN_0); //B
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_5); //E
        break;
    case 4:
        //CUATRO
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, OFF); //A
        GPIOPinWrite(GPIO_PORTB_BASE,
                     GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5,
                     GPIO_PIN_1 | GPIO_PIN_4); //B
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_5); //E
        break;
    }
}
