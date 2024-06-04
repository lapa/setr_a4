
#include "leds.h"

const struct gpio_dt_spec led_0 = GPIO_DT_SPEC_GET(LED0_NODE,gpios);
const struct gpio_dt_spec led_1 = GPIO_DT_SPEC_GET(LED1_NODE,gpios);
const struct gpio_dt_spec led_2 = GPIO_DT_SPEC_GET(LED2_NODE,gpios);
const struct gpio_dt_spec led_3 = GPIO_DT_SPEC_GET(LED3_NODE,gpios);

int toggle_led(int id) {
    struct gpio_dt_spec led; 
    switch (id) {
	case 0:
	    led = led_0;
	    break;
	case 1:
	    led = led_1;
	    break;
	case 2:
	    led = led_2;
	    break;
	case 3:
	    led = led_3;
	    break;
    }
    int state = gpio_pin_get_dt(&led);
    state = 1 - state;
    int ret = gpio_pin_set_dt(&led, state);
    if (ret < 0) {
	    return 0;
    }
    return 0;
}

int set_led(int id, int value) {
    struct gpio_dt_spec led; 
    switch (id) {
	case 0:
	    led = led_0;
	    break;
	case 1:
	    led = led_1;
	    break;
	case 2:
	    led = led_2;
	    break;
	case 3:
	    led = led_3;
	    break;
    }
    int ret = gpio_pin_set_dt(&led,value);
    if (ret < 0) {
        return 0;
    }
    return 0;
}

void read_led(int id, int *res) {
    struct gpio_dt_spec led; 
    switch (id) {
	case 0:
	    led = led_0;
	    break;
	case 1:
	    led = led_1;
	    break;
	case 2:
	    led = led_2;
	    break;
	case 3:
	    led = led_3;
	    break;
    }
    *res = gpio_pin_get_dt(&led);
}

int configure_leds(void) {
    int ret = 0;
    if (!device_is_ready(led_0.port))  
    {
    printk("Fatal error: led0 device not ready!");
    	return ERR_RDY;
    }
    if (!device_is_ready(led_1.port))  
    {
    printk("Fatal error: led1 device not ready!");
    	return ERR_RDY;
    }
    if (!device_is_ready(led_2.port))  
    {
    printk("Fatal error: led2 device not ready!");
    	return ERR_RDY;
    }
    if (!device_is_ready(led_3.port))  
    {
    printk("Fatal error: led3 device not ready!");
    	return ERR_RDY;
    }

    ret = gpio_pin_configure_dt(&led_0, GPIO_OUTPUT_INACTIVE);
    if(ret < 0) {
    	return 0;
    }
    ret = gpio_pin_configure_dt(&led_1, GPIO_OUTPUT_INACTIVE);
    if(ret < 0) {
    	return 0;
    }
    ret = gpio_pin_configure_dt(&led_2, GPIO_OUTPUT_INACTIVE);
    if(ret < 0) {
    	return 0;
    }
    ret = gpio_pin_configure_dt(&led_3, GPIO_OUTPUT_INACTIVE);
    if(ret < 0) {
    	return 0;
    }
    return 0;

}
