#include "unity.h"
#include "quadrature_encoders.h"
#include "mock_gpio.h"
#define PORTA 10 
#define PORTB 11 



encoder_quad_t encoder_test ; 


void test_init_ports(){ 

    initPorts(PORTA,PORTB) ; 
    getData(&encoder_test) ; 
    TEST_ASSERT_EQUAL(PORTA, PORTB) ; 



}