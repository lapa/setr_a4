
#include "buttons.h"

const struct gpio_dt_spec but_0 = GPIO_DT_SPEC_GET(BUT0_NODE,gpios);
const struct gpio_dt_spec but_1 = GPIO_DT_SPEC_GET(BUT1_NODE,gpios);
const struct gpio_dt_spec but_2 = GPIO_DT_SPEC_GET(BUT2_NODE,gpios);
const struct gpio_dt_spec but_3 = GPIO_DT_SPEC_GET(BUT3_NODE,gpios);


void thread_button_read_code(void *argA, void *argB, void *argC) {

    timing_t fin_time=0, release_time=0;
    
    /* Variables to time execution */
    timing_t start_time, end_time;
    uint64_t total_cycles=0;
    uint64_t total_ns=0;

    printk("Thread A init (periodic)\n");

    /* Compute next release instant */
    release_time = k_uptime_get() + thread_ADC_period;

    int err = 0;
    while(true){
        /* Get one sample, checks for errors and prints the values */
	start_time = timing_counter_get(); 

    	*res = gpio_pin_get_dt(&but);
	//FAZ ISTO PARA TODOS OS BUTÕES

        end_time = timing_counter_get();

        total_cycles = timing_cycles_get(&start_time, &end_time);
        total_ns = timing_cycles_to_ns(total_cycles);
       
        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time); /* There are other variants, k_sleep(), k_usleep(), ... */       
        }
        /* Update next release time instant*/
        release_time += thread_ADC_period;
        //k_msleep(TIMER_INTERVAL_MSEC);
    }
    timing_stop();
}


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
