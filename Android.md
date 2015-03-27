# Introduction #

An Android accordion emulator. Emulates a three row diatonic and continental chromatic accordion. There is a choice of midi intruments and keys, including C System and B System. It may also be found on [Github](https://github.com/billthefarmer/accordion). The app is now on [F-Droid](https://f-droid.org/repository/browse/?fdid=org.billthefarmer.accordion).

![http://accordion.googlecode.com/svn/wiki/images/Accordion.png](http://accordion.googlecode.com/svn/wiki/images/Accordion.png)
![http://accordion.googlecode.com/svn/wiki/images/Accordion-25button.png](http://accordion.googlecode.com/svn/wiki/images/Accordion-25button.png)
![http://accordion.googlecode.com/svn/wiki/images/Accordion-10inch.png](http://accordion.googlecode.com/svn/wiki/images/Accordion-10inch.png)
![http://accordion.googlecode.com/svn/wiki/images/Accordion-settings.png](http://accordion.googlecode.com/svn/wiki/images/Accordion-settings.png)

  * Keys: F/Bb/Eb, G/C/F, A/D/G, C#/D/G, B/C/C#, C System, B System
  * Instruments: Standard midi set, defaults to accordion
  * Layouts: Standard Hohner 31 button and two 25 button layouts
  * Fascias: Choice of images

## Playing ##

Hold the tablet facing away from you the right way up. Play the buttons, touching the vertical grey bar as the bellows. If you don't know how to play button accordion, see http://info.melodeon.net. The buttons may be reversed using the slide button at the bottom.

## Technical Note ##

The standard documented android system does not support sending midi messages to the built in midi synthesizer. This app uses an undocumented interface to the synthesizer, which may possibly change with future android releases. This hasn't happened so far, but if it does the app may no longer work correctly. Hopefully, if this happens it will get fixed.

The button images are one of the buttons on my 70+ year old Hohner Lilliput.