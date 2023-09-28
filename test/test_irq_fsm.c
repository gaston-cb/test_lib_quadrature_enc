#include "unity.h" 
#include "mock_gpio.h"
#include "mock_timer.h"
#include "irq_gpio.h"

/// define a test for a complete turn encoder  
#define PULSES_COUNT_CW 580
#define PULSES_COUNT_CCW 1755 
/// define a test for a uncomplete turn encoder  
#define PULSES_COUNT_CW2 963
#define PULSES_COUNT_CCW2 185
/// define a test for a uncomplete turn encoder  
#define PULSES_COUNT_CW3 129
#define PULSES_COUNT_CCW3 520 
#define PORTA 10
#define PORTB 11
//// support function for a testing sw 
void initial_state_00_cw(){
     gpio_get_ExpectAndReturn(PORTA,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        // 01 TO 11 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
}

//void initial_state_10_cw() ; 
//void initial_state_11_cw() ; 
//void initial_state_00_cw() ; 
/// ccw support function 
//void initial_state_00_ccw() ; 
//void initial_state_10_ccw() ; 
//void initial_state_11_ccw() ; 
//void initial_state_00_ccw() ; 






extern uint8_t count_state  ; 
encoder_quad_t encoder_test ; 



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

/************************************************
 * ESTADO DE MOVIMIENTO EN SENTIDO HORARIO (cw) *
 * TEST DE FSM EN ESTE SENTIDO                  *
 ************************************************/


/// test de cambios de ESTADO EN CW  
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
    //gpio_get_ExpectAndReturn(PORTB,1) ; 
    //time_us_64_IgnoreAndReturn(35) ;    

    setPortsInit(PORTA ,PORTB) ; // a = 10, b= 11 
    initialState(STATE_11) ; 
    setZero() ; // pongo pulsos a cero 
    gpio_get_ExpectAndReturn(PORTB,1) ;
    time_us_64_IgnoreAndReturn(35) ;   
    gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(encoder_test.state, STATE_01) ; 
    TEST_ASSERT_EQUAL(encoder_test.direction, COUNTER_CLOCKWISE); 
    TEST_ASSERT_EQUAL(count_state,1) ; 
}

void test_direction_cw_st0100(void){ 
    gpio_get_ExpectAndReturn(PORTA,0) ; 
    time_us_64_IgnoreAndReturn(35) ;    

    setPortsInit(PORTA ,PORTB) ; // a = 10, b= 11 
    initialState(STATE_01) ; 
    setZero() ; // pongo pulsos a cero 
    gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(encoder_test.state, STATE_00) ; 
    TEST_ASSERT_EQUAL(encoder_test.direction, COUNTER_CLOCKWISE); 
    TEST_ASSERT_EQUAL(count_state,1) ; 
}

///void test_direction_cw_invalid_transition(void){ }

//// TEST DE CONTADOR DE PULSOS EN VUELTA COMPLETA , IDA en sentido cw  
//// USE A MAX PULSES 1755
void test_count_pulses_cw_sti00(void){ 
    int16_t pulses = (int16_t)PULSES_COUNT_CW ; 
    int16_t count_pulses ; 
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_00)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    for (count_pulses = 0; count_pulses<pulses;count_pulses++){ 
        gpio_get_ExpectAndReturn(PORTB,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(pulses%1755,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_CLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(count_state,0) ; 
    TEST_ASSERT_EQUAL(STATE_00,encoder_test.state) ; 

}

void test_count_pulses_cw_sti10(void){ 
    int16_t pulses = (int16_t)PULSES_COUNT_CW ; 
    int16_t count_pulses ; 
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_10)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    for (count_pulses = 0; count_pulses<pulses;count_pulses++){ 
        //10 to 11         
        gpio_get_ExpectAndReturn(PORTA,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
 
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 to 01 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    

        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        // 01 TO 00 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 TO 10 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    }

    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(pulses%1755,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_CLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(0,count_state) ; 
    TEST_ASSERT_EQUAL(STATE_10,encoder_test.state) ; 

}

void test_count_pulses_cw_sti11(void){ 
    int16_t pulses = (int16_t)PULSES_COUNT_CW ; 
    int16_t count_pulses ; 
    initialState(STATE_11)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    for (count_pulses = 0; count_pulses<pulses;count_pulses++){ 
        //11 to 01         
        gpio_get_ExpectAndReturn(PORTB,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    }

    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(pulses%1755,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_CLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(0,count_state) ; 
    TEST_ASSERT_EQUAL(STATE_11,encoder_test.state) ; 
}

void test_count_pulses_cw_sti01(void){ 
    int16_t pulses = (int16_t)PULSES_COUNT_CW ; 
    int16_t count_pulses ; 
    initialState(STATE_01)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    for (count_pulses = 0; count_pulses<pulses;count_pulses++){ 
        //01 to 00         
        gpio_get_ExpectAndReturn(PORTA,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 A 10 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        // 10 A 11 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 A 01 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    }

    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(pulses%1755,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_CLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(0,count_state) ; 
    TEST_ASSERT_EQUAL(STATE_01,encoder_test.state) ; 
}


/*****************************************************
 * ESTADO DE MOVIMIENTO EN SENTIDO ANTIHORARIO (ccw) *
 * TEST DE FSM EN ESTE SENTIDO                       *
 *****************************************************/

/// test de cambios de ESTADO EN CCW  
void test_direction_ccw_sttransition_initial(void){ 

    setPortsInit(PORTA ,PORTB) ; // a = 10, b= 11 
    initialState(STATE_00) ; 
    setZero() ; // pongo pulsos a cero, como es en sentido contrario los pulsos iran ddesde 1755 a 0 
    /// TEST TRANSITION 00 TO 01 
    gpio_get_ExpectAndReturn((uint)PORTA,0) ; 
    time_us_64_IgnoreAndReturn(35) ;    
    gpio_callback_channel_ab((uint)PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(STATE_01,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(encoder_test.direction, COUNTER_ANTICLOCKWISE); 
    TEST_ASSERT_EQUAL(count_state,1) ; 

    /// TEST TRANSITION 01 TO 11 
    initialState(STATE_01) ; 
    setZero() ; 
    gpio_get_ExpectAndReturn((uint)PORTB,1) ; 
    time_us_64_IgnoreAndReturn(35) ;    
    gpio_callback_channel_ab((uint)PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(STATE_11,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(encoder_test.direction, COUNTER_ANTICLOCKWISE); 
    TEST_ASSERT_EQUAL(count_state,1) ; 


    /// TEST TRANSITION 11 TO 10 
    initialState(STATE_11) ; 
    setZero() ; 
    gpio_get_ExpectAndReturn((uint)PORTA,1) ; 
    time_us_64_IgnoreAndReturn(35) ;    
    gpio_callback_channel_ab((uint)PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(STATE_10,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(encoder_test.direction, COUNTER_ANTICLOCKWISE); 
    TEST_ASSERT_EQUAL(count_state,1) ; 

    /// TEST TRANSITION 10 TO 00 

    initialState(STATE_10) ; 
    setZero() ; 
    gpio_get_ExpectAndReturn((uint)PORTB,0) ; 
    time_us_64_IgnoreAndReturn(35) ;    
    gpio_callback_channel_ab((uint)PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
    get_data_encoder(&encoder_test) ;  
    TEST_ASSERT_EQUAL(STATE_00,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(encoder_test.direction, COUNTER_ANTICLOCKWISE); 
    TEST_ASSERT_EQUAL(count_state,1) ; 

}
///void test_direction_ccw_invalid_transition(void){ }

//// test de vuelta completa en sentido ccw 
//// distintos test con distintos valores iniciales de st 
void test_count_pulses_ccw_sti00(void){ 
    int16_t pulses = (int16_t)PULSES_COUNT_CCW ; 
    int16_t count_pulses ; 
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_00)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    for (count_pulses = 0; count_pulses<pulses;count_pulses++){ 
       // 00 TO 01 
       gpio_get_ExpectAndReturn(PORTA,0) ;
       time_us_64_IgnoreAndReturn(35) ;    
       gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
       // 01 TO 11 
       gpio_get_ExpectAndReturn(PORTB,1) ; 
       time_us_64_IgnoreAndReturn(35) ;    
       gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
       //11 TO 10 
       gpio_get_ExpectAndReturn(PORTA,1) ; 
       time_us_64_IgnoreAndReturn(35) ;    
       gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
       //10 TO 00 
       gpio_get_ExpectAndReturn(PORTB,0) ; 
       time_us_64_IgnoreAndReturn(35) ;    
       gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
//        initial_state_00_cw() ; 
        resetTest() ;  ///clear memory test 
    }
    count_pulses = count_pulses%MAX_COUNT_PULSES_PER_REV; 
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(
       (MAX_COUNT_PULSES_PER_REV-count_pulses)%MAX_COUNT_PULSES_PER_REV,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(count_state,0) ; 
    TEST_ASSERT_EQUAL(STATE_00,encoder_test.state) ; 


}

void test_count_pulses_ccw_sti01(void){ 
    int16_t pulses = (int16_t)PULSES_COUNT_CCW ; 
    int16_t count_pulses ; 
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_01)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    for (count_pulses = 0; count_pulses<pulses;count_pulses++){ 
        // 01 TO 11 
        gpio_get_ExpectAndReturn(PORTB,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        // 11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 TO 01 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    }
    count_pulses = count_pulses%MAX_COUNT_PULSES_PER_REV; 
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(
       (MAX_COUNT_PULSES_PER_REV-count_pulses)%MAX_COUNT_PULSES_PER_REV,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(count_state,0) ; 
    TEST_ASSERT_EQUAL(STATE_01,encoder_test.state) ; 
}

void test_count_pulses_ccw_sti11(void){
    int16_t pulses = (int16_t)PULSES_COUNT_CCW ; 
    int16_t count_pulses ; 
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_11)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    for (count_pulses = 0; count_pulses<pulses;count_pulses++){ 
        // 11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        // 10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 TO 01 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //01 TO 11 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    }
    count_pulses = count_pulses%MAX_COUNT_PULSES_PER_REV; 
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(
       (MAX_COUNT_PULSES_PER_REV-count_pulses)%MAX_COUNT_PULSES_PER_REV,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(count_state,0) ; 
    TEST_ASSERT_EQUAL(STATE_11,encoder_test.state) ; 
}

void test_count_pulses_ccw_sti10(void){
    int16_t pulses = (int16_t)PULSES_COUNT_CCW ; 
    int16_t count_pulses ; 
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_10)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    for (count_pulses = 0; count_pulses<pulses;count_pulses++){ 
        // 10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        // 00 TO 01 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //01 TO 11 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    }
    count_pulses = count_pulses%MAX_COUNT_PULSES_PER_REV; 
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(
       (MAX_COUNT_PULSES_PER_REV-count_pulses)%MAX_COUNT_PULSES_PER_REV,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(count_state,0) ; 
    TEST_ASSERT_EQUAL(STATE_10,encoder_test.state) ; 
}


/***************************************
*       TEST DE CONTEO DE PULSOS       * 
*       IDA Y VUELTA  sentido CW->CCW  *     
***************************************/

void test_count_pulses_cw2ccw_st00(void){
    int16_t pulses_cw = (int16_t)PULSES_COUNT_CW2 ; 
    int16_t pulses_ccw = (int16_t)PULSES_COUNT_CCW2 ;
    int16_t pulses_count = 0 ;  
    int16_t count_pulses = pulses_cw - pulses_ccw ; 
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_00)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    /// gira pulses_cw iniciando y terminando en el mismo punto inicial
    for (pulses_count = 0; pulses_count<pulses_cw;pulses_count++){ 
        // 00 TO 10 
        //   setPortsInit(PORTA,PORTB) ;         
        gpio_get_ExpectAndReturn(PORTB,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //10 TO 11 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    

        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        // 11 TO 01 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    

        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //01 TO 00 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    

        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    }

    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){ 
        // 00 TO 01 
        gpio_get_ExpectAndReturn(PORTA,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        // 01 TO 11 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(count_pulses,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(STATE_00,encoder_test.state) ; 
} ; 

void test_count_pulses_cw2ccw_st10(void)
{
    int16_t pulses_cw = (int16_t)PULSES_COUNT_CW2 ; 
    int16_t pulses_ccw = (int16_t)PULSES_COUNT_CCW2 ;
    int16_t pulses_count = 0 ;  
    int16_t count_pulses = pulses_cw - pulses_ccw ; // pulses cw > pulses _ccw
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_10)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    /// gira pulses_cw iniciando y terminando en el mismo punto inicial
    for (pulses_count = 0; pulses_count<pulses_cw;pulses_count++){ 
             //10 to 11         
        gpio_get_ExpectAndReturn(PORTA,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
 
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 to 01 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    

        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        // 01 TO 00 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 TO 10 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(STATE_10,encoder_test.state) ; 
    // sentido ccw 
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){ 
              // 10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        // 00 TO 01 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //01 TO 11 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 

    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(count_pulses,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(STATE_10,encoder_test.state) ; 

} ; 

void test_count_pulses_cw2ccw_st11(void){
    int16_t pulses_cw = (int16_t)PULSES_COUNT_CW2 ; 
    int16_t pulses_ccw = (int16_t)PULSES_COUNT_CCW2 ;
    int16_t pulses_count = 0 ;  
    int16_t count_pulses = pulses_cw - pulses_ccw ; // pulses cw > pulses _ccw
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_11)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    for (pulses_count = 0 ; pulses_count<pulses_cw; pulses_count++){ 
        gpio_get_ExpectAndReturn(PORTB,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(STATE_11,encoder_test.state) ; 
    for (pulses_count = 0 ; pulses_count<pulses_ccw; pulses_count++){
       // 11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        // 10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 TO 01 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //01 TO 11 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    
    
    } 
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(count_pulses,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(STATE_11,encoder_test.state) ; 

} ; 

void test_count_pulses_cw2ccw_st01(void){
    int16_t pulses_cw = (int16_t)PULSES_COUNT_CW2 ; 
    int16_t pulses_ccw = (int16_t)PULSES_COUNT_CCW2 ;
    int16_t pulses_count = 0 ;  
    int16_t count_pulses = pulses_cw - pulses_ccw ; // pulses cw > pulses _ccw
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_01)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    for (pulses_count = 0 ; pulses_count<pulses_cw; pulses_count++){ 
        gpio_get_ExpectAndReturn(PORTA,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 A 10 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        // 10 A 11 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 A 01 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(STATE_01,encoder_test.state) ; 
    for (pulses_count = 0 ; pulses_count<pulses_ccw; pulses_count++){ 
        gpio_get_ExpectAndReturn(PORTB,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        // 11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 TO 01 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(count_pulses,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(STATE_01,encoder_test.state) ; 


} ; 

/***************************************
*       TEST DE CONTEO DE PULSOS       * 
*       IDA Y VUELTA  sentido CCW->CW  *     
***************************************/
void test_count_pulses_ccw2cw_st00(void){
    int16_t pulses_cw = (int16_t)PULSES_COUNT_CW3 ; 
    int16_t pulses_ccw = (int16_t)PULSES_COUNT_CCW3 ;
    int16_t pulses_count = 0 ;  
    int16_t count_pulses = (MAX_COUNT_PULSES_PER_REV - pulses_ccw+pulses_cw)% MAX_COUNT_PULSES_PER_REV; 
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_00)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    // SENTIDO CCW 
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){ 
        // 00 TO 01 
        gpio_get_ExpectAndReturn(PORTA,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        // 01 TO 11 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    }
    // SENTIDO CW 
    for (pulses_count = 0; pulses_count<pulses_cw;pulses_count++){ 
        // 00 TO 10 
        //   setPortsInit(PORTA,PORTB) ; 
        
        gpio_get_ExpectAndReturn(PORTB,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
 
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //10 TO 11 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    

        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        // 11 TO 01 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    

        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //01 TO 00 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    

        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(count_pulses,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_CLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(STATE_00,encoder_test.state) ; 
} ; 

void test_count_pulses_ccw2cw_st01(void){
    int16_t pulses_cw = (int16_t)PULSES_COUNT_CW3 ; 
    int16_t pulses_ccw = (int16_t)PULSES_COUNT_CCW3 ;
    int16_t pulses_count = 0 ;  
    int16_t count_pulses = (MAX_COUNT_PULSES_PER_REV - pulses_ccw+pulses_cw)% MAX_COUNT_PULSES_PER_REV; 
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_01)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    // SENTIDO CCW 
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){ 
        gpio_get_ExpectAndReturn(PORTB,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        // 11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 TO 01 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test    
    }
    // SENTIDO CW 
    for (pulses_count = 0; pulses_count<pulses_cw;pulses_count++){ 
            //01 to 00         
        gpio_get_ExpectAndReturn(PORTA,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 A 10 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        // 10 A 11 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 A 01 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(count_pulses,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_CLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(STATE_01,encoder_test.state) ; 
} ; 

void test_count_pulses_ccw2cw_st11(void){
    int16_t pulses_cw = (int16_t)PULSES_COUNT_CW3 ; 
    int16_t pulses_ccw = (int16_t)PULSES_COUNT_CCW3 ;
    int16_t pulses_count = 0 ;  
    int16_t count_pulses = (MAX_COUNT_PULSES_PER_REV - pulses_ccw+pulses_cw)% MAX_COUNT_PULSES_PER_REV; 
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_11)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    // SENTIDO CCW 
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){ 
            // 11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        // 10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 TO 01 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //01 TO 11 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    }
    // SENTIDO CW 
    for (pulses_count = 0; pulses_count<pulses_cw;pulses_count++){ 
        gpio_get_ExpectAndReturn(PORTB,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 

    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(count_pulses,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_CLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(STATE_11,encoder_test.state) ; 
}

void test_count_pulses_ccw2cw_st10(void){ 
    int16_t pulses_cw = (int16_t)PULSES_COUNT_CW3 ; 
    int16_t pulses_ccw = (int16_t)PULSES_COUNT_CCW3 ;
    int16_t pulses_count = 0 ;  
    int16_t count_pulses = (MAX_COUNT_PULSES_PER_REV - pulses_ccw+pulses_cw)% MAX_COUNT_PULSES_PER_REV; 
    time_us_64_IgnoreAndReturn(35) ;        
    initialState(STATE_10)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    // SENTIDO CCW 
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){ 
       // 10 TO 00 
        gpio_get_ExpectAndReturn(PORTB,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        // 00 TO 01 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //01 TO 11 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 TO 10 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 

    }
    for (pulses_count = 0; pulses_count<pulses_cw;pulses_count++){ 
        //10 to 11         
        gpio_get_ExpectAndReturn(PORTA,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
 
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        //11 to 01 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    

        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        // 01 TO 00 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        //00 TO 10 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 

    }

    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(count_pulses,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_CLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(STATE_10,encoder_test.state) ; 
}

/****************************************** 
*       TEST CONTEO DE PULSOS             * 
*       GIRO n pulsos cw y luego n pulsos *
*       en ccw. Debe volver a cero        *    
*******************************************/
#define NTURNS_ROUND_TRIP 25

void test_round_trip_cw2ccw_st00(void){ 
    int16_t pulses_cw = (int16_t)NTURNS_ROUND_TRIP ; 
    int16_t pulses_ccw = (int16_t)NTURNS_ROUND_TRIP ;
    int16_t pulses_count = 0 ;  
    initialState(STATE_00)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    /// sentido CW
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){
        gpio_get_ExpectAndReturn(PORTB,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    } 
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(STATE_00,encoder_test.state) ; 
    /// SENTIDO CCW 
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){
        gpio_get_ExpectAndReturn(PORTA,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    } 
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(STATE_00,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(0,encoder_test.count_pulses) ; 
}

void test_round_trip_cw2ccw_st01(void){
    int16_t pulses_cw = (int16_t)NTURNS_ROUND_TRIP ; 
    int16_t pulses_ccw = (int16_t)NTURNS_ROUND_TRIP ;
    int16_t pulses_count = 0 ;  
    initialState(STATE_01)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    /// SENTIDO CW 
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){     
        gpio_get_ExpectAndReturn(PORTA,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test  
    }
    /// SENTIDO CCW 
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){
        gpio_get_ExpectAndReturn(PORTB,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(STATE_01,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(0,encoder_test.count_pulses) ; 


 }

void test_round_trip_cw2ccw_st11(void){
    int16_t pulses_cw = (int16_t)NTURNS_ROUND_TRIP ; 
    int16_t pulses_ccw = (int16_t)NTURNS_ROUND_TRIP ;
    int16_t pulses_count = 0 ;  
    initialState(STATE_11)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    /// CW 
    for (pulses_count = 0 ; pulses_count<pulses_cw;pulses_count++){
        gpio_get_ExpectAndReturn(PORTB,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 

    }
    ///CCW 
    for (pulses_count = 0 ; pulses_count<pulses_ccw;pulses_count++){
        gpio_get_ExpectAndReturn(PORTA,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    }
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(STATE_11,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(0,encoder_test.count_pulses) ; 

}

void test_round_trip_cw2ccw_st10(void){
    int16_t pulses_cw = (int16_t)NTURNS_ROUND_TRIP ; 
    int16_t pulses_ccw = (int16_t)NTURNS_ROUND_TRIP ;
    int16_t pulses_count = 0 ;  
    initialState(STATE_11)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    /// cw 
    for (pulses_count = 0 ; pulses_count<pulses_cw;pulses_count++){
        gpio_get_ExpectAndReturn(PORTB,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 

    }
    /// ccw 
    for (pulses_count = 0 ; pulses_count<pulses_cw;pulses_count++){
        gpio_get_ExpectAndReturn(PORTA,1) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        resetTest() ;  ///clear memory test 
    }

    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(STATE_11,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(0,encoder_test.count_pulses) ; 

 }



/****************************************** 
*       TEST CONTEO DE PULSOS             * 
*      GIRO n pulsos ccw y luego n pulsos *
*       en cw. Debe volver a cero         *    
*******************************************/

void test_round_trip_ccw2cw_st00(void) {
    int16_t pulses_cw = (int16_t)NTURNS_ROUND_TRIP ; 
    int16_t pulses_ccw = (int16_t)NTURNS_ROUND_TRIP ;
    int16_t pulses_count = 0 ;  
    initialState(STATE_00)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
      /// SENTIDO CCW 
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){
        gpio_get_ExpectAndReturn(PORTA,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTB,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    } 
    /// sentido CW
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){
        gpio_get_ExpectAndReturn(PORTB,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
    } 
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(STATE_00,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(COUNTER_CLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(0,encoder_test.count_pulses) ; 


}

//void test_round_trip_ccw2cw_st01(void) {}
//void test_round_trip_ccw2cw_st11(void) {}
//void test_round_trip_ccw2cw_st10(void) {}


/****************************************** 
*  TEST N pulse and add new state         *
*          VUELTA A CERO  CW --> CCW 
*******************************************/
#define NPULSES_TEST 11
//////// TEST INITIAL WITH A STATE_00 
void test_n_pulses_round_trip_add1state_st00(void){ 
    int16_t pulses_cw = (int16_t)NPULSES_TEST ; 
    int16_t pulses_ccw = (int16_t)NPULSES_TEST ;
    int16_t pulses_count = 0 ;  
    initialState(STATE_00)  ;   /// generate pulses 
    setPortsInit(PORTA,PORTB) ; 
    setZero() ; 
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){
        gpio_get_ExpectAndReturn(PORTB,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 
      } 
    //generar siguiente estado 
    gpio_get_ExpectAndReturn(PORTB,0) ;
    time_us_64_IgnoreAndReturn(35) ;    
    gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE);   
    get_data_encoder(&encoder_test)   ; 

    //assert siguiente estado 
    TEST_ASSERT_EQUAL(STATE_10,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(NPULSES_TEST,encoder_test.count_pulses) ; 
    TEST_ASSERT_EQUAL(COUNTER_CLOCKWISE,encoder_test.direction) ;

    /// ccw usint state initial 10 
    ///ccw 
    for (pulses_count = 0; pulses_count<pulses_ccw;pulses_count++){
        gpio_get_ExpectAndReturn(PORTB,0) ;
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        gpio_get_ExpectAndReturn(PORTA,0) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTB,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
        gpio_get_ExpectAndReturn(PORTA,1) ; 
        time_us_64_IgnoreAndReturn(35) ;    
        gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
        resetTest() ;  ///clear memory test 

    } 
    //comprobar estado, pulsos, y sentido de giro 
    get_data_encoder(&encoder_test)   ; 
    TEST_ASSERT_EQUAL(STATE_10,encoder_test.state) ; 
    TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
    TEST_ASSERT_EQUAL(0,encoder_test.count_pulses) ; 


}

// void test_n_pulses_round_trip_add2state_st00(void){ 

// }
// void test_n_pulses_round_trip_add3state_st00(void){ 

// }





/****************************************** 
*                                         *  
*******************************************/
