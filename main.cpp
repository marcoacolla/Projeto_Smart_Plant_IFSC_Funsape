#define F_CPU 16000000UL

#include "funsape/globalDefines.hpp"
#include "funsape/device/hd44780.hpp"
#include "funsape/peripheral/timer0.hpp"
#include "funsape/peripheral/timer1.hpp"
#include "funsape/peripheral/usart0.hpp"
#include "fatFs/ff.h"
#include "fatFs/mmc_avr.h"
#include <string.h>

//bluetooth HC05 componente 101 datasheet
//baudrate inicial = 38400
//Configuracao API ou AT
//AT: Inicia com AT e termina com /r/n - responde com OK (está funcionando)
//Alguns comandos precisam de paramentro adicional
// = separa o comando do parâmetro, se colocar ? retorna o GET se for = dá POST
//param 0 master param 1 slave

vuint8_t counter = 1;
vuint8_t umidade = 22;

FATFS disk;
FRESULT res;
FIL file;
char strBuffer[50];
UINT bytesWritten;

Hd44780 lcd;

typedef struct {
    uint8_t dia;
    uint8_t mes;
    uint16_t ano;
    uint8_t hora;
    uint8_t minuto;
    uint8_t segundo;
} DataHora;

volatile DataHora data;

int main()
{
    //Configuração LCD
    lcd.dataPortSet(&DDRD, PD2);
    lcd.controlPortSet(&DDRC, PC2, PC0, PC1);
    lcd.init(Hd44780::Size::LCD_16X2, Hd44780::Font::FONT_5X8, true, true);

    //Configuração Timer0
    timer0.setCompareAValue(157);
    timer0.init(Timer0::Mode::CTC_OCRA, Timer0::ClockSource::PRESCALER_1024);
    timer0.activateCompareAInterrupt();

    timer1.init(Timer1::Mode::CTC_OCRA, Timer1::ClockSource::PRESCALER_1024);
    timer1.setCompareAValue(15619);
    timer1.activateCompareAInterrupt();

    // Usart configuration
    usart0.enableTransmitter();
    usart0.init();
    usart0.stdio();
    sei();

    printf("[OK] USART configuration!\r");

    sdCardCreate(LOCATION_PB2);

    res = f_mount(&disk, "", 0);
    if(res != FR_OK) {
        printf("[FAIL] SD card not mounted: %d\r", res);
    } else {
        printf("[ OK ] SD card mounted!\r");
    }

    res = f_open(&file, "out_file.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if(res != FR_OK) {
        printf("[FAIL] File not created: %d\r", res);
    } else {
        printf("[ OK ] File created!\r");
    }

    while(1) {

    }

    return 0;
}

void timer1CompareACallback(void)
{
    if(counter == 10) {
        counter = 1;
        lcd.stdio();
        printf("    Umidade:\n    %d kg/m3", umidade);
        usart0.stdio();
        //Escreve dados de umidade com data/hora
        sprintf(strBuffer, "%02d/%02d/%04d %02d:%02d:%02d - Umidade = %d kg/m3.\r\n", data.dia, data.mes, data.ano, data.hora,
                data.minuto, data.segundo, umidade);

        res = f_write(&file, strBuffer, strlen(strBuffer), &bytesWritten);
        if(res != FR_OK) {
            printf("[FAIL] Data not written: %d\r", res);
        } else {
            printf("[ OK ] Data written (%d bytes)!\r", bytesWritten);
        }

        res = f_sync(&file);
        if(res != FR_OK) {
            printf("[FAIL] File not saved: %d\r", res);
        } else {
            printf("[ OK ] File saved!\r");
        }

    } else {
        counter++;
    }
}

void timer0CompareACallback(void)
{
    mmc_disk_timerproc();
}
