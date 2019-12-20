# SimonSays-Bluetooth-alarm-Clock
Project for CSE 321 "Embedded Systems" at SUNY University at Buffalo

This project involved using two arduinos to create two units-

One that functions as an alarm clock with two modes either the tradition press one button to shut the alarm off, or a Simon says mode which
communicates via bluetooth to the other module to start a game of Simon Says and waits for the Simon Says module to indicate that the game 
is over and to shut the alarm off

The other unit is Simon Says unit that generates a series of random patterns that light Leds and validates that a user correctly repeats 
that pattern via pressing corresponding buttons to those LEDs. If the user completes all the patterns it then will iterate to the next 
sequence otherwise it will repeat the pattern and wait for the user to correctly input it

In the situation that either module becomes disconnected from the other, the Simon Says unit will stop playing the patterns and the game, 
and the alarm clock itself will convert back to the convential button press alarm clock

see a vide demo here -https://www.youtube.com/watch?v=kcmmuJqj5ac&t=112s
