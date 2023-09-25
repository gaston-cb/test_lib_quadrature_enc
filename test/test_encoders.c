#include "unity.h"
#include "quadrature_encoders.h"
#include "mock_gpio.h"
#include "mock_irq.h"
#include "mock_timer.h"


#define PORTA 10 
#define PORTB 11 

//// DEFINED A STATIC VARIABLE OUT OF CONTEXT TEST 
extern encoder_quad_t encoder ; 
extern volatile uint8_t count_state      ; 
extern volatile uint16_t _port_channel_a ; 
extern volatile uint16_t _port_channel_b ; 
extern volatile uint64_t clock_speed ; ; 
//extern void gpio_callback_channel_ab(unsigned int gpio,uint32_t event_mask ) ; 

void set_sequence(uint8_t porta_value,uint8_t portb_value,int value){ 
    //printf("line call is %d",value) ; 
    gpio_get_ExpectAndReturn(PORTA, porta_value) ; 
    gpio_get_ExpectAndReturn(PORTB, portb_value) ; 

}




/// @brief  DATA FOR A TEST 
encoder_quad_t encoder_test ; 

void setUp(void) {}



void test_init_ports(void){ 

    gpio_init_ExpectAnyArgs() ; 
    gpio_set_dir_ExpectAnyArgs() ; 
    gpio_pull_up_ExpectAnyArgs();
   

    gpio_init_ExpectAnyArgs() ; 
    gpio_set_dir_ExpectAnyArgs() ; 
    gpio_pull_up_ExpectAnyArgs();
    set_sequence(0,0,__LINE__) ; 
    /// fn dependences hardware 
    gpio_set_irq_enabled_Ignore() ; 
    gpio_set_irq_enabled_Ignore() ; 
    gpio_set_irq_callback_Ignore() ;
    irq_set_enabled_Ignore() ;  



    initPorts(PORTA,PORTB) ; 
    getData(&encoder_test) ; 
    TEST_ASSERT_EQUAL(PORTA, _port_channel_a) ; 
    TEST_ASSERT_EQUAL(PORTB, _port_channel_b) ; 
    TEST_ASSERT_EQUAL(encoder.state,encoder_test.state) ; 

    //printf("encoder state: %d", encoder_test.state) ; 
}



