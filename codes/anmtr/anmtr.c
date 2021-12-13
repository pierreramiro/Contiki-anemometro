#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-hal.h"
#include "button-sensor.h"
#include "dev/gpio-hal.h"
#include <stdio.h>
#include <stdint.h>
#include "ti/drivers/SPI.h"//Usar esta libreria para 16 bits
//#include "dev/spi.h"//Quitar esta libreria usar la de 16 bits

//Definimos pines
//SPI selector
#define CSB0_pin    IOID_11  //Modificar el valor de estos pines en el código al establecer los SPI
#define CSB1_pin    IOID_12
//pines SM Control Unit
#define ERRB_pin    IOID_13
#define SS_pin      IOID_14//START STOP PIN

#define sel_pin     IOID_15
#define EN_pin      IOID_20
#define TRIGGER_pin IOID_19
#define RESET_pin   IOID_18
//pines mux
#define sel0mux_pin IOID_22//TDC0
#define sel1mux_pin IOID_21//TDC1
//pin led
#define LEDR_pin     IOID_6//pin rojo
#define LEDG_pin     IOID_7//pin verde

static struct etimer tempo;//creamos el contador
uint16_t address_data[9]={0xC6,0x46,0x20,0x53,0x1F,0x84,0x00,0x59,0x00};

PROCESS (medir_viento_process, "Medicion del viento");
AUTOSTART_PROCESSES(&medir_viento_process);

PROCESS_THREAD(medir_viento_process,ev,data)
{
    
    //Definimos los pines de entrada
    gpio_hal_arch_pin_set_input(0,ERRB_pin);
    gpio_hal_arch_pin_set_input(0,SS_pin);
    //Definimos los pines de salida
    gpio_hal_arch_pin_set_output(0,sel_pin);
    gpio_hal_arch_pin_set_output(0,EN_pin);
    gpio_hal_arch_pin_set_output(0,TRIGGER_pin);
    gpio_hal_arch_pin_set_output(0,RESET_pin);

    gpio_hal_arch_pin_set_output(0,sel0mux_pin);
    gpio_hal_arch_pin_set_output(0,sel1mux_pin);

    gpio_hal_arch_pin_set_output(0,LEDR_pin);
    gpio_hal_arch_pin_set_output(0,LEDG_pin);
    
    //Configuramos los parametros del SPI
    SPI_Handle      spi;
    SPI_Params      spiParams;
    SPI_Transaction spiTransaction;
    //uint16_t        transmitBuffer[9];
    //uint16_t        receiveBuffer[16];
    bool            transferOK;
    SPI_init();  // Initialize the SPI driver
    SPI_Params_init(&spiParams);  // Initialize SPI parameters
    spiParams.dataSize = 16;       // 16-bit data size
    spiParams.transferMode        = SPI_MODE_BLOCKING;
    spiParams.transferTimeout     = SPI_WAIT_FOREVER;//be careful with this
    spiParams.transferCallbackFxn = NULL;
    spiParams.mode                = SPI_MASTER;
    spiParams.dataSize            = 16; //en bits
    spiParams.frameFormat         = SPI_POL1_PHA1;
    spiParams.bitRate             = 4000000; /* bitRate */
    //spi->pin_spi_cs= CSB0_pin;
    
    spi = SPI_open(0, &spiParams);
    if (spi == NULL) {
        while (1);  // SPI_open() failed
    }
      
    //Iniciamos el proceso
    PROCESS_BEGIN();
    //Habilitamso el TDC1 para configurar

    //Enviamos los datos de configuracion
    spiTransaction.count = 9;
    spiTransaction.txBuf = (void *)address_data;
    transferOK = SPI_transfer(spi, &spiTransaction);
    if (!transferOK) {
    // Error in SPI or transfer already in progress.
        while (1);
    }
    etimer_set(&tempo,CLOCK_SECOND/2);
    printf("Terminamos de enviar el SPI. activamos modo stand by\n");
    gpio_hal_arch_toggle_pin(0,LEDG_pin);
    while(1){
        gpio_hal_arch_toggle_pin(0,LEDR_pin);
        gpio_hal_arch_toggle_pin(0,LEDG_pin);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&tempo));
        etimer_reset(&tempo);
    }
    PROCESS_END();
}