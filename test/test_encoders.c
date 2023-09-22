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




/// @brief  DATA FOR A TEST 
encoder_quad_t encoder_test ; 

void setUp(void) {}



void test_init_ports(void){ 

//    gpio_get_ExpectAndReturn(PORTA, 1) ; 

    gpio_init_ExpectAnyArgs() ; 
    gpio_set_dir_ExpectAnyArgs() ; 
    gpio_pull_up_ExpectAnyArgs();
   

    gpio_init_ExpectAnyArgs() ; 
    gpio_set_dir_ExpectAnyArgs() ; 
    gpio_pull_up_ExpectAnyArgs();
    gpio_get_ExpectAndReturn(PORTA, 1) ; 
    gpio_get_ExpectAndReturn(PORTB, 1) ; 

//
//_ExpectAndReturn
   
    
    //func_ExpectAnyArgsAndReturn(retval_to_return)
    initPorts(PORTA,PORTB) ; 
    getData(&encoder_test) ; 
    TEST_ASSERT_EQUAL(PORTA, _port_channel_a) ; 
    TEST_ASSERT_EQUAL(PORTB, _port_channel_b) ; 
    printf("encoder state: %d", encoder_test.state) ; 



}