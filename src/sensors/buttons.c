
#include "buttons.h"

const struct gpio_dt_spec but0_dev = GPIO_DT_SPEC_GET(BUT0_NID,gpios);
const struct gpio_dt_spec but1_dev = GPIO_DT_SPEC_GET(BUT1_NID,gpios);
const struct gpio_dt_spec but2_dev = GPIO_DT_SPEC_GET(BUT2_NID,gpios);
const struct gpio_dt_spec but3_dev = GPIO_DT_SPEC_GET(BUT3_NID,gpios);

int configure_buttons(void) {
    if (!device_is_ready(but0_dev.port))  
	{
        printk("Fatal error: but0 device not ready!");
		return ERR_RDY;
	}

    if (!device_is_ready(but1_dev.port))  
	{
        printk("Fatal error: but1 device not ready!");
		return ERR_RDY;
	}

    if (!device_is_ready(but2_dev.port))  
	{
        printk("Fatal error: but2 device not ready!");
        return ERR_RDY;
	}

    if (!device_is_ready(but3_dev.port))  
	{
        printk("Fatal error: but3 device not ready!");
		return ERR_RDY;
	}

    int ret = 0;
    
    ret = gpio_pin_configure_dt(&but_0, GPIO_INPUT);
    if(ret < 0) {
    	return 0;
    }
    ret = gpio_pin_configure_dt(&but_1, GPIO_INPUT);
    if(ret < 0) {
    	return 0;
    }
    ret = gpio_pin_configure_dt(&but_2, GPIO_INPUT);
    if(ret < 0) {
    	return 0;
    }
    ret = gpio_pin_configure_dt(&but_3, GPIO_INPUT);
    if(ret < 0) {
    	return 0;
    }

}
