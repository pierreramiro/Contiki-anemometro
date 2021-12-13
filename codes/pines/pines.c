#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/button-hal.h"
#include "button-sensor.h"
#include "dev/gpio-hal.h"
#include <stdio.h>
#include <stdint.h>
static struct etimer tempo;//creamos el contador

PROCESS (pines_process, "Proceso pines");
AUTOSTART_PROCESSES(&pines_process);

PROCESS_THREAD(pines_process,ev,data)
{
    PROCESS_BEGIN();
    gpio_hal_arch_pin_set_output(0,18);
    etimer_set(&tempo,CLOCK_SECOND/2);
    printf("hola");
    leds_off(LEDS_ALL);
    leds_single_on(LEDS_RED);
    while(1){
        leds_toggle(LEDS_GREEN);
        leds_toggle(LEDS_RED);
        gpio_hal_arch_toggle_pin(0,18);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&tempo));
        etimer_reset(&tempo);
    }
    PROCESS_END();
}