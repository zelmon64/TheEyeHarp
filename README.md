# TheEyeHarp
The EyeHarp is a gaze controlled Digital Musical Instrument. The aim of the EyeHarp is to allow people with severe physical disabilities play music. 

In order to run, just execute "bin\EyeHarpVS.exe". If it does not run, you will have to install Visual C++ Redistributable for Visual Studio 2015, version x86.

The interface can be controlled either with the mouse cursor, or with an Eye-Tracker. For the moment the EyeTribe eye-tracking device and the Tobii PcEye are supported. If you have any problem setting up the Eye-Trackers, please contact me at zacharias.vamvakousis@upf.edu.
Using the interface with a mouse curson, can be usefull for other forms of input, such as head tracking devices, or trackballs. When the program is executed, by default the mouse is used as input. If an eye-tracking server is detected, by pressing the 'm' keyboard shortcut, the data coming from the eye-tracker are used instead. Alternatively the software provided by the Eye-Tracker can be used to control the mouse pointer with the Eye-tracker. On the top left of the screen more keyboard shortcuts appear.

The EyeHarp comes with a build-in synthesizer. Alternatively, it can be used as a midi controller. In that case LoopBe1 virtual midi port can be used to send midi messages to an external DAW, such as REAPER, that would host virtual instruments. The EyeHarp outputs in midi channels 1 (melody) and 2 (arpeggios).

The selections in the EyeHarp are made by looking at the cyrclic buttons for about 700ms. Alternatively the mouse click can be used (to alter between the two methods see keyboard shortcuts on the top left). 

Soon a extended manual will be added here. To get

