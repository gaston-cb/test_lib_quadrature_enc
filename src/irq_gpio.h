#include <stdint.h>
typedef unsigned int uint;

typedef enum{
    STATE_00 = 0  , //00
    STATE_01 = 1  , // 1   
    STATE_11 = 3  , // 3
    STATE_10 = 2  , // 2 

}state_quad_enc_t ; 

typedef enum{
    COUNTER_CLOCKWISE,
    COUNTER_ANTICLOCKWISE,
    COUNTER_STILL,  
}direction_t ;  


typedef struct{
    int16_t count_pulses; //max 600 - min 0 
    state_quad_enc_t state ;
    direction_t direction ; 
    float speed ; 
    float angle ; 
}encoder_quad_t ;
#define MAX_COUNT_PULSES_PER_REV  1755
#define CONVERSION_PULSES_FACTOR (0.3077)

#define DELAY_SIGNAL 800 /// time signal for a control of still state encoder_t 






uint get_porta(void); 
uint get_portb(void); 
void setPortsInit(uint port_a, uint port_b) ; 
void initialState( state_quad_enc_t initialstate) ; 
void setZero() ; 
void get_data_encoder(encoder_quad_t *encoder_rx) ; 
void gpio_callback_channel_ab(uint gpio,uint32_t event_mask ); 

