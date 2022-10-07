#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/adc.h"


const char *TAG = "MAIN";	//message
#define BLINK_GPIO 26		//salida para el led
#define Button_GPIO 25		//entrada para el botton
int contador_pulsos=0;		//contador de pulsos del boton
//#define adc1_channel_t ADC1_CHANNEL_4;			//seleccionamos el canal del adc pin 32
//#define adc_atten_t ADC_ATTEN_DB_11;			//seleccionamos el voltage de referencia en este caso 2430mv max3500mv
//#define adc_bits_width_t ADC_WIDTH_BIT_12;		//especificamos la resolución del adc en este caso 12bits


TaskHandle_t controlador_t1=NULL;	//handle task1
TaskHandle_t controlador_t2=NULL;	//handle task2
TaskHandle_t controlador_t3=NULL;	//handle task2

void vTaskBlink(void *pvParameters)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
	gpio_reset_pin(BLINK_GPIO);
	/* Set the GPIO as a push/pull output */
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1){
    	gpio_set_level(BLINK_GPIO, 1);
    	ESP_LOGI(TAG,"LED:	1");
    	//ESP_LOGI(TAG,"Estado del led: %d", gpio_get_level(BLINK_GPIO));
    	vTaskDelay(2000 / portTICK_PERIOD_MS);

    	gpio_set_level(BLINK_GPIO, 0);
    	ESP_LOGI(TAG,"LED:	0");
    	//ESP_LOGI(TAG,"Estado del led: %d", gpio_get_level(BLINK_GPIO));
    	vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void vTaskInput(void *pvParameters)
{
	//COnfiguración del pin
	gpio_reset_pin(Button_GPIO);
	gpio_set_direction(Button_GPIO, GPIO_MODE_INPUT);
	gpio_set_pull_mode(Button_GPIO, GPIO_PULLDOWN_ONLY);

    while(1){
    	if (gpio_get_level(Button_GPIO)){
    		contador_pulsos++;
    		if (contador_pulsos>2){
    			contador_pulsos=0;
    		}
    		ESP_LOGW(TAG,"Se ha presionado el boton. El contador de pulsos es %d",contador_pulsos);
    		while(gpio_get_level(Button_GPIO));
    		if (contador_pulsos==0){
    			vTaskResume(controlador_t1);
    		    ESP_LOGW(TAG,"Todas las tareas estan activas");
    		}else if (contador_pulsos==1){
    		  	vTaskSuspend(controlador_t1);
    		  	ESP_LOGW(TAG,"Se ha suspendido la tarea 1");
    		}
    	}
    	vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}


void vTaskADC(void *vparameter){
	//configuración del adc canal y resolución etc
	adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
	adc1_config_width(ADC_WIDTH_BIT_12);

	//ahora si leemos el valor anlógico
	int lectura_dig=0;
	while(1){
		lectura_dig=adc1_get_raw(ADC1_CHANNEL_4);
		ESP_LOGI(TAG,"Lectura digital del adc: %i",lectura_dig);
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}

}

void app_main(void)
{
    //xTaskCreatePinnedToCore(vTaskBlink, "blink",2024,NULL,1,&controlador_t1,1);	//crearmos la tarea blink en core 1
    //xTaskCreatePinnedToCore(vTaskInput, "button",5024,NULL,1,&controlador_t2,1);
    xTaskCreatePinnedToCore(vTaskADC, "adc",5024,NULL,1,&controlador_t3,1);
    //vTaskDelay(10000 / portTICK_PERIOD_MS);
    //vTaskDelete(controlador_t1);		//borramos la tarea blink
    //vTaskSuspend(controlador_t1);		//pausamos la ejecución de la tarea blink
    //vTaskResume(controlador_t1);		//activamos nuevamente la ejecución de la tarea blink

}
