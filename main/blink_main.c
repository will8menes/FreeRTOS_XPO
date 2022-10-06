#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"


#define BLINK_GPIO 26
#define Button_GPIO 25

const char *TAG = "MAIN";
TaskHandle_t controlador_t1=NULL;
TaskHandle_t controlador_t2=NULL;

void vTaskBlink(void *pvParameters)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
	gpio_reset_pin(BLINK_GPIO);
	/* Set the GPIO as a push/pull output */
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1){
    	ESP_LOGI(TAG,"encendiendo el led");
    	gpio_set_level(BLINK_GPIO, 1);
    	vTaskDelay(2000 / portTICK_PERIOD_MS);

    	ESP_LOGW(TAG,"apagando el led");
    	gpio_set_level(BLINK_GPIO, 0);
    	vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void vTaskInput(void *pvParameters)
{
    int contador_pulsos=0;

	//COnfiguración del pin
	gpio_reset_pin(Button_GPIO);
	gpio_set_direction(Button_GPIO, GPIO_MODE_INPUT);
	gpio_set_pull_mode(Button_GPIO, GPIO_PULLDOWN_ONLY);

    while(1){
    	if (gpio_get_level(Button_GPIO)){
    		ESP_LOGI(TAG,"Se ha presionado el boton");
    		contador_pulsos++;
    		if (contador_pulsos>10){
    			ESP_LOGE(TAG,"Se ha detenido la tarea button");
    			vTaskDelete(controlador_t2);
    		    contador_pulsos=0;
    		}
    		while(gpio_get_level(Button_GPIO));
    	}
    	vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}


void app_main(void)
{
    //xTaskCreatePinnedToCore(vTaskBlink, "blink",2024,NULL,1,&controlador_t1,1);	//crearmos la tarea blink en core 1
    xTaskCreatePinnedToCore(vTaskInput, "button",5024,NULL,1,&controlador_t2,1);
    //vTaskDelay(10000 / portTICK_PERIOD_MS);
    //vTaskDelete(controlador_t1);		//borramos la tarea blink
    //vTaskSuspend(controlador_t1);		//pausamos la ejecución de la tarea blink
    //vTaskResume(controlador_t1);		//activamos nuevamente la ejecución de la tarea blink

}
