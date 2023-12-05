#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "funsape/peripheral/adc.hpp"

//baudrate inicial bluetooth = 38400
//Configuravel usando Comandos AT

//COMANDO AT: AT[ALGUMACOISA]\r\n
//TEST:
//AT: AT\r\n
//Responde no terminal: OK

//RESET:
//AT: AT+RESET\r\n
//Responde no terminal: OK


//CONFIG:

//NOME
//AT+NAME=<parametro>
//Responde no terminal: OK

//AT+NAME?
//Responde no terminal: <param>

//br@y terminal
int main()
{


    return 0
}
