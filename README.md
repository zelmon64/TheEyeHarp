# TheEyeHarp
The EyeHarp is a gaze controlled Digital Musical Instrument. The aim of the EyeHarp is to allow people with severe physical disabilities play music. 

In order to run, just execute "bin\EyeHarpVS.exe". If it does not run, you will have to install Visual C++ Redistributable for Visual Studio 2015, version x86. Just google "Visual C++ Redistributable for Visual Studio 2015, version x86", go to Micrisoft's download page, make sure you select the vc_redist.x86.exe version and install it. 

The interface can be controlled either with the mouse cursor, or with an Eye-Tracker. For the moment the EyeTribe eye-tracking device and the Tobii eye-trackers are supported (after installing Tobii EyeX). If you have any problem setting up the Eye-Trackers, please contact me at zacharias.vamvakousis@upf.edu.
Using the interface with a mouse curson, can be usefull for other types of input devices, such as head tracking devices, or trackballs. 

The EyeHarp comes with a build-in synthesizer. Alternatively, it can be used as a midi controller. In that case LoopBe1 virtual midi port can be used to send midi messages to an external DAW, such as REAPER, that would host virtual instruments. The EyeHarp outputs in midi channels 1 (melody) and 2 (arpeggios).

A configuration file named "eyeharp.txt" is placed inside the bin folder. A number of settings can be adjusted. The following default values are set:

discNotesNumber 15          // reduce the number of notes in the screen if the accuracy of the eye tracker is low.
stepSequencerNotesNumber 6  // reduce the number of notes in the screen if the accuracy of the eye tracker is low.
chords 0                    // 0: not show chords, 1: show chords
showScale 0                 // show/hide the "pedals" of the harp
mouseEyetribeInput 0        // mouse input or input straight from the sdk of the eye tracker
clickDwell 1                // switch between click or dwell time selection method
bufferSize 512              // if you listen to sound crackles, increase the bufferSize
help 0                      // show/hide the available shortcuts
tomidi 0                    // switch between internal synth and midi out
fullscreen 0                // widowed/fullscreen mode.

