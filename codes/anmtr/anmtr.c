#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-hal.h"
#include "button-sensor.h"
#include "dev/gpio-hal.h"
#include "dev/spi.h"
#include <stdio.h>
#include <stdint.h>

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
const uint16_t address_data[9]={0xC6,0x46,0x20,0x53,0x1F,0x84,0x00,0x59,0x00};
spi_device_t *spi;

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
    spi->pin_spi_sck=  IOID_10;
    spi->pin_spi_miso= IOID_8;
    spi->pin_spi_miso= IOID_9;
    spi->pin_spi_cs= CSB0_pin;
    spi->spi_bit_rate=1000000;
    spi->spi_pha =1;
    spi->spi_pol=1;
    spi->spi_controller=SPI_CONTROLLER_SPI0;   
    
    //Iniciamos el proceso
    PROCESS_BEGIN();
    //Habilitamso el TDC1 para configurar

    //Enviamos los datos de configuracion
    spi_acquire(spi);
    //spi_arch_transfer(spi,address_data,9,0,0,0);
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