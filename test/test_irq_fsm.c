#include "unity.h" 
#include "mock_gpio.h"
#include "mock_timer.h"
#include "irq_gpio.h"

#define PULSES_COUNT_CW 1756
#define PULSES_COUNT_CCW 1800 

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


/************************************************
 * ESTADO DE MOVIMIENTO EN SENTIDO HORARIO (ccw) *
 * TEST DE FSM EN ESTE SENTIDO                  *
 ************************************************/

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

/// test de vuelta completa en sentido ccw 
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


// void test_count_pulses_ccw_sti10(void){
//     int16_t pulses = (int16_t)PULSES_COUNT_CCW ; 
//     int16_t count_pulses ; 
//     time_us_64_IgnoreAndReturn(35) ;        
//     initialState(STATE_10)  ;   /// generate pulses 
//     setPortsInit(PORTA,PORTB) ; 
//     setZero() ; 
//     for (count_pulses = 0; count_pulses<pulses;count_pulses++){ 
//         // 10 TO 00 
//         gpio_get_ExpectAndReturn(PORTA,1) ;
//         time_us_64_IgnoreAndReturn(35) ;    
//         gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_FALL); 
//         // 00 TO 01 
//         gpio_get_ExpectAndReturn(PORTB,0) ; 
//         time_us_64_IgnoreAndReturn(35) ;    
//         gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_FALL); 
//         //01 TO 11 
//         gpio_get_ExpectAndReturn(PORTA,0) ; 
//         time_us_64_IgnoreAndReturn(35) ;    
//         gpio_callback_channel_ab(PORTB,(uint32_t)GPIO_IRQ_EDGE_RISE); 
//         //11 TO 10 
//         gpio_get_ExpectAndReturn(PORTB,1) ; 
//         time_us_64_IgnoreAndReturn(35) ;    
//         gpio_callback_channel_ab(PORTA,(uint32_t)GPIO_IRQ_EDGE_RISE); 
//         resetTest() ;  ///clear memory test 
//     }
//     count_pulses = count_pulses%MAX_COUNT_PULSES_PER_REV; 
//     get_data_encoder(&encoder_test)   ; 
//     TEST_ASSERT_EQUAL(
//        (MAX_COUNT_PULSES_PER_REV-count_pulses)%MAX_COUNT_PULSES_PER_REV,encoder_test.count_pulses) ; 
//     TEST_ASSERT_EQUAL(COUNTER_ANTICLOCKWISE,encoder_test.direction) ;
//     TEST_ASSERT_EQUAL(count_state,0) ; 
//     TEST_ASSERT_EQUAL(STATE_10,encoder_test.state) ; 
// }


