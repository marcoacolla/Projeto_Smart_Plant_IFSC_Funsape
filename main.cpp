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


adc.enable();
adc.setReference(Adc::Reference::POWER_SUPPLY);
adc.setPrescaler(Adc::Prescaler::PRESCALER_128);
adc.setMode(Adc::Mode::AUTO_CONTINUOUS);
adc.setChannel(Adc::Channel::CHANNEL_0);
adc.disableDigitalInput(Adc::DigitalInput::INPUT_0);
adc.activateInterrupt();
adc.startConversion();
sei();
//br@y terminal
int main()
{


    return 0
}
