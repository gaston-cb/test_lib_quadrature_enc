#include "quadrature_encoders.h"
#include <stdint.h>
#include "string.h"
#include "gpio.h"
#include "timer.h"




static encoder_quad_t encoder ;
static uint _port_channel_a ; 
static uint _port_channel_b ; 
uint64_t clock_speed ; 
uint8_t count_state = 0 ; 
uint8_t new_state_test ;  

void setPortsInit(uint port_a, uint port_b){ 
     _port_channel_a = port_a ; 
     _port_channel_a = port_b ; 
     
}

/// @brief  test initial function 
/// @param initialstate 

void initialState( state_quad_enc_t initialstate){ 
    encoder.state = initialstate ; 


}

void setZero(){
    encoder.angle = 0;
    encoder.count_pulses = 0;
    //3  ;
    encoder.direction = COUNTER_STILL; 
}  




void get_data_encoder(encoder_quad_t *encoder_rx){ 
    memcpy(encoder_rx,&encoder, sizeof(encoder_quad_t)) ; 
}




static void fsm_encoder(const state_quad_enc_t new_state){
    switch(encoder.state){
        case STATE_00:           
            if (new_state == STATE_10){
                encoder.state = STATE_10 ; 
                if (encoder.direction == COUNTER_STILL){
                    clock_speed = time_us_64() ; 
                    encoder.direction = COUNTER_CLOCKWISE ; 
                    count_state = 0;
                }else if (encoder.direction ==COUNTER_ANTICLOCKWISE){ 
                    encoder.direction = COUNTER_CLOCKWISE ; 
                    count_state = 0;
                }
                count_state++ ; 
                if (count_state%4 == 0){
                    clock_speed = time_us_64() - clock_speed ; 
                    encoder.speed = (1/(float) clock_speed) *CONVERSION_PULSES_FACTOR ;
                    count_state = 0; 
                    encoder.count_pulses++ ; 
                    encoder.count_pulses = (encoder.count_pulses + MAX_COUNT_PULSES_PER_REV)%MAX_COUNT_PULSES_PER_REV ; 
                    encoder.angle = encoder.count_pulses*CONVERSION_PULSES_FACTOR ; 
                    clock_speed =  time_us_64()  ; 
                }
            }else if (new_state == STATE_01){
                encoder.state = new_state;
                if (encoder.direction == COUNTER_STILL){
                    clock_speed =  time_us_64()  ; 
                    encoder.direction = COUNTER_ANTICLOCKWISE ; 
                    count_state = 0 ; 
                }else if (encoder.direction ==COUNTER_CLOCKWISE){ 
                    encoder.direction = COUNTER_ANTICLOCKWISE ; 
                }

                count_state++ ; 
                if (count_state%4 == 0){
                    clock_speed = time_us_64() - clock_speed ; 
                    encoder.speed = -1.0*(1/(float) clock_speed) *CONVERSION_PULSES_FACTOR ;
                    count_state = 0;                     
                    encoder.count_pulses-- ; 
                    encoder.count_pulses = (encoder.count_pulses + MAX_COUNT_PULSES_PER_REV)%MAX_COUNT_PULSES_PER_REV ; 
                    encoder.angle = encoder.count_pulses*CONVERSION_PULSES_FACTOR ; 
                    clock_speed =  time_us_64()  ;  // lo agregue yo
                }
            }
            break ; 
        case STATE_10:
            if (new_state == STATE_11){
                 encoder.state = new_state ; 
                if (encoder.direction == COUNTER_STILL){
                    clock_speed = time_us_64() ; 
                    encoder.direction = COUNTER_CLOCKWISE ; 
                    count_state = 0 ;
                }else if (encoder.direction ==COUNTER_ANTICLOCKWISE){ 
                    encoder.direction = COUNTER_CLOCKWISE ; 
                    count_state = 0 ;
                }
                count_state++ ; 
                if (count_state%4 == 0){
                    clock_speed = time_us_64() - clock_speed ; 
                    encoder.speed = (1.0/(float) clock_speed) *CONVERSION_PULSES_FACTOR ;
                    count_state = 0 ; 
                    encoder.count_pulses++ ; 
                    //encoder.count_pulses = (encoder.count_pulses + MAX_COUNT_PULSES_PER_REV)%MAX_COUNT_PULSES_PER_REV ; 
                    encoder.angle = encoder.count_pulses*CONVERSION_PULSES_FACTOR ; 
                    clock_speed = time_us_64() ; 
                }
            }else if (new_state == STATE_00){
                encoder.state = new_state ; 

                if (encoder.direction == COUNTER_STILL){
                    encoder.direction = COUNTER_ANTICLOCKWISE ; 
                    clock_speed = time_us_64() ; 
                    count_state = 0 ; 
                }else if (encoder.direction ==COUNTER_CLOCKWISE){ 
                    encoder.direction = COUNTER_ANTICLOCKWISE ; 
                    count_state = 0 ;  
                }
                count_state++ ; 
                if (count_state == 4){
                    clock_speed = time_us_64() - clock_speed ; 
                    encoder.speed = -(1.0/(float) clock_speed) *CONVERSION_PULSES_FACTOR ; 
                    count_state = 0  ; 
                    encoder.count_pulses-- ; 
                    encoder.count_pulses = (encoder.count_pulses + MAX_COUNT_PULSES_PER_REV)%MAX_COUNT_PULSES_PER_REV ; 
                    encoder.angle = encoder.count_pulses*CONVERSION_PULSES_FACTOR ; 
                    clock_speed = time_us_64() ; 
                }

            }

            break ; 
        case STATE_11:
            if (new_state == STATE_01){
                encoder.state = new_state ; 
                if (encoder.direction == COUNTER_STILL){
                    clock_speed = time_us_64() ; 
                    encoder.direction = COUNTER_CLOCKWISE ; 
                    count_state = 0 ;
                }else if (encoder.direction ==COUNTER_ANTICLOCKWISE){ 
                    encoder.direction = COUNTER_CLOCKWISE ; 
                    count_state = 0 ;
                }
                count_state++ ; 
                if (count_state == 4){
                    clock_speed = time_us_64() - clock_speed ; 
                    encoder.speed = (1.0/(float) clock_speed) *CONVERSION_PULSES_FACTOR ; 
                    count_state = 0 ; 
                    encoder.count_pulses++ ; 
                    //encoder.count_pulses = (encoder.count_pulses + MAX_COUNT_PULSES_PER_REV)%MAX_COUNT_PULSES_PER_REV ; 
                    encoder.angle = encoder.count_pulses*CONVERSION_PULSES_FACTOR ; 
                    clock_speed = time_us_64() ; 
                }
            }else if (new_state == STATE_10){
                 encoder.state = new_state ; 

                if (encoder.direction == COUNTER_STILL){
                    clock_speed = time_us_64() ; 
                    encoder.direction = COUNTER_ANTICLOCKWISE ; 
                    count_state = 0 ; 
                }else if (encoder.direction ==COUNTER_CLOCKWISE){ 
                    encoder.direction = COUNTER_ANTICLOCKWISE ; 
                    count_state = 0 ;  
                }

                count_state++ ; 
                if (count_state == 4){ 
                    clock_speed = time_us_64() - clock_speed ;
                    encoder.speed = -(1.0/(float) clock_speed) *CONVERSION_PULSES_FACTOR ;  
                    count_state = 0 ; 
                    encoder.count_pulses-- ; 
                    encoder.count_pulses = (encoder.count_pulses + MAX_COUNT_PULSES_PER_REV)%MAX_COUNT_PULSES_PER_REV ; 
                    encoder.angle = encoder.count_pulses*CONVERSION_PULSES_FACTOR ; 
                    clock_speed = time_us_64() ; 
                }
            }
            break ; 
        case STATE_01:
            if (new_state == STATE_00){
                 encoder.state = new_state ; 
                if (encoder.direction == COUNTER_STILL){
                    clock_speed = time_us_64() ; 
                    encoder.direction = COUNTER_CLOCKWISE ; 
                    count_state = 0 ;
                }else if (encoder.direction ==COUNTER_ANTICLOCKWISE){ 
                    encoder.direction = COUNTER_CLOCKWISE ; 
                    count_state = 0 ;
                }
                count_state++ ; 
                if (count_state%4 == 0){
                    clock_speed =time_us_64() - clock_speed ; 
                    encoder.speed = (1.0/(float) clock_speed) *CONVERSION_PULSES_FACTOR ; 
                    count_state =0 ; 
                    encoder.count_pulses++ ; 
                    encoder.count_pulses = (encoder.count_pulses + MAX_COUNT_PULSES_PER_REV)%MAX_COUNT_PULSES_PER_REV ; 
                    encoder.angle = encoder.count_pulses*CONVERSION_PULSES_FACTOR ; 
                    clock_speed = time_us_64() ; 

                }
            }else if (new_state == STATE_11){
                encoder.state = new_state ; 

                if (encoder.direction == COUNTER_STILL){
                    clock_speed = time_us_64() ; 
                    encoder.direction = COUNTER_ANTICLOCKWISE ; 
                    count_state = 0 ; 
                }else if (encoder.direction ==COUNTER_CLOCKWISE){ 
                    encoder.direction = COUNTER_ANTICLOCKWISE ; 
                    count_state = 0 ;  
                }

                count_state++ ; 
                if (count_state%4 == 0){
                    count_state = 0 ; 
                    clock_speed = time_us_64() - clock_speed ;  
                    encoder.speed = -(1.0/(float) clock_speed) *CONVERSION_PULSES_FACTOR ; 
                    encoder.count_pulses-- ; 
                    encoder.count_pulses = (encoder.count_pulses + MAX_COUNT_PULSES_PER_REV)%MAX_COUNT_PULSES_PER_REV ; 
                    encoder.angle = encoder.count_pulses *CONVERSION_PULSES_FACTOR ; 
                    clock_speed = time_us_64() ;
                }

            }
            break ; 
        default:  //safe code 
            break ; 
    }    
}





void gpio_callback_channel_ab(uint gpio,uint32_t event_mask ) { 
    volatile uint8_t new_state ; 
    new_state_test = 25 ; 
    if (gpio == 10){
        new_state = gpio_get(11) ; 
        new_state_test = 11 ; 
        if (event_mask == GPIO_IRQ_EDGE_FALL){ 
            new_state_test = 1 ; 
            new_state = 0<<1 | new_state; 
        }else if(event_mask == GPIO_IRQ_EDGE_RISE) {
            new_state = 1<<1 | new_state ; 
            new_state_test = 2           ; 
        }

    }else if (gpio ==  11){
        new_state =  gpio_get(10)<<1 ; 
        if (event_mask == GPIO_IRQ_EDGE_FALL){ 
            new_state = new_state | 0 ; 
            new_state_test = 3 ; 

        }else if(event_mask == GPIO_IRQ_EDGE_RISE) {        
            new_state = new_state | 1 ; 
            new_state_test = 4 ; 

        }
    }else{ 
        new_state_test = 33 ; 
        return ;
    }
   //new_state_test = new_state ; 
    ///FIXME: CHANGE FOR A QUEUE 
    fsm_encoder(new_state);  
}
