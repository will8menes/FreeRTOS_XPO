#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/adc.h"
#include "freertos/queue.h"
#include "comp_blink.h"


const char *tagBlink = "tagBlink";		//sms para proposito de la tarea del blink
#define BLINK_GPIO 26					//salida para el led
//TaskHandle_t controlador_t1=NULL;	//handle task1

void vTaskBlink(void *pvParameters)
{
	float data_receive=0;
	//Declaramos la cola que pasamos como parametro
	QueueHandle_t local_queque= *(QueueHandle_t *) pvParameters;

	//configuración del pin de salida para el led
	gpio_reset_pin(BLINK_GPIO);
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


	// parapadeo del led
	while(1){

    	gpio_set_level(BLINK_GPIO, 1);
    	ESP_LOGI(tagBlink,"Estado del led: %d", gpio_get_level(BLINK_GPIO));
    	vTaskDelay(1000 / portTICK_PERIOD_MS);

    	gpio_set_level(BLINK_GPIO, 0);
    	ESP_LOGI(tagBlink,"Estado del led: %d", gpio_get_level(BLINK_GPIO));
    	vTaskDelay(1000 / portTICK_PERIOD_MS);


    	// recepción del dato en cola
    	if (uxQueueMessagesWaiting(local_queque)!=0){
    		xQueueReceive(local_queque, &data_receive, pdMS_TO_TICKS(100));
    		ESP_LOGI(tagBlink,"Dato recibido de voltaje es: %f",data_receive);
    		vTaskDelay(500 / portTICK_PERIOD_MS);
    	}
    	vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
