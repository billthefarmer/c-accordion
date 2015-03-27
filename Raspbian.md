# Introduction #

A Raspbian accordion emulator. Emulates a three row diatonic and continental chromatic accordion. There is a choice of midi intruments and keys, including C System and B System. This version uses the android EAS Sonivox midi synthesizer which may be found on [Github](https://github.com/billthefarmer/raspi-sonivox)

![http://accordion.googlecode.com/svn/wiki/images/Accordion-raspi.png](http://accordion.googlecode.com/svn/wiki/images/Accordion-raspi.png)

  * Keys: F/Bb/Eb, G/C/F, A/D/G, C#/D/G, B/C/C#, C System, B System
  * Instruments: Standard midi set, defaults to accordion

## Playing ##

Hold the keyboard facing away from you with the escape key at the top. Play the keys, using the space bar as the bellows. If you don't know how to play button accordion, see http://info.melodeon.net. The buttons may be reversed using the reverse button.

## Building ##

The emulator has been tested successfully on both Raspbian and Arch Linux ARM with LXDE. Do not be tempted to upgrade the sources to GTK3, it becomes horribly slow and unresponsive on both Raspbian and Arch Linux.