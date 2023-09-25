#include "quadrature_encoders.h"
#include <stdint.h>
#include "string.h"
#include "gpio.h"
#include "timer.h"




static encoder_quad_t encoder ;
uint16_t _port_channel_a ; 
uint16_t _port_channel_b ; 
uint64_t clock_speed ; 
uint8_t count_state = 0 ; 


void setPortsInit(uint16_t port_a, uint16_t port_b){ 
     _port_channel_a = port_a ; 
     _port_channel_a = port_b ; 
     
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
                encoder.state = new_state ; 
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
                    //encoder.count_pulses = (encoder.count_pulses + MAX_COUNT_PULSES_PER_REV)%MAX_COUNT_PULSES_PER_REV ; 
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
  
    if (gpio == _port_channel_a){
        new_state = gpio_get(_port_channel_b) ; 
        if (event_mask == GPIO_IRQ_EDGE_FALL){ 
            new_state = 0<<1 | new_state; 
        }else if(event_mask == GPIO_IRQ_EDGE_RISE) {
            new_state = 1<<1 | new_state; 
        }

    }else if (gpio == _port_channel_b){
        new_state =  gpio_get(_port_channel_a)<<1 ; 
        if (event_mask == GPIO_IRQ_EDGE_FALL){ 
            new_state = new_state | 0 ; 
        }else if(event_mask == GPIO_IRQ_EDGE_RISE) {        
            new_state = new_state | 1 ; 
        }


    }else return ; 
    ///FIXME: CHANGE FOR A QUEUE 
    fsm_encoder(new_state);  
}
