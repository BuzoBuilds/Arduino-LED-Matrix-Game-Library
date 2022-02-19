32x64 LED Matrix Video Game Library for Arduino Mega by BuzoBuilds (Amby Agubuzo) 
video of library in action: https://youtu.be/YwMnKW-urns
BuzoBuilds youtube channel link: https://www.youtube.com/channel/UCPmbcK8IcX0D4FibWSiUAdw

The purposes of this library is to facilitate the development of Arduino Mega video game applications
or sketches that make use of a 32x64 LED matrix  

Although the library was specifically programmed for 32X64 LED matrices it can definitely be adjusted to 
work for any size matrix that uses the same shift register and addressing mechanism that 32x64 matrices use

Requirements: 
    LED_Matrix_Lib.h  
    
To use, add this folder into the Arduino libraries folder and then simply write an include line in your sketch 
to include the LED_Matrix_Lib.h file 

Current known bugs and other tidbits:  
    anything displayed on row 15 will be displayed on row 0 for some reason, likely to do with drawLEDMatrix function 

    similar to the first bug, anything displayed on row 31 will be displayed on row 16