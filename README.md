![alt text](https://github.com/gameblabla/flappybird_vz200/raw/master/flappy.png "Screenshot")

This is a clone of Flappy Bird for Vtech Laser 200.

I originally worked on this project to see if i could make a decent game for this very weak computer.

It is almost entirely programmed in C with z88dk,

with the exception of the screen refreshing code coming from the dstar example.


The game should work on a non-expanded Vtech Laser 200.

On MAME, use the wav file to play it.

mame laser200 -cass flappy.wav -ab "CLOAD\nRUN"


On real hardware, you can either put the game on a real cassette, or if you own a computer with line in/out plugs,

you can simply type CLOAD on your machine and then play the wav file on your computer.

____________________________________________________

For such a simple game, i ran into several issues with z88dk...


For once, the Vtech specific functions provided by z88dk have no documentation so you need to guess what they are supposed to do.

My own sprite routines were very slow so i switched to the Z88dk ones instead.

Too bad they are in mono color though !

Sound seems to work properly only in text mode... 

In bitmap mode, it switches back to text mode before going back to bitmap mode, strange...


Current version have a small issue : 

The first time you play it, you need to confirm twice before you can play.

I do not know why, it's probably a bug with the compiler.

In all, it was a fun project.

Maybe i could make another one for this platform with better routines !