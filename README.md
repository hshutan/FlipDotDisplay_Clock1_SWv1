**This project is old! I do not recommend my hardware based project anymore, check out my RS-485 protocol-based projects! [Main Flipdot Project Page Here](https://github.com/hshutan/FlipDotCompendium)**
---
# FlipDotDisplay_Clock1_SWv1
This is an Arduino program, and more importantly, a library I created which will drive a 45x7 flip dot sign with my custom controller PCB.

**Most information on this project is on my hardware controller PCB's [github page](https://github.com/hshutan/45x7-flipdot-controller)**

This Arduino program displays a simple clock on a 45x7 flip dot panel. Time is read from a connected I<sup>2</sup>C RTC. The code will quickly write minute changes, and then once per hour, fully re-write the display from scratch.

Note that you will need to try various `onPulseLengthMicros`, `offPulseLengthMicros`, `onRecoverTime`, and `offRecoverTime settings` to produce reliable dot flips on your specific power supply and voltage.

If you need help understanding what my `Matrix_CoProcessor` library is doing, please see examples from the [FlipDot Bit Stream Workbook Excel file](FlipDot Bit Stream Workbook.xlsx) in this github project.
