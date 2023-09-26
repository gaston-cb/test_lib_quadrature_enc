#include "unity.h" 

#include "irq_gpio.c"
#include "mock_gpio.h"
#include "mock_timer.h"

typedef unsigned int uint;

#define PORTA 10
#define PORTB 11
extern uint8_t count_state  ; 
encoder_quad_t encoder_test ; 
enum gpio_irq_level events_irq[4] = {
   GPIO_IRQ_LEVEL_LOW  ,
   GPIO_IRQ_LEVEL_HIGH ,
   GPIO_IRQ_EDGE_FALL ,
   GPIO_IRQ_EDGE_RISE 
} ; 


void test_init_ports(void) {
    setPortsInit(PORTA,PORTB) ; 
    TEST_ASSERT_EQUAL(0,count_state); 
}




/// test posicion cero  
void test_zero_position(void){ 
    setZero() ; ///seteo de cero pulsos y grados 
    get_data_encoder(&encoder_test) ; 
    TEST_ASSERT_EQUAL(encoder_test.count_pulses,0); 
    TEST_ASSERT_EQUAL(encoder_test.direction,COUNTER_STILL); 
    TEST_ASSERT_EQUAL(0,count_state); 

}


/**************************************************
 *  TESTING INTENSIVO DE MAQUINA DE ESTADOS       *
 *          TEST DE SOFTWARE                      * 
 *                                                *                  
***************************************************/

///test de estado inicial
void test_initial_state(void){ 
    initialState(STATE_00) ; 
    setZero() ; // pongo pulsos a cero 
    // state 00 initial test 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(encoder_test.state, STATE_00) ;
    // state 01 initial test 
    initialState(STATE_01) ; 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(encoder_test.state, STATE_01) ;
    // state 10 initial test 
    initialState(STATE_10) ; 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(encoder_test.state, STATE_10) ;

    // state 11 initial test 
    initialState(STATE_11) ; 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(encoder_test.state, STATE_11) ;


}
extern uint8_t new_state_test ; 
/// inicio estado 00 -> 10 --> still inicio
void test_direction_cw(void){ 

    //setPortsInit(PORTA,PORTB) ; 
    setPortsInit(PORTA ,11) ; // a = 10, b= 11 
    initialState(STATE_00) ; 
    
    setZero() ; // pongo pulsos a cero 
//    TEST_ASSERT_EQUAL(encoder_test.state, STATE_00) ; 
    gpio_get_ExpectAndReturn(11,0) ; 
    time_us_64_IgnoreAndReturn(35) ; 
    //gpio_get_ExpectAndReturn(11u,1u) ; 
    
    gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
    get_data_encoder(&encoder_test) ;  

    TEST_ASSERT_EQUAL(new_state_test, 2) ; 
    TEST_ASSERT_EQUAL(encoder_test.state, STATE_10) ; 
//    TEST_ASSERT_EQUAL(encoder_test.direction, COUNTER_STILL); 
//    TEST_ASSERT_EQUAL(count_state,0) ; 
    


}










