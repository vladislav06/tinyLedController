#include <avr/io.h>

#define F_CPU 8000000UL
#include <util/delay.h>


#define OUT_PIN PORTB0
#define IN_PIN PORTB2

//0.3 and 0.9
#define S0                             \
            PORTB |=1<<OUT_PIN;        \
            asm("nop");                \
            PORTB &=~(1<<OUT_PIN);



//0.6 and 0.6
#define S1                             \
            PORTB |=1<<OUT_PIN;        \
            asm("nop");                \
            asm("nop");                \
            asm("nop");                \
            asm("nop");                \
            PORTB &=~(1<<OUT_PIN);


struct Color{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
};

void writeColors(struct Color color){
    for(uint8_t i=0;i<80;i++){
for(uint8_t n=8;n>=1;n--){
    if(color.r&(1<<(n-1))){
                S1
            }else{
                S0
            }
        }
for(uint8_t n=8;n>=1;n--){
    if(color.g&(1<<(n-1))){
                S1
            }else{
                S0
            }
        }
for(uint8_t n=8;n>=1;n--){
    if(color.b&(1<<(n-1))){
                S1
            }else{
                S0
            }
        }
for(uint8_t n=8;n>=1;n--){
    if(color.w&(1<<(n-1))){
                S1
            }else{
                S0
            }
        }
    }
    //reset
    PORTB |=1<<OUT_PIN;
    _delay_us(80);
    PORTB &=~(1<<OUT_PIN);
}
uint8_t coef=0;
int8_t target=0;

int main(){
    //no division to get 8Mhz
    CCP=0xD8;
    CLKPSR =0;

    DDRB |= (1 << OUT_PIN); // Make OUT be an output.
    DDRB &= ~(1 << IN_PIN);	   // Make IN be an input.

    uint8_t prev = 0;

    while (1){
        uint8_t now = (PINB & (1 << IN_PIN))>>IN_PIN;
        //on rising edge
        if (now&&!prev){
           target = 1;
        }
        //on falling edge
        if (!now&&prev){
           target = -1;
        }

        prev = now;
        int16_t temp = coef;

        temp+=target;

        if(temp>255) temp = 255;
        if(temp<0) temp = 0;

        coef = temp;

        struct Color out;
        out.r = coef;
        out.g = coef;
        out.b = coef;
        out.w = coef;
        writeColors(out);
    }
}
