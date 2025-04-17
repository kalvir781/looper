# Looper

Modified Pod Example Looper to the way I'd like to use it.

## Author

Original Author: Ben Sergentanis
Kalvir Sandhu

## modifications from original
- No drywet, want all one levels
- modified the color for record to be green and play to be blue

## Design and thinking for future changes
Icebox for concepts to investigate and implement in the future as I make music with it.
- Rotate the knob to record a different track? Would have to stitch together the track buffers to play out.
- Adding reverb to the mix of the recorded track? Effects similiar to the Boss Looper.

## Description
Loops incoming audio at a user defined interval.  

Use the simple controls to record loops, play and pause them, and record over them.   

Loops can be very long, or very short.  
  
[Source Code](https://github.com/electro-smith/DaisyExamples/tree/master/pod/Looper)

## Development
Modify the Makefile to point to the right DaisyExamples libDaisy and DaisySP, build them in the DaisyExamples directory.

`make` to make the binary

`make program-dfu` for the dfu programming

## Controls
| Control | Description | Comment |
| --- | --- | --- |
| Button 1 | Play / Pause| |
| Button 2 | Record | Press to record/SOS. Hold to reset |
| Leds | Modes | Red: record enable. Green: Playback |
| Knob 1 | Live In / Loop | Left is in, right is loop |




