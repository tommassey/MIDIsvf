#include <Arduino.h>
#include "usb_names.h"

#define MIDI_NAME   {'N','o','n',' ','M','u','s','i','c',' ','L','a','b','s',' ','D','u','a','l',' ','S','V','F'}
#define MIDI_NAME_LEN  23

struct usb_string_descriptor_struct usb_string_product_name = {
        2 + MIDI_NAME_LEN * 2,
        3,
        MIDI_NAME
};
