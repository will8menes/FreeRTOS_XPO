#include <stdio.h>
#include "comp_dht.h"
#include "dht_service.h"
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



static const char *tag_dht = "DHT";
static float humedad;
static float temp;

#define TEMP_SENSOR_DATA_PIN CONFIG_DHT_PIN
#define GPIO_TEMP_PIN_SEL  ((1ULL<<TEMP_SENSOR_DATA_PIN))
#define DHT_TYPE CONFIG_SENSOR_TYPE

void vTask_dht(void *pvParameters)
{
	gpio_config_t io_conf = {};
	//bit mask
	io_conf.pin_bit_mask = GPIO_TEMP_PIN_SEL;
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_up_en = 1;
	gpio_config(&io_conf);

	while (1)
	{
		if(!dht_read_float_data(DHT_TYPE,TEMP_SENSOR_DATA_PIN,&humedad,&temp)){
			ESP_LOGW(tag_dht,"Humedad %f: Temperatura %f",humedad, temp);;
		}else{
			ESP_LOGE(tag_dht,"No hay valores de HUMEDAD");
		}

		vTaskDelay(1500 / portTICK_PERIOD_MS);
	}
}
