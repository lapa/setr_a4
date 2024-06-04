
#include "adc.h"


const struct device *adc_dev = DEVICE_DT_GET(ADC_NODE);	

/* ADC channel configuration */
static const struct adc_channel_cfg my_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_CHANNEL_ID,
	.input_positive = ADC_CHANNEL_INPUT
};

/* Global vars */
struct k_timer my_timer;
static uint16_t adc_sample_buffer[BUFFER_SIZE];

K_THREAD_STACK_DEFINE(thread_ADC_stack, STACK_SIZE);
struct k_thread thread_ADC_data;
k_tid_t thread_ADC_tid;

int adc_sample(void)
{
	int ret;
	const struct adc_sequence sequence = {
		.channels = BIT(ADC_CHANNEL_ID),
		.buffer = adc_sample_buffer,
		.buffer_size = sizeof(adc_sample_buffer),
		.resolution = ADC_RESOLUTION,
	};

	if (adc_dev == NULL) {
            printk("adc_sample(): error, must bind to adc first \n\r");
            return -1;
	}

	ret = adc_read(adc_dev, &sequence);
	if (ret) {
            printk("adc_read() failed with code %d\n", ret);
	}	

	return ret;
}

void thread_ADC_code(void *argA, void *argB, void *argC) {

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

        err=adc_sample();
        if(err) {
            printk("adc_sample() failed with error code %d\n\r",err);
        }
        else {
            if(adc_sample_buffer[0] > 1023) {
                printk("adc reading out of range (value is %u)\n\r", adc_sample_buffer[0]);
            }
            else {
                /* ADC is set to use gain of 1/4 and reference VDD/4, so input range is 0...VDD (3 V), with 10 bit resolution */
                printk("adc reading: raw:%4u / %4u mV: \n\r",adc_sample_buffer[0],(uint16_t)(1000*adc_sample_buffer[0]*((float)3/1023)));
            }
        }
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

int configure_adc(void) {

   int err = 0;

    err = adc_channel_setup(adc_dev, &my_channel_cfg);
    if (err) {
        printk("adc_channel_setup() failed with error code %d\n", err);
        return ERR_CONFIG;
    }
    NRF_SAADC->TASKS_CALIBRATEOFFSET = 1;
        /* Main loop */
    thread_ADC_tid = k_thread_create(&thread_ADC_data, thread_ADC_stack,
        K_THREAD_STACK_SIZEOF(thread_ADC_stack), thread_ADC_code,
        NULL, NULL, NULL, thread_ADC_prio, 0, K_NO_WAIT);

    /* Should never reach this place ...*/
    return ERR_OK;
}



