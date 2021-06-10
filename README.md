# MIDIsvf
14bit MIDI to CV


the device is a 2 channel USB MIDI CC to CV converter with LFO.  

a MIDI CC (continuous controller) message is a 7 or 14bit value that can control various things on a MIDI device (pitch bend, filter cutoff, volume, pan, etc.)
it can be sent with note on/off data or assigned to a pot/slider or used from a sequencer/DAW as an automation clip

basically this thing allows you to have two DAC outputs each controlled by different MIDI CCs.
there's one LFO and each channel has it's own LFO amount knob.  
the LFO and the CC data are added together and written to the DAC.

currently the external 'hardware' is:

pot for LFOrate
pot for LFO amountchannel1
pot for LFO amount channel2  <--  not currently implemented in code

an LED to show LFO output
an LED on the teensy board itself

single pushbutton for config

2 channel SPI DAC for the 2 outputs




Config mode

if you hold down the button during power up the device enters config mode where you can set the MIDI channel, CC and min & max values for each channel

- with the usb unplugged, hold down the button
- plug in the usb cable, and release the button, the LED will flash once to show we're configuring channel 1
- move the MIDI Controller you want assigned to channel 1 across it's whole range, all the way from top to bottom
- press the button again, the led will flash twice to show we're now configuring channel 2
- repeat the previous step but for the MIDI controller you want assigned to channel 2

  + if you're happy with the changes, hold the button for a couple of seconds and the LED will flash
    to confim the data has been saved, the device will now restart
    
  + if want to make some changes, press the button again, the LED will flash once and you will return
    to the channel 1 config and the process starts over
  

