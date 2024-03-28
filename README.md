# Juce Midi VST3 Plugin Multi Sequence Manipulator

VST3 plugin able to be used in DAW that takes 2 sequences and allows user to perform all sorts of operations on them to generate midi output sequences in sync with DAW's tempo clock using C++ and Juce framework.

![alt_image](https://github.com/nathanfenoglio/Juce-Midi-VST3-Plugin-Multi-Sequence-Manipulator/blob/master/images/screenshot_1.jpg)

<h2 align="center">Operations: </h2>

- **Mod By Add By**: mod each # in sequence by "mod by" value then add "add by" value 
- **Add By Mod By**: add each # in sequence by "add by" value then mod by "mod by" value 
- **Rotate Left**: rotates the sequence to the left with the leftest # wrapping around to the rightest most index 
- **Rotate Right**: rotates the sequence to the right with the rightest # wrapping around to the first index 
- **Reverse**: reverses sequence 
- **Sum Inversion**: inverts each # in sequence, for instance if # is 3 and "sum inversion #" is 10 then new # will be 10 - 3 = 7 
- **Tower of Hanoi it**: takes the 1st "# discs t_o_h" # of digits in sequence, maps them to discs, and executes tower of hanoi discs on poles algorithm generating sequence by starting from bottom of poles going left to right on poles for each move until solved 
- **apply replacement**: replace a substring in whichever sequence with another substring 
- **splice sequences**: takes 1 element from 1 sequence and then 1 element from the other sequence repeatedly until both sequences are interleaved 
- **multiply sequences by scalars add together mod by**: multiplies each sequence by respective specified scalar, adds sequences together, then mods by "mod by" value