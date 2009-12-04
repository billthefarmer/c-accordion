                              Accordion
                              =========

Three row accordion emulator. Uses the standard keyboard keys as
accordion buttons, and the space bar or the escape key as the
bellows. Unfortunately, depending on your luck, the keyboard on your
PC may not detect more than two or three buttons pressed at once. I
found an old keyboard worked much better. You can see what is going on
in the bottom half of the accordion window, which shows what keys the
program thinks are pressed. The bellows button is shown at the bottom
left.

Controls
========

There are three drop down controls, a check box, a slider, and a
button in the top half of the window. The instruments drop down lets
you choose what midi instrument you would like to attempt to play. The
reverse check box lets you play widdershins. The key drop down lets
you choose keys to play in. There is a choice of F/Bb/Eb, G/C/F,
A/D/G, C#/D/G, B/C/C#. The keys default to A/D/G. More kays can be
added by popular request, or you can hack the program yourself. The
layout drop down lets you choose a layout. The volume slider lets you
change the volume. The volume defaults to maximum. The quit button
lets you give up, or you can click on the window close icon.

Playing
=======

Hold the PC keyboard in a vertical position, so the keys are on the
side you want to play the melody on. Adjust the reverse check box
according to which hand you want to use. Press the keys as if they
were accordion buttons, and press the space bar or escape key to
emulate waggling the bellows. If you don't know how to play accordion
see http://info.melodeon.net

Files
=====

Changes			- Change log
COPYING			- Licence file
Makefile		- Build file
Accordion.c		- Source file
Accordion.exe		- Application
Accordion.manifest	- Manifest file
Accordion.nsi		- NSIS installer script
Accordion.rc		- Resource file
Readme.txt		- This file

Building
========

Use the makefile with Mingw or Cygwin GCC, or use a Microsoft C
compiler to build.
