
#include "leds.h"

const struct gpio_dt_spec led0_dev = GPIO_DT_SPEC_GET(LED0_NID,gpios);
const struct gpio_dt_spec led1_dev = GPIO_DT_SPEC_GET(LED1_NID,gpios);
const struct gpio_dt_spec led2_dev = GPIO_DT_SPEC_GET(LED2_NID,gpios);
const struct gpio_dt_spec led3_dev = GPIO_DT_SPEC_GET(LED3_NID,gpios);

uint16_t set_led(uint16_t id) {

}

uint16_t read_led(uint16_t id, uint16_t *res) {

}

int configure_leds(void) {
    if (!device_is_ready(led0_dev.port))  
    {
    printk("Fatal error: led0 device not ready!");
    	return ERR_RDY;
    }
    if (!device_is_ready(led1_dev.port))  
    {
    printk("Fatal error: led1 device not ready!");
    	return ERR_RDY;
    }
    if (!device_is_ready(led2_dev.port))  
    {
    printk("Fatal error: led2 device not ready!");
    	return ERR_RDY;
    }
    if (!device_is_ready(led3_dev.port))  
    {
    printk("Fatal error: led3 device not ready!");
    	return ERR_RDY;
    }
}
