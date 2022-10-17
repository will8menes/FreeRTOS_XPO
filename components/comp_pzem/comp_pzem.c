#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <comp_pzem.h>
#include <pzem_service.h>
#include "comp_save_nvs.h"


static struct pzem_values data_pzem;
static const char *tag_pzem = "PZEM";

void vTask_pzem(void *pvParameters)
{
	PZEM_init();
	int i=0;
	ESP_LOGE(tag_pzem,"*--------DATOS EXTRAIDOS ANTES DE LEER MEMORIA------------");
	ESP_LOGI(tag_pzem,"Voltaje:			%f",data_pzem.voltage);
	ESP_LOGI(tag_pzem,"COrriente:			%f",data_pzem.current);
	ESP_LOGI(tag_pzem,"Potencia:			%f",data_pzem.power);
	ESP_LOGI(tag_pzem,"Energia:			%f",data_pzem.energy);
	ESP_LOGI(tag_pzem,"Frecuencia:		%f",data_pzem.frequency);
	ESP_LOGI(tag_pzem,"Facotor pot:		%f",data_pzem.pf);
	vTaskDelay(3500 / portTICK_PERIOD_MS);
	nvs_read_pzem(&data_pzem);

	ESP_LOGE(tag_pzem,"*--------DATOS EXTRAIDOS DE MEMORIA------------");
	ESP_LOGI(tag_pzem,"Voltaje:			%f",data_pzem.voltage);
	ESP_LOGI(tag_pzem,"COrriente:			%f",data_pzem.current);
	ESP_LOGI(tag_pzem,"Potencia:			%f",data_pzem.power);
	ESP_LOGI(tag_pzem,"Energia:			%f",data_pzem.energy);
	ESP_LOGI(tag_pzem,"Frecuencia:		%f",data_pzem.frequency);
	ESP_LOGI(tag_pzem,"Facotor pot:		%f",data_pzem.pf);
	ESP_LOGE(tag_pzem,"*--------DATOS EXTRAIDOS DEl SENSOR------------");
	vTaskDelay(3500 / portTICK_PERIOD_MS);

	while (1){
		if (PZEM_updateValues(0xA2)){
			data_pzem = _currentValues;
			ESP_LOGI(tag_pzem,"el valor de la voltaje es: 	%f",data_pzem.voltage);
			ESP_LOGI(tag_pzem,"el valor de la corriente es: 	%f",data_pzem.current);
			ESP_LOGI(tag_pzem,"el valor de la potencia es: 	%f",data_pzem.power);
			ESP_LOGI(tag_pzem,"el valor de la energia es: 	%f",data_pzem.energy);
			ESP_LOGI(tag_pzem,"el valor de la frecuencia es:	%f",data_pzem.frequency);
			ESP_LOGI(tag_pzem,"el valor de la fp es: 		%f",data_pzem.pf);
			ESP_LOGI(tag_pzem,"Alarmas activas: %d", data_pzem.alarms);
		}else{
			ESP_LOGW(tag_pzem,"Alarmas activas: %d", data_pzem.alarms);
			ESP_LOGE(tag_pzem,"NO se ha recibido un dato");
		}
		i++;
		vTaskDelay(1500 / portTICK_PERIOD_MS);
		/*
		if (i>10){
			i=0;
			fflush(stdout);
			esp_restart();
		}
		*/
	}
}

void guardar_pzem(void){
	nvs_save_pzem(&data_pzem);
}
