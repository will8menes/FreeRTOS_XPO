#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/adc.h"
#include "freertos/queue.h"
#include "comp_adc.h"


const char *tagADC = "tagADC";			//sms para proposito de la tarea del adc
//TaskHandle_t controlador_t2=NULL;		//handle task2


void vTaskADC(void *pvParameters){
	//creamos la cola para enviar los datos dig del adc con 10 espacios y de tipo int

	//configuración del adc canal y resolución etc
	adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11); 		//canal 4 correspondiente al pin 32, nivel de atenuación es el valor del voltaje de ref maxímo en este caso el nivel 11 corresponde a un max de 3500mV
	adc1_config_width(ADC_WIDTH_BIT_12);								//es al resulción del adc, en este caso de 12 bits

	/////
	QueueHandle_t local_queque= *(QueueHandle_t *) pvParameters;
	/////

	float lect_an=0;
	int lect_dig=0;													//variable para guardar el valor del adc digital 0-4095
	while(1){
		lect_dig=adc1_get_raw(ADC1_CHANNEL_4);
		lect_an=lect_dig*(3.3/4095);

		if(lect_an>2.5){
			//enviamos el valor del adc al task blink
			xQueueSend(local_queque,&lect_an,pdMS_TO_TICKS(100));
			//ESP_LOGW(tagADC,"Dato enviado al blink: %d",lect_dig);
		}else{
			ESP_LOGW(tagADC,"Voltaje del ADC: %f",lect_an);
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

}
