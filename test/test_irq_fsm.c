#include "unity.h" 

#include "mock_gpio.h"
#include "mock_timer.h"
#include "irq_gpio.h"


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
    //uint pa =  get_porta() ; 
    //uint pb =  get_portb() ; 
    
    TEST_ASSERT_EQUAL(0,count_state); 
    TEST_ASSERT_EQUAL(PORTA,get_porta()); 
    TEST_ASSERT_EQUAL(PORTB,get_portb()); 
    
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

/// test de estado inicial en cualquier estado 
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

/********************************************
 * ESTADO DE MOVIMIENTO EN SENTIDO HORARIO  *
 * TEST DE FSM EN ESTE SENTIDO              *
 ********************************************/

//  
// inicio estado 00 -> 10
//
void test_direction_cw_st0010(void){ 
    gpio_get_ExpectAndReturn((uint)PORTB,0) ; 
    time_us_64_IgnoreAndReturn(35) ;    

    setPortsInit(PORTA ,PORTB) ; // a = 10, b= 11 
    initialState(STATE_00) ; 
    setZero() ; // pongo pulsos a cero 
    gpio_callback_channel_ab((uint)PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(STATE_10,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(encoder_test.direction, COUNTER_CLOCKWISE); 
    TEST_ASSERT_EQUAL(count_state,1) ; 
}


void test_direction_cw_st1011(void){
    gpio_get_ExpectAndReturn((uint)PORTA,true) ; 
    time_us_64_IgnoreAndReturn(35) ;    
    setPortsInit(PORTA ,PORTB) ; // a = 10, b= 11 0
    initialState(STATE_10) ; 
    setZero() ; // pongo pulsos a cero 
   
    gpio_callback_channel_ab((uint)PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(encoder_test.state, STATE_11) ; 
    TEST_ASSERT_EQUAL(encoder_test.direction, COUNTER_CLOCKWISE); 
    TEST_ASSERT_EQUAL(count_state,1) ; 
}



void test_direction_cw_st1101(void){ 
    gpio_get_ExpectAndReturn(PORTB,1) ; 
    time_us_64_IgnoreAndReturn(35) ;    

    setPortsInit(PORTA ,PORTB) ; // a = 10, b= 11 
    initialState(STATE_11) ; 
    setZero() ; // pongo pulsos a cero 
    gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(encoder_test.state, STATE_01) ; 
    TEST_ASSERT_EQUAL(encoder_test.direction, COUNTER_CLOCKWISE); 
    TEST_ASSERT_EQUAL(count_state,1) ; 
}








