#include "contiki.h"
#include "micontador.h"
#include "sys/etimer.h"
#include <stdio.h>

PROCESS(conteo,"Proceso de conteo");
AUTOSTART_PROCESSES(&conteo);
static int contador=0;
    
PROCESS_THREAD(conteo,ev,data)
{
    static struct etimer tempo;//creamos el contador
    PROCESS_BEGIN();
    etimer_set(&tempo,CLOCK_SECOND*1);
    printf("comienza el conteo\n");
    while (1)
    {
        sumaconteo(&contador);
        printf("%d\n",contador);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&tempo));
        etimer_reset(&tempo);
    }
    PROCESS_END();

}