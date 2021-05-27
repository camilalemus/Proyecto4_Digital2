//*******************************************************
// Camila Lemus Lone
// 18272
// Universidad del Valle de Guatemala
//*******************************************************

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

//****VARIABLES***

//bool state = false;
//bool stateantes = true;
bool flag = false;
int led;
char letra;
int parqueo;
int parqueo1;
int parqueo2;
int parqueo3;
int parqueo4;
int contador;


int pushP[] = {GPIO_PORTF_BASE,GPIO_PORTF_BASE,GPIO_PORTB_BASE,GPIO_PORTC_BASE};
int pushV[] = {GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_3,GPIO_PIN_4};
int state[] ={1,1,1,1};
int stateantes[] ={1,1,1,1};

//GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2); //Lee push 1
//GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3); //Lee push 2
//GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3); //Lee push 3
//GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4); //Lee push 4

#define RED GPIO_PIN_1
#define GREEN GPIO_PIN_3
#define BLUE GPIO_PIN_2
#define OFF 0
#define ON 0



// Display
//-----------
// PB5
// PB0
// PB1
// PE4
// PE5
// PB4
// PA5

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

//***INSTANCIAR FUNCIONES***

void UART_Config(void);
void Parqueos(void);

/**
 * main.c
 */


//*********

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); // Clk a 40MHz
    //UART_Config();

    //Peripheral Enable
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //pushes
    GPIOPinConfigure(GPIO_PF2_T1CCP0);
    GPIOPinConfigure(GPIO_PF3_T1CCP1);//2
    GPIOPinConfigure(GPIO_PB3_T3CCP1);//3
    GPIOPinConfigure(GPIO_PC4_WT0CCP0);//4


    //leds

    GPIOPinConfigure(GPIO_PD0_WT2CCP0); //Parque 1 verde


    //Salidas y entradas
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5); //A
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5); //B
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //D
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5); //E
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); //F

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3); //B
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4); //C
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_3); //F

    //Push Button - Pullup
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0); //Led verde parqueo 1 apagada
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1); //Led verde parqueo 2 apagada
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2); //Led verde parqueo 3 apagada
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3); //Led verde parqueo 4 apagada

        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0); //Led roja parqueo 1 encendida
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0); //Led roja parqueo 2 encendida
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0); //Led roja parqueo 3 encendida
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); //Led roja parqueo 4 encendida




        int n=0;

    while (1)
        {



 //       if (n<4){
//            n++;
 //       }

//        else {
//            n=0;
//        }
      //  contador = parqueo1 + parqueo2 + parqueo3 + parqueo4;
                        state[n] = GPIOPinRead(pushP[n], pushV[n]); //Lee push
                        if (state[n] != stateantes[n]){
                            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2); //Led roja parqueo 2 encendida
                            stateantes[n] = state[n];
                            parqueo = n+1;

                            Parqueos();
                        }
                        else{

                        }



        }
    }

//**** UART *****



void UART_Config(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //Peripheral del UART enable
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //Peripheral del puerto A enable
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1); //Los pines TX y RX se ponen en modo UART
        //Inicializar el modulo UART con: 115200, 8 data bits, 1 stop bit, None parity.
        UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
}

//**** LEDS PARQUEO *****
void Parqueos(void){
    switch (parqueo){
        case 1:

            if (state==0){
                GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0); //Led verde parqueo 1 encendida
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1); //Led verde parqueo 1 encendida
            }

            else {
                GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0); //Led roja parqueo 1 encendida
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0); //Led verde parqueo 1 encendida
            }

            parqueo1 = 1;
            break;
        case 2:


            if (state==0){
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2); //Led ROJO parqueo 2 encendida
                GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0); //Led ROJO parqueo 2 encendida


            }

            else {

                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0); //Led ROJO parqueo 2 encendida
                GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1 ); //Led ROJO parqueo 2 encendida
            }

            parqueo2 = 1;
            break;
        case 3:
            if (state==0){
                GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0); //Led ROJO parqueo 2 encendida
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3); //Led ROJO parqueo 2 encendida


            }

            else {
                GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2); //Led ROJO parqueo 2 encendida
                GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0); //Led ROJO parqueo 2 encendida
            }


            parqueo3 = 3;
            break;
        case 4:
            if (state==0){
                           GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0); //Led ROJO parqueo 2 encendida
                           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); //Led ROJO parqueo 2 encendida


                       }

                       else {
                           GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3); //Led ROJO parqueo 2 encendida
                           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); //Led ROJO parqueo 2 encendida
                       }
            parqueo4 = 1;
            break;

    }
}