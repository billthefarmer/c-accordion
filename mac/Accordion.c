////////////////////////////////////////////////////////////////////////////////
//
//  Melodeon - A Melodeon emulator written in C.
//
//  Copyright (C) 2009  Bill Farmer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//  Bill Farmer  william j farmer [at] tiscali [dot] co [dot] uk.
//
////////////////////////////////////////////////////////////////////////////////

#include <Carbon/Carbon.h>
#include <AudioUnit/AudioUnit.h>
#include <AudioToolbox/AudioToolbox.h>

// Macros

#define LENGTH(a) (sizeof(a) / sizeof(a[0]))

// Max volume

#define MAXVOL 127

// Buttons

#define ROWS     3
#define BUTTONS 11

// Button size

#define SIZE 34

// Command IDs

enum {
    kCommandInst    = 'inst',
    kCommandReverse = 'rvrs',
    kCommandKey     = 'key ',
    kCommandVolume  = 'vol ',
    kCommandLayout  = 'lay '};

// HIView IDs

HIViewID kHIViewIDReverse =
    {kCommandReverse, 101};

HIViewID kHIViewIDKey =
    {kCommandKey, 102};

HIViewID kHIViewIDLayout =
    {kCommandLayout, 103};

// Key IDs

enum {
    kKeyboardAKey     = 0x00,
    kKeyboardBKey     = 0x0b,
    kKeyboardCKey     = 0x08,
    kKeyboardDKey     = 0x02,
    kKeyboardEKey     = 0x0e,
    kKeyboardFKey     = 0x03,
    kKeyboardGKey     = 0x05,
    kKeyboardHKey     = 0x04,
    kKeyboardIKey     = 0x22,
    kKeyboardJKey     = 0x26,
    kKeyboardKKey     = 0x28,
    kKeyboardLKey     = 0x25,
    kKeyboardMKey     = 0x2e,
    kKeyboardNKey     = 0x2d,
    kKeyboardOKey     = 0x1f,
    kKeyboardPKey     = 0x23,
    kKeyboardQKey     = 0x0c,
    kKeyboardRKey     = 0x0f,
    kKeyboardSKey     = 0x01,
    kKeyboardTKey     = 0x11,
    kKeyboardUKey     = 0x20,
    kKeyboardVKey     = 0x09,
    kKeyboardWKey     = 0x0d,
    kKeyboardXKey     = 0x07,
    kKeyboardYKey     = 0x10,
    kKeyboardZKey     = 0x06,

    kKeyboardEscapeKey = 0x35,
    kKeyboardSpaceKey  = 0x31,
    kKeyboardCommaKey  = 0x2b,
    kKeyboardStopKey   = 0x2f,
    kKeyboardSlashKey  = 0x2c,
    kKeyboardColonKey  = 0x29,
    kKeyboardQuoteKey  = 0x27,
    kKeyboardLBrktKey  = 0x21};

// Keyboard modifiers

enum {
    kKeyboardCommandMask = 0x0100,
    kKeyboardShiftMask   = 0x0200,
    kKeyboardOptionMask  = 0x0800,
    kKeyboardControlMask = 0x1000};

// Midi messages

enum {
    kMidiMessageNoteOn        = 0x90,
    kMidiMessageNoteOff       = 0x80,
    kMidiMessageBassOn        = 0x91,
    kMidiMessageBassOff       = 0x81,
    kMidiMessageChordOn       = 0x92,
    kMidiMessageChordOff      = 0x82,
    kMidiMessageControlChange = 0xB0,
    kMidiMessageProgramChange = 0xC0};

// Midi control codes

enum {
    kMidiControlAllSoundOff  = 0x78,
    kMidiResetAllControllers = 0x79,
    kMidiControlAllNotesOff  = 0x7B};

// List of midi instruments

char *instruments[] =
    {"Acoustic Grand Piano", "Bright Acoustic Piano",
     "Electric Grand Piano", "Honky-tonk Piano",
     "Electric Piano 1", "Electric Piano 2", "Harpsichord",
     "Clavi", "Celesta", "Glockenspiel", "Music Box",
     "Vibraphone", "Marimba", "Xylophone", "Tubular Bells",
     "Dulcimer", "Drawbar Organ", "Percussive Organ",
     "Rock Organ", "Church Organ", "Reed Organ",
     "Accordion", "Harmonica", "Tango Accordion",
     "Acoustic Guitar (nylon)", "Acoustic Guitar (steel)",
     "Electric Guitar (jazz)", "Electric Guitar (clean)",
     "Electric Guitar (muted)", "Overdriven Guitar",
     "Distortion Guitar", "Guitar harmonics",
     "Acoustic Bass", "Electric Bass (finger)",
     "Electric Bass (pick)", "Fretless Bass",
     "Slap Bass 1", "Slap Bass 2", "Synth Bass 1",
     "Synth Bass 2", "Violin", "Viola", "Cello",
     "Contrabass", "Tremolo Strings", "Pizzicato Strings",
     "Orchestral Harp", "Timpani", "String Ensemble 1",
     "String Ensemble 2", "SynthStrings 1", "SynthStrings 2",
     "Choir Aahs", "Voice Oohs", "Synth Voice",
     "Orchestra Hit", "Trumpet", "Trombone", "Tuba",
     "Muted Trumpet", "French Horn", "Brass Section",
     "SynthBrass 1", "SynthBrass 2", "Soprano Sax",
     "Alto Sax", "Tenor Sax", "Baritone Sax", "Oboe",
     "English Horn", "Bassoon", "Clarinet", "Piccolo",
     "Flute", "Recorder", "Pan Flute", "Blown Bottle",
     "Shakuhachi", "Whistle", "Ocarina", "Lead 1 (square)",
     "Lead 2 (sawtooth)", "Lead 3 (calliope)", "Lead 4 (chiff)",
     "Lead 5 (charang)", "Lead 6 (voice)", "Lead 7 (fifths)",
     "Lead 8 (bass + lead)", "Pad 1 (new age)", "Pad 2 (warm)",
     "Pad 3 (polysynth)", "Pad 4 (choir)", "Pad 5 (bowed)",
     "Pad 6 (metallic)", "Pad 7 (halo)", "Pad 8 (sweep)",
     "FX 1 (rain)", "FX 2 (soundtrack)", "FX 3 (crystal)",
     "FX 4 (atmosphere)", "FX 5 (brightness)", "FX 6 (goblins)",
     "FX 7 (echoes)", "FX 8 (sci-fi)", "Sitar", "Banjo",
     "Shamisen", "Koto", "Kalimba", "Bag pipe", "Fiddle",
     "Shanai", "Tinkle Bell", "Agogo", "Steel Drums",
     "Woodblock", "Taiko Drum", "Melodic Tom", "Synth Drum",
     "Reverse Cymbal", "Guitar Fret Noise", "Breath Noise",
     "Seashore", "Bird Tweet", "Telephone Ring",
     "Helicopter", "Applause", "Gunshot"};

int instrument;

// List of keys and offset values

char *keys[] =
    {"F/Eb/Bb", "G/C/F", "A/D/G", "C#/D/G", "B/C/C#"};

int keyvals[LENGTH(keys)][ROWS] =
    {{ 3, -2, -7},
     { 5,  0, -5},
     { 7,  2, -3},
     { 7,  2,  1},
     { 1,  0, -1}};

//      Eb  Bb   F   C   G   D   A
//     { 3, -2,  5,  0, -5,  2, -3};

int key;

// Layouts

char *layouts[] =
    {"Low notes", "Accidentals"};

int layout;

// Keyboard

int keyboard[3][11] =
    {{kKeyboardWKey,
      kKeyboardEKey,
      kKeyboardRKey,
      kKeyboardTKey,
      kKeyboardYKey,
      kKeyboardUKey,
      kKeyboardIKey,
      kKeyboardOKey,
      kKeyboardPKey,
      kKeyboardLBrktKey,
      -1},
     {kKeyboardAKey,
      kKeyboardSKey,
      kKeyboardDKey,
      kKeyboardFKey,
      kKeyboardGKey,
      kKeyboardHKey,
      kKeyboardJKey,
      kKeyboardKKey,
      kKeyboardLKey,
      kKeyboardColonKey,
      kKeyboardQuoteKey},
     {kKeyboardZKey,
      kKeyboardXKey,
      kKeyboardCKey,
      kKeyboardVKey,
      kKeyboardBKey,
      kKeyboardNKey,
      kKeyboardMKey,
      kKeyboardCommaKey,
      kKeyboardStopKey,
      kKeyboardSlashKey,
      -1}};

char *keytops[3] =
    {"WERTYUIOP-",
     "ASDFGHJKL--",
     "ZXCVBNM---"};

// Midi notes for 'C'

int notes[BUTTONS][2] =
    {{52, 57},
     {55, 59},
     {60, 62},
     {64, 65},
     {67, 69},
     {72, 71},
     {76, 74},
     {79, 77},
     {84, 81},
     {88, 83},
     {91, 86}}; 

// Synthesizer unit

AudioUnit synthUnit;

// Buttons

Boolean buttons[ROWS][BUTTONS];
HIViewRef display[ROWS][BUTTONS];

// Bellows handle

HIViewRef spacebar;
Boolean bellows;

// Reverse value

Boolean reverse = false;

// Volume value

int volume = MAXVOL;

// Function prototypes

OSStatus ApplicationHandler(EventHandlerCallRef, EventRef, void*);
OSStatus KeyboardHandler(EventHandlerCallRef, EventRef, void*);
OSStatus ComboBoxHandler(EventHandlerCallRef, EventRef, void*);
OSStatus CommandHandler(EventHandlerCallRef, EventRef, void*);
OSStatus WindowHandler(EventHandlerCallRef, EventRef, void*);
OSStatus MouseHandler(EventHandlerCallRef, EventRef, void*);

void ChangeInstrument(int);

// Function main

int main(int argc, char *argv[])
{
    WindowRef window;
    HIViewRef content;
    HIViewRef combo;
    HIViewRef group;
    HIViewRef check;
    HIViewRef text;
    HIViewRef slider;
    HIViewRef quit;

    MenuRef menu;
    HIRect rect;
    int i;

    // Window bounds

    Rect bounds = {0, 0, 436, 590};

    // Create window

    CreateNewWindow(kDocumentWindowClass,
                    kWindowStandardFloatingAttributes |
                    kWindowStandardHandlerAttribute |
		    kWindowInWindowMenuAttribute |
                    kWindowCompositingAttribute,
                    &bounds, &window);

    // Set the title

    SetWindowTitleWithCFString(window, CFSTR("Accordion"));

    // Create an application menu

    CreateNewMenu(0, 0, &menu);

    // Set menu title

    SetMenuTitleWithCFString(menu,
	CFStringCreateWithPascalString(kCFAllocatorDefault,
                                       "\p\024",
                                       kCFStringEncodingMacRoman));
    // Create an about item

    InsertMenuItemTextWithCFString(menu, CFSTR("About Accordion"),
                                   0, 0, kHICommandAbout);

    // Insert the menu

    InsertMenu(menu, 0);

    // Create a standard window menu

    CreateStandardWindowMenu(0, &menu);

    // Insert the menu

    InsertMenu(menu, 0);

    // Show and position the window

    ShowWindow(window);
    RepositionWindow(window, NULL, kWindowCascadeOnMainScreen);

    // Find the window content

    HIViewFindByID(HIViewGetRoot(window),
                   kHIViewWindowContentID,
                   &content);

    // Set bounds for group box

    bounds.bottom = 92;
    bounds.right  = 550;

    // Create group box

    CreateGroupBoxControl(window, &bounds, NULL, true, &group);

    // Place in the window

    HIViewAddSubview(content, group);
    HIViewPlaceInSuperviewAt(group, 20, 20);

    // Bounds of text

    bounds.bottom = 16;
    bounds.right = 74;

    // Create static text

    CreateStaticTextControl(window, &bounds, CFSTR("Instrument:"),
			    NULL, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 16, 18);

    // Bounds of combo box

    rect.size.height = 20;
    rect.size.width = 168;    

    // Create combo box

    HIComboBoxCreate(&rect, CFSTR("Accordion"), NULL, NULL,
                     kHIComboBoxStandardAttributes,
                     &combo);

    // Set visible and set command ID

    HIViewSetVisible(combo, true);
    HIViewSetCommandID(combo, kCommandInst); 

    // Add the instruments

    for (i = 0; i < LENGTH(instruments); i++)
    {
        HIComboBoxAppendTextItem(combo,
            CFStringCreateWithCString(kCFAllocatorDefault,
                                      instruments[i],
                                      kCFStringEncodingMacRoman), NULL);

        // Set the current instrument

        if (strcmp(instruments[i], "Accordion") == 0)
            instrument = i;
    }

    // Place in the group box

    HIViewAddSubview(group, combo);
    HIViewPlaceInSuperviewAt(combo, 102, 16);

    // Bounds of check box

    bounds.bottom = 18;
    bounds.right = 121;

    // Create check box

    CreateCheckBoxControl(window, &bounds, CFSTR("Reverse Buttons"),
                          false, true, &check);

    // Set the control ID and the command ID

    HIViewSetID(check, kHIViewIDReverse);
    HIViewSetCommandID(check, kCommandReverse); 

    // Place in the group box

    HIViewAddSubview(group, check);
    HIViewPlaceInSuperviewAt(check, 286, 17);

    // Bounds of text

    bounds.bottom = 16;
    bounds.right  = 32;

    // Create static text

    CreateStaticTextControl(window, &bounds, CFSTR("Key:"), NULL, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 420, 18);

    // Bounds of combo box

    rect.size.width = 70;

    // Create combo box

    HIComboBoxCreate(&rect, CFSTR("A/D/G"), NULL, NULL,
                     kHIComboBoxStandardAttributes,
                     &combo);

    // Set visible and set command ID

    HIViewSetVisible(combo, true);
    HIViewSetID(combo, kHIViewIDKey);
    HIViewSetCommandID(combo, kCommandKey); 

    // Add keys

    for (i = 0; i < LENGTH(keys); i++)
    {
        HIComboBoxAppendTextItem(combo,
            CFStringCreateWithCString(kCFAllocatorDefault,
                                      keys[i],
                                      kCFStringEncodingMacRoman), NULL);

        // Set current key

        if (strcmp(keys[i], "A/D/G") == 0)
            key = i;
    }

    // Place in the group box

    HIViewAddSubview(group, combo);
    HIViewPlaceInSuperviewAt(combo, 460, 16);

    // Bounds of text

    bounds.bottom = 16;
    bounds.right  = 54;

    // Create static text

    CreateStaticTextControl(window, &bounds, CFSTR("Volume:"), NULL, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 16, 56);
    
    // Bounds of slider

    bounds.bottom = 16;
    bounds.right  = 168;

    // Create slider

    CreateSliderControl(window, &bounds, MAXVOL, 0, MAXVOL,
                        kControlSliderDoesNotPoint, 0, false, NULL, &slider);

    // Set command ID

    HIViewSetCommandID(slider, kCommandVolume); 

    // Place in the group box

    HIViewAddSubview(group, slider);
    HIViewPlaceInSuperviewAt(slider, 100, 58);

    // Bounds of text

    bounds.bottom = 16;
    bounds.right  = 48;

    // Create static text

    CreateStaticTextControl(window, &bounds, CFSTR("Layout:"), NULL, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 286, 56);

    // Bounds of combo box

    rect.size.width = 98;

    // Create combo box

    HIComboBoxCreate(&rect, CFSTR("Accidentals"), NULL, NULL,
                     kHIComboBoxStandardAttributes,
                     &combo);

    // Set visible and set command ID

    HIViewSetVisible(combo, true);
    HIViewSetID(combo, kHIViewIDLayout);
    HIViewSetCommandID(combo, kCommandLayout); 

    // Add layouts

    for (i = 0; i < LENGTH(layouts); i++)
    {
        HIComboBoxAppendTextItem(combo,
            CFStringCreateWithCString(kCFAllocatorDefault,
                                      layouts[i],
                                      kCFStringEncodingMacRoman), NULL);

        // Set current layout

        if (strcmp(layouts[i], "Accidentals") == 0)
            layout = i;
    }

    // Place in the group box

    HIViewAddSubview(group, combo);
    HIViewPlaceInSuperviewAt(combo, 346, 54);

    // Bounds of push button

    bounds.bottom = 20;
    bounds.right  = 70;

    // Create push button

    CreatePushButtonControl(window, &bounds, CFSTR("Quit"), &quit);

    // Set command ID

    HIViewSetCommandID(quit, kHICommandQuit); 

    // Place in the group box

    HIViewAddSubview(group, quit);
    HIViewPlaceInSuperviewAt(quit, 460, 54);

    // Group box bounds

    bounds.bottom = 92;
    bounds.right  = 550;

    // Create group box

    CreateGroupBoxControl(window, &bounds, NULL, true, &group);

    // Place in the window

    HIViewAddSubview(content, group);
    HIViewPlaceInSuperviewAt(group, 20, 132);

    // Font style

    ControlFontStyleRec style;
    style.flags = kControlUseFontMask|kControlUseJustMask;
    style.font = kControlFontBigSystemFont;
    style.just = teCenter;

    // Bounds of text
    
    bounds.bottom = 16;
    bounds.right  = 550;

    // Create static text

    CreateStaticTextControl(window, &bounds, CFSTR("Accordion"),
                            &style, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 0, 26);

    // Bounds of text

    bounds.bottom = 16;
    bounds.right  = 550;

    // Create static text

    CreateStaticTextControl(window, &bounds,
                            CFSTR("Play accordion on your keyboard"),
                            &style, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 0, 50);

    // Group box bounds

    bounds.bottom = 152;
    bounds.right  = 550;

    // Create group box

    CreateGroupBoxControl(window, &bounds, NULL, true, &group);

    // Place in the window

    HIViewAddSubview(content, group);
    HIViewPlaceInSuperviewAt(group, 20, 244);

    int j;
    static char s[] = " ";

    // button bounds

    bounds.bottom = SIZE;
    bounds.right  = SIZE;

    // Create three rows of buttons

    for (i = 0; i < LENGTH(display); i++)
    {
	for (j = 0; j < ((i == 1)? LENGTH(display[i]):
			 LENGTH(display[i]) - 1); j++)
	{
	    int x = (i == 1)? 38 + 44 * j: 60 + 44 * j;
	    int y = 15 + 44 * i;
	    s[0] = keytops[i][j];

	    // Create button

	    CreateBevelButtonControl(window, &bounds,
                CFStringCreateWithCString(kCFAllocatorDefault,
                                          s, kCFStringEncodingMacRoman),
				     kControlBevelButtonNormalBevel,
				     kControlBehaviorPushbutton,
				     NULL, 0, 0, 0, &display[i][j]);

	    // Place in the group box

	    HIViewAddSubview(group, display[i][j]);
	    HIViewPlaceInSuperviewAt(display[i][j], x, y);
        }
    }

    // Create spacebar button

    CreateBevelButtonControl(window, &bounds,
			     NULL,
			     kControlBevelButtonNormalBevel,
			     kControlBehaviorPushbutton,
			     NULL, 0, 0, 0, &spacebar);

    // Place in the group box

    HIViewAddSubview(group, spacebar);
    HIViewPlaceInSuperviewAt(spacebar, 16, 103);

    // Group box bounds, wider than the window to hide rounded corners

    bounds.bottom = 20;
    bounds.right = 598;

    // Create group box for fake status bar

    CreateGroupBoxControl(window, &bounds, NULL, false, &group);

    // Place in window at negative offset to hide rounded corners

    HIViewAddSubview(content, group);
    HIViewPlaceInSuperviewAt(group, -4, 416);

    // Text bounds

    bounds.bottom = 16;
    bounds.right  = 590;

    // Font style

    style.flags = kControlUseFontMask|kControlUseJustMask;
    style.font = kControlFontSmallSystemFont;
    style.just = teCenter;

    // Create static text

    CreateStaticTextControl(window, &bounds,
        CFSTR("Press the function keys F1-F12 as melodeon buttons "
              "and the space bar as the bellows. 4th button start."),
                            &style, &text);

    // Place in group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 0, 2);

    // Application events type spec

    EventTypeSpec applicationEvents[] =
        {{kEventClassApplication, kEventAppFrontSwitched}};

    // Install event handler

    InstallApplicationEventHandler(NewEventHandlerUPP(ApplicationHandler),
                                   LENGTH(applicationEvents), applicationEvents,
                                   NULL, NULL);

    // Mouse events type spec

    EventTypeSpec mouseEvents[] =
	{{kEventClassMouse, kEventMouseDown}};

    // Install event handler on the event dispatcher, so that we can
    // see mouse events before the default handler gets them

    InstallEventHandler(GetEventDispatcherTarget(),
                        NewEventHandlerUPP(MouseHandler),
                        LENGTH(mouseEvents), mouseEvents,
                        NULL, NULL);

    // Window events type spec

    EventTypeSpec windowEvents[] =
        {{kEventClassWindow, kEventWindowClose}};

    // Install event handler

    InstallWindowEventHandler(window, NewEventHandlerUPP(WindowHandler),
                              LENGTH(windowEvents), windowEvents,
                              NULL, NULL);

    // Combo box events type spec

    EventTypeSpec comboBoxEvents[] =
        {{kEventClassHIComboBox, kEventComboBoxListItemSelected}};

    // Install event handler

    InstallApplicationEventHandler(NewEventHandlerUPP(ComboBoxHandler),
                                   LENGTH(comboBoxEvents), comboBoxEvents,
                                   NULL, NULL);

    // Command events type spec

    EventTypeSpec commandEvents[] =
        {{kEventClassCommand, kEventCommandProcess}};

    // Install event handler

    InstallApplicationEventHandler(NewEventHandlerUPP(CommandHandler),
                                   LENGTH(commandEvents), commandEvents,
                                   NULL, NULL);

    // Keyboard events type spec

    EventTypeSpec keyboardEvents[] =
        {{kEventClassKeyboard, kEventRawKeyDown},
         {kEventClassKeyboard, kEventRawKeyUp},
         {kEventClassKeyboard, kEventRawKeyModifiersChanged}};

    // Install event handler on the event dispatcher

    InstallEventHandler(GetEventDispatcherTarget(),
			NewEventHandlerUPP(KeyboardHandler),
			LENGTH(keyboardEvents), keyboardEvents,
			NULL, NULL);

    // Audio Unit graph

    AUGraph graph;

    // Audio Unit synthesizer and output node

    AUNode synthNode;
    AUNode outNode;

    // Component description

    ComponentDescription cd;
    cd.componentManufacturer = kAudioUnitManufacturer_Apple;
    cd.componentFlags = 0;
    cd.componentFlagsMask = 0;

    // New AU graph

    NewAUGraph(&graph);

    // Synthesizer

    cd.componentType = kAudioUnitType_MusicDevice;
    cd.componentSubType = kAudioUnitSubType_DLSSynth;

    // New synthesizer node

    AUGraphNewNode(graph, &cd, 0, NULL, &synthNode);

    // Output

    cd.componentType = kAudioUnitType_Output;
    cd.componentSubType = kAudioUnitSubType_DefaultOutput;
 
    // New output node

    AUGraphNewNode(graph, &cd, 0, NULL, &outNode);

    // Open graph

    AUGraphOpen(graph);

    // Connect synthesizer node to output node

    AUGraphConnectNodeInput(graph, synthNode, 0, outNode, 0);

    // Get a synthesizer unit

    AUGraphGetNodeInfo(graph, synthNode, NULL, 0, NULL, &synthUnit);

    // Initialise

    AUGraphInitialize(graph);

    // Start

    AUGraphStart(graph);

    // Show the graph

//     CAShow(graph);

    // Change instrument

    ChangeInstrument(instrument);

    // Run the application event loop

    RunApplicationEventLoop();

    // Stop the graph

    AUGraphStop(graph);

    // Dispose of the graph

    DisposeAUGraph(graph);

    // Exit

    return 0;
}

// Application Handler

OSStatus ApplicationHandler(EventHandlerCallRef next,
                            EventRef event, void *data)
{
    ProcessSerialNumber them;
    ProcessSerialNumber us;
    Boolean same;
    UInt32 kind;
    int i, j;

    // Get event kind

    kind = GetEventKind(event);

    // switch on event kind

    switch (kind)
    {
	// Front app switched

    case kEventAppFrontSwitched:

        // Get their process id

        GetEventParameter(event, kEventParamProcessID,
                          typeProcessSerialNumber, NULL, sizeof(them),
                          NULL, &them);

        // Get our process id

        GetCurrentProcess(&us);

        // Is it the same?

        SameProcess(&them, &us, &same);

        // If not the same

        if (!same)
        {
            // Turn all the notes off and reset all the buttons that
            // are down

            for (i = 0; i < LENGTH(buttons); i++)
            {
                for (j = 0; j < LENGTH(buttons[i]); j++)
                {
                    // Melody buttons

                    if (buttons[i][j])
                    {
                        buttons[i][j] = false;
                        HIViewSetValue(display[j][j], false);

                        int note = notes[j][bellows] + keyvals[key][i];
                        MusicDeviceMIDIEvent(synthUnit, kMidiMessageNoteOff,
                                             note, 0, 0);
                    }
                }
            }

            if (bellows)
            {
                // Space bar button

                bellows = false;
            }
        }
        break;

    default:
        return eventNotHandledErr;
    }

    // Return ok

    return noErr;
}

// Mouse handler

OSStatus MouseHandler(EventHandlerCallRef next,
		      EventRef event, void *data)
{
    HIViewRef control;
    WindowRef window;
    ControlKind view;
    UInt32 kind;

    // Get the event kind

    kind = GetEventKind(event);

    // Get the window

    GetEventParameter(event, kEventParamWindowRef,
                      typeWindowRef, NULL, sizeof(window),
                      NULL, &window);

    // Get the view

    HIViewGetViewForMouseEvent(HIViewGetRoot(window), event,
			       &control);

    // Get the control kind

    GetControlKind(control, &view);

    // Switch on event kind

    switch (kind)
    {
        // Mouse down

    case kEventMouseDown:

        // Switch on control kind

        switch (view.kind)
        {
            // Combo box

        case kControlKindHIComboBox:

            // Do nothing, let the default event handler do it's thing

            break;

            // Everything else

        default:

            // Clear the keyboard focus, otherwise the focus stays on the
            // combo box and makes it drop down when the user presses a key

            ClearKeyboardFocus(window);
            break;
        }

    default:
        return eventNotHandledErr;
    }

    // Return event not handled so that the default event handler gets
    // to do it's thing

    return eventNotHandledErr;
}

// Window handler

OSStatus WindowHandler(EventHandlerCallRef next,
                       EventRef event, void *data)
{
    WindowRef window;
    UInt32 kind;

    // Get the event kind

    kind = GetEventKind(event);

    // Get the window

    GetEventParameter(event, kEventParamDirectObject,
                      typeWindowRef, NULL, sizeof(window),
                      NULL, &window);

    // Switch on event kind

    switch (kind)
    {
        // Window close event

    case kEventWindowClose:

        // Quit the application, as it only has one window

        QuitApplicationEventLoop();
        break;

    default:
        return eventNotHandledErr;
    }

    // Return ok

    return noErr;
}

// Command handler

OSStatus CommandHandler(EventHandlerCallRef next,
                        EventRef event, void *data)
{
    HICommandExtended command;
    WindowRef window;
    UInt32 value;

    // Get the command

    GetEventParameter(event, kEventParamDirectObject,
                      typeHICommand, NULL, sizeof(command),
                      NULL, &command);

    // Get the value

    value = HIViewGetValue(command.source.control);

    // Switch on the command ID

    switch (command.commandID)
    {
        // Key, instrument, or layout control

    case kCommandKey:
    case kCommandInst:
    case kCommandLayout:

	// If the combo box list isn't visible (the user just closed
	// it)

        if (!HIComboBoxIsListVisible(command.source.control))
        {
            // Get the window
    
            window = ActiveNonFloatingWindow();

            // Clear the keyboard focus, otherwise the focus stays on the
            // combo box and makes it drop down when the user presses a key

            ClearKeyboardFocus(window);
        }
        break;

        // Reverse

    case kCommandReverse:
        reverse = value;
        break;

        // Volume

    case kCommandVolume:
        volume = value;
        break;

        // Quit

    case kHICommandQuit:

        // Let the default handler handle it

    default:
        return eventNotHandledErr;
    }

    // Report success

    return noErr;
}

// Combo box handler

OSStatus ComboBoxHandler(EventHandlerCallRef next,
                         EventRef event, void *data)
{
    WindowRef window;
    HIViewRef combo;
    CFIndex index;
    UInt32 id;

    // Get the window
    
    window = ActiveNonFloatingWindow();

    // Get the control

    GetEventParameter(event, kEventParamDirectObject,
                      typeControlRef, NULL, sizeof(combo),
                      NULL, &combo);

    // Get the index

    GetEventParameter(event, kEventParamComboBoxListSelectedItemIndex,
                      typeCFIndex, NULL, sizeof(index),
                      NULL, &index);

    // Get the command id

    HIViewGetCommandID(combo, &id);

    // Switch on the command id
        
    switch (id)
    {
        // Instrument

    case kCommandInst:
        instrument = index;
        ChangeInstrument(instrument);
        break;

        // Key

    case kCommandKey:
        key = index;
	HIViewFindByID(HIViewGetRoot(window),
		       kHIViewIDLayout,
		       &combo);
	if (key > 2)
	    HIViewSetText(combo, CFSTR("Low Notes"));

	else
	    HIViewSetText(combo, CFSTR("Accidentals"));

        break;

        // Layout

    case kCommandLayout:
        layout = index;
	HIViewFindByID(HIViewGetRoot(window),
		       kHIViewIDKey,
		       &combo);

	if (layout > 0)
	    HIViewSetText(combo, CFSTR("A/D/G"));

	else
	    HIViewSetText(combo, CFSTR("C#/D/G"));

        break;

        // Something else

    default:
        return eventNotHandledErr;
    }

    // Clear the keyboard focus, otherwise the focus stays on the
    // combo box and makes it drop down when the user presses a key

    ClearKeyboardFocus(window);

    // Report success

    return noErr;
}

// Keyboard handler

OSStatus  KeyboardHandler(EventHandlerCallRef next,
                          EventRef event, void *data)
{
    UInt32 kind;
    UInt32 keyCode;
    int i, j;

    // Get the event kind

    kind = GetEventKind(event);

    // Switch on event kind

    switch (kind)
    {
        // Modifiers changed

    case kEventRawKeyModifiersChanged:

        return noErr;

	// Key down or key up

    case kEventRawKeyDown:

        // Get the key code

        GetEventParameter(event, kEventParamKeyCode, typeUInt32,
                          NULL, sizeof(keyCode), NULL, &keyCode);

        switch (keyCode)
        {
            // Space bar

	case kKeyboardEscapeKey:
        case kKeyboardSpaceKey:

            // If the key isn't already down

            if (!bellows)
            {
                bellows = true;
                HIViewSetValue(spacebar, true);

                // Check the keys

		for (i = 0; i < LENGTH(buttons); i++)
		{
                    for (j = 0; j < LENGTH(buttons[i]); j++)
                    {
                        // If a key is down

                        if (buttons[i][j])
                        {
                            // Stop the current note

                            int note = notes[j][!bellows] + keyvals[key][i];
                            MusicDeviceMIDIEvent(synthUnit,
						 kMidiMessageNoteOff + i,
                                                 note, 0, 0);

                            // Play the new note

                            note = notes[j][bellows] + keyvals[key][i];
                            MusicDeviceMIDIEvent(synthUnit,
						 kMidiMessageNoteOn + i,
                                                 note, volume, 0);
                        }
		    }
		}
            }
            break;

	    // Check keyboard

	default:

	    // Look up the key code in the keyboard table

	    for (i = 0; i < LENGTH(keyboard); i++)
	    {
		for (j = 0; j < LENGTH(keyboard[i]); j++)
		{
		    if (keyboard[i][j] == keyCode && !buttons[i][j])
		    {
			buttons[i][j] = true;
			HIViewSetValue(display[i][j], true);

			// Play the note

			int k;

			switch (i)
			{
			case 0:
			    k = (reverse)? LENGTH(buttons[i]) - j - 2: j;
			    break;

			case 1:
			    k = (reverse)? LENGTH(buttons[i]) - j - 1: j;
			    break;

			case 2:
			    k = (reverse)? LENGTH(buttons[i]) - j - 1: j + 1;
			    break;
			}

			int note = notes[k][bellows] + keyvals[key][i];
			MusicDeviceMIDIEvent(synthUnit,
					     kMidiMessageNoteOn + i,
					     note, volume, 0);
			return noErr;
		    }
		}
	    }
	    break;
	}
	break;

    case kEventRawKeyUp:

        // Get the key code

        GetEventParameter(event, kEventParamKeyCode, typeUInt32,
                          NULL, sizeof(keyCode), NULL, &keyCode);

	switch (keyCode)
	{
            // Space bar

	case kKeyboardEscapeKey:
	case kKeyboardSpaceKey:

            // If the key isn't already up

            if (bellows)
            {
                bellows = false;
                HIViewSetValue(spacebar, false);

                // Check the keys

		for (i = 0; i < LENGTH(buttons); i++)
		{
                    for (j = 0; j < LENGTH(buttons[i]); j++)
                    {
                        // If a key is down

                        if (buttons[i][j])
                        {
                            // Stop the current note

                            int note = notes[j][!bellows] + keyvals[key][i];
                            MusicDeviceMIDIEvent(synthUnit,
						 kMidiMessageNoteOff + i,
                                                 note, 0, 0);

                            // Play the new note

                            note = notes[j][bellows] + keyvals[key][i];
                            MusicDeviceMIDIEvent(synthUnit,
						 kMidiMessageNoteOn + i,
                                                 note, volume, 0);
                        }
		    }
		}
            }
            break;

	    // Check keyboard

	default:

	    // Look up the key code in the keyboard table

	    for (i = 0; i < LENGTH(keyboard); i++)
	    {
		for (j = 0; j < LENGTH(keyboard[i]); j++)
		{
		    if (keyboard[i][j] == keyCode && buttons[i][j])
		    {
			buttons[i][j] = false;
			HIViewSetValue(display[i][j], false);

			// Stop note

			int k;

			switch (i)
			{
			case 0:
			    k = (reverse)? LENGTH(buttons[i]) - j - 2: j;
			    break;

			case 1:
			    k = (reverse)? LENGTH(buttons[i]) - j - 1: j;
			    break;

			case 2:
			    k = (reverse)? LENGTH(buttons[i]) - j - 1: j + 1;
			    break;
			}

			int note = notes[k][bellows] + keyvals[key][i];
			MusicDeviceMIDIEvent(synthUnit,
					     kMidiMessageNoteOff + i,
					     note, 0, 0);
			return noErr;
		    }
		}
	    }
	    break;
	}
	break;

    default:
        return eventNotHandledErr;
    }

    // Report success

    return noErr;
}

// Change instrument

void ChangeInstrument(int instrument)
{
    // Send a program change message on all three channels

    MusicDeviceMIDIEvent(synthUnit, kMidiMessageProgramChange + 0,
                         instrument, 0, 0);
    MusicDeviceMIDIEvent(synthUnit, kMidiMessageProgramChange + 1,
                         instrument, 0, 0);
    MusicDeviceMIDIEvent(synthUnit, kMidiMessageProgramChange + 2,
                         instrument, 0, 0);
}
