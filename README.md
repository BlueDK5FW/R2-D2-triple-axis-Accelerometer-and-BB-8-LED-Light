# R2-D2-triple-axis-Accelerometer-and-BB-8-LED-Light
R2-D2 is to match the car's movement , and move the neck to look at the curve ahead . BB-8 is blink eye as in communication with the R2-D2.

I used an arduino nano micro boad.

Description of connected equipment:

Digital i/o pin:

D pin 3:cheap(less than 20$) mp3 playback foundation(Automatically playing mp3 when the power is turned ON) for startup (R2-D2) sound

D pin 4:cheap mp3 playback foundation for when R2 are worried (about Sudden acceleration,sudden stop) sound

D pin 5:cheap mp3 playback foundation for when R2 are worried (about soaring , steep descent) Sound

D pin 7:connect to Turnigy TG9e micro servo signal input wire

D pin 9:R2-D2 RED LED (add 180Ω resistor to the anode)

D pin 12:BB-8 RED LED (add 180Ω resistor to the anode)

D pin 13:R2-D2 BLUE LED (add 120Ω resistor to the anode)

Analog i/o pin:

A pin 3:ADXL335 analog tripleaxis Accelerometer X axis

A pin 5:ADXL335 Y axis

A pin 7:ADXL335 Z axis

3v3 PIN:connect to ADXL335 vdd

5v PIN: connect to TG9e servo power input wire 
