# FlipDotDisplay_Clock1_SWv1
This is an Arduino program, and more importantly a library I created that will drive a 45x7 flip dot sign with my custom controller PCB.

The Arduino program displays a simple clock on a 45x7 flip dot panel. Time is read from a connected IIC RTC. The code will quickly write minute changes, and then once per hour, fully re-write the display from scratch.

Most information on this project is on the PCB's [github page](https://github.com/hshutan/45x7-flipdot-controller)
