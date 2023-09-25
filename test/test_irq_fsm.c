#include "unity.h" 

#include "mock_gpio.h"
#include "mock_timer.h"

#include "irq_gpio.c"
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

///test de dirección inicial cw
void test_initial_direction_cw(void){ 
    setZero() ; // pongo pulsos a cero 

}









