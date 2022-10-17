#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/adc.h"
#include "freertos/queue.h"
#include "comp_input.h"
#include "comp_pzem.h"




const char *tagInput = "INPUT";				//sms para proposito de entradas
#define Button_GPIO 25						//entrada para el botton
int contador_pulsos=0;						//contador de pulsos del boton

void vTaskInput(void *pvParameters)
{
	struct controlador{
		TaskHandle_t ct1;
		TaskHandle_t ct2;
	};
	struct controlador *control = (struct controlador*) pvParameters;


	//COnfiguración del pin
	gpio_reset_pin(Button_GPIO);
	gpio_set_direction(Button_GPIO, GPIO_MODE_INPUT);
	gpio_set_pull_mode(Button_GPIO, GPIO_PULLDOWN_ONLY);
	int timer=0;
    while(1){
    	if (gpio_get_level(Button_GPIO)){
    		contador_pulsos++;
    		if (contador_pulsos>2){
    			contador_pulsos=0;
    		}
    		ESP_LOGE(tagInput,"Se ha presionado el boton. El contador de pulsos es %d",contador_pulsos);

    		//mientras se mantenga presionado el boton se queda en bucle infinito hasta soltarlo
    		//ademas cuenta si pasaron 5s, en este caso guarda la info del pzem en nvs
    		while(gpio_get_level(Button_GPIO)){
    			vTaskDelay(50 / portTICK_PERIOD_MS);
    			timer++;
    			if (timer>=100){
    				//ESP_LOGE(tagInput,"guardar accionando");
    				guardar_pzem();
    				break;
    			}

    		}
    		timer=0;

    		//verificamos el contador para pausar o activar cada tarea
    		if (contador_pulsos==0){
    			vTaskResume(control->ct1);
    			vTaskResume(control->ct2);
    		    ESP_LOGE(tagInput,"Todas las tareas estan activas");
    		}else if (contador_pulsos==1){
    		  	vTaskSuspend(control->ct1);
    		  	vTaskResume(control->ct2);
    		  	ESP_LOGE(tagInput,"Se ha suspendido la tarea 1");
    		}else if (contador_pulsos==2){
    		  	vTaskSuspend(control->ct2);
    		  	vTaskResume(control->ct1);
    		  	ESP_LOGE(tagInput,"Se ha suspendido la tarea 2");
    		}
    	}
    	vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

