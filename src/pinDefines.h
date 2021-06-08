#ifndef PINDEFINES_H
#define PINDEFINES_H


#define CONFIG_SWITCH_PIN 4  // pull low to enter midi config mode
#define CONFIG_LED_PIN 5
#define INPUT_BUTTON_PIN 6

#define LFO_RATE_POT_PIN 23



#define Filter1CCmsb 31
#define Filter1CClsb 63

#define Filter2CCmsb 32
#define Filter2CClsb 64

#define twelvebit 4095


enum CCresolution
{
    sevenBit = 0,
    fourteenBit
};

enum filters
{
    noFilter = 0,
    Filter1,
    Filter2,
    totalFilters
};


enum filterBytes
{
    filt1LSB = 0,
    filt1MSB,
    filt2LSB,
    filt2MSB,
    totalFiltValues
};



#endif