#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/adc.h"
#include "freertos/queue.h"

#include "comp_adc.h"
#include "comp_blink.h"
#include "comp_input.h"
#include "comp_pzem.h"
#include "comp_dht.h"

#include "comp_save_nvs.h"
#include "nvs_flash.h"
#include "nvs.h"


const char *TAG = "MAIN";				//sms para proposito general

int memory_task=1024;
QueueHandle_t GlobalQueue=0;			//declaramos una cola para todas las colas


void app_main(void)
{
	//iniciamos la particion para guardar los datos en memoria no volatil
	nvs_flash_init();

	//creamos la cola para enviar los datos dig del adc con 10 espacios y de tipo int
	GlobalQueue = xQueueCreate(10,sizeof(int));

	//esta estructura nos sirve para enviar como parametros los dos controladores para detener las tareas
	struct controlador{
		TaskHandle_t ct1;
		TaskHandle_t ct2;
	};
	struct controlador *control =malloc(sizeof(*control));
	control->ct1=NULL;
	control->ct2=NULL;

	xTaskCreatePinnedToCore(vTaskBlink, "blink",memory_task*2,&GlobalQueue,1,&control->ct1,1);
    xTaskCreatePinnedToCore(vTaskADC, "adc",memory_task*2,&GlobalQueue,1,&control->ct2,1);
    xTaskCreatePinnedToCore(vTaskInput, "button",memory_task*3,(void *) control,1,NULL,1);

    xTaskCreatePinnedToCore(vTask_pzem, "PZEM",memory_task*2,NULL,1,NULL,1);
    xTaskCreatePinnedToCore(vTask_dht, "DHT",memory_task*2,NULL,1,NULL,1);

    //vTaskDelay(10000 / portTICK_PERIOD_MS);
    //vTaskDelete(controlador_t1);		//borramos la tarea blink
    //vTaskSuspend(controlador_t1);		//pausamos la ejecución de la tarea blink
    //vTaskResume(controlador_t1);		//activamos nuevamente la ejecución de la tarea blink



}
