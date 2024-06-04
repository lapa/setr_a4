
#include "buttons.h"

const struct gpio_dt_spec but_0 = GPIO_DT_SPEC_GET(BUT0_NODE,gpios);
const struct gpio_dt_spec but_1 = GPIO_DT_SPEC_GET(BUT1_NODE,gpios);
const struct gpio_dt_spec but_2 = GPIO_DT_SPEC_GET(BUT2_NODE,gpios);
const struct gpio_dt_spec but_3 = GPIO_DT_SPEC_GET(BUT3_NODE,gpios);

int read_button(int id, int *res) {
    struct gpio_dt_spec but;
    switch (id) {
	case 0:
	    but = but_0;
	    break;
	case 1:
	    but = but_1;
	    break;
	case 2:
	    but = but_2;
	    break;
	case 3:
	    but = but_3;
	    break;
    }
    *res = gpio_pin_get_dt(&but);
    return 0;
}

int configure_buttons(void) {
    if (!device_is_ready(but_0.port))  
	{
        printk("Fatal error: but0 device not ready!");
		return ERR_RDY;
	}

    if (!device_is_ready(but_1.port))  
	{
        printk("Fatal error: but1 device not ready!");
		return ERR_RDY;
	}

    if (!device_is_ready(but_2.port))  
	{
        printk("Fatal error: but2 device not ready!");
        return ERR_RDY;
	}

    if (!device_is_ready(but_3.port))  
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
    return 0;
}
