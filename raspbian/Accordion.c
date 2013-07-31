////////////////////////////////////////////////////////////////////////////////
//
//  Accordion - A simple Accordion emulator written in C.
//
//  Copyright (C) 2013  Bill Farmer
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
//  Bill Farmer  william j farmer [at] yahoo [dot] co [dot] uk.
//
///////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "midi.h"

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>

// Macros

#define Length(a) (sizeof(a) / sizeof(a[0]))

// Types

enum
  {DIATONIC,
   CHROMATIC};

// Midi codes

enum
    {NOTEOFF = 0x80,
     NOTEON  = 0x90,
     CHANGE  = 0xc0};

// Buttons

enum
  {ROWS        =  3,
   TYPES       =  2,
   BUTTONS     = 11,
   DIRECTIONS  =  2,
   BASSBUTTONS = 12};

// Button size

enum
  {BUTTONSIZE = 34};

// Max volume

enum
    {MAXVOL = 127};

// Button size

enum
    {SIZE = 34};

// Space

enum
    {MARGIN = 10};

// List of midi instruments

static char *instruments[] =
    {" Acoustic Grand Piano", " Bright Acoustic Piano",
     " Electric Grand Piano", " Honky-tonk Piano",
     " Electric Piano 1", " Electric Piano 2", " Harpsichord",
     " Clavi", " Celesta", " Glockenspiel", " Music Box",
     " Vibraphone", " Marimba", " Xylophone", " Tubular Bells",
     " Dulcimer", " Drawbar Organ", " Percussive Organ",
     " Rock Organ", " Church Organ", " Reed Organ",
     " Accordion", " Harmonica", " Tango Accordion",
     " Acoustic Guitar (nylon)", " Acoustic Guitar (steel)",
     " Electric Guitar (jazz)", " Electric Guitar (clean)",
     " Electric Guitar (muted)", " Overdriven Guitar",
     " Distortion Guitar", " Guitar harmonics",
     " Acoustic Bass", " Electric Bass (finger)",
     " Electric Bass (pick)", " Fretless Bass",
     " Slap Bass 1", " Slap Bass 2", " Synth Bass 1",
     " Synth Bass 2", " Violin", " Viola", " Cello",
     " Contrabass", " Tremolo Strings", " Pizzicato Strings",
     " Orchestral Harp", " Timpani", " String Ensemble 1",
     " String Ensemble 2", " SynthStrings 1", " SynthStrings 2",
     " Choir Aahs", " Voice Oohs", " Synth Voice",
     " Orchestra Hit", " Trumpet", " Trombone", " Tuba",
     " Muted Trumpet", " French Horn", " Brass Section",
     " SynthBrass 1", " SynthBrass 2", " Soprano Sax",
     " Alto Sax", " Tenor Sax", " Baritone Sax", " Oboe",
     " English Horn", " Bassoon", " Clarinet", " Piccolo",
     " Flute", " Recorder", " Pan Flute", " Blown Bottle",
     " Shakuhachi", " Whistle", " Ocarina", " Lead 1 (square)",
     " Lead 2 (sawtooth)", " Lead 3 (calliope)", " Lead 4 (chiff)",
     " Lead 5 (charang)", " Lead 6 (voice)", " Lead 7 (fifths)",
     " Lead 8 (bass + lead)", " Pad 1 (new age)", " Pad 2 (warm)",
     " Pad 3 (polysynth)", " Pad 4 (choir)", " Pad 5 (bowed)",
     " Pad 6 (metallic)", " Pad 7 (halo)", " Pad 8 (sweep)",
     " FX 1 (rain)", " FX 2 (soundtrack)", " FX 3 (crystal)",
     " FX 4 (atmosphere)", " FX 5 (brightness)", " FX 6 (goblins)",
     " FX 7 (echoes)", " FX 8 (sci-fi)", " Sitar", " Banjo",
     " Shamisen", " Koto", " Kalimba", " Bag pipe", " Fiddle",
     " Shanai", " Tinkle Bell", " Agogo", " Steel Drums",
     " Woodblock", " Taiko Drum", " Melodic Tom", " Synth Drum",
     " Reverse Cymbal", " Guitar Fret Noise", " Breath Noise",
     " Seashore", " Bird Tweet", " Telephone Ring",
     " Helicopter", " Applause", " Gunshot"};

// List of keys and offset values

char *keys[] =
  {" F/Bb/Eb", " G/C/F", " A/D/G", " C#/D/G", " B/C/C#",
   " C System", " B System"};

int keyvals[Length(keys)][ROWS] =
  {{ 3, -2, -7},  // F/Bb/Eb
   { 5,  0, -5},  // G/C/F
   { 7,  2, -3},  // A/D/G
   { 7,  2,  1},  // C#/D/G
   { 1,  0, -1},  // B/C/C#
   { 1,  0, -1},  // C System
   { 2,  0, -2}}; // B System

//      Eb  Bb   F   C   G   D   A
//     { 3, -2,  5,  0, -5,  2, -3};

int key;

GtkWidget *key_combo;

// Types

int types[Length(keys)] =
  {DIATONIC, DIATONIC, DIATONIC, DIATONIC,
   DIATONIC, CHROMATIC, CHROMATIC};

int type;

// Keyboard keys

int keyboard[ROWS][BUTTONS] =
    {{GDK_KEY_w, GDK_KEY_e, GDK_KEY_r, GDK_KEY_t, GDK_KEY_y,
      GDK_KEY_u, GDK_KEY_i, GDK_KEY_o, GDK_KEY_p, GDK_KEY_bracketleft},
     {GDK_KEY_a, GDK_KEY_s, GDK_KEY_d, GDK_KEY_f, GDK_KEY_g, GDK_KEY_h,
      GDK_KEY_j, GDK_KEY_k, GDK_KEY_l, GDK_KEY_semicolon, GDK_KEY_quoteright},
     {GDK_KEY_z, GDK_KEY_x, GDK_KEY_c, GDK_KEY_v, GDK_KEY_b,
      GDK_KEY_n, GDK_KEY_m, GDK_KEY_comma, GDK_KEY_period, GDK_KEY_slash}};

// Keyboard notes

char *notetops[Length(keys)][ROWS][BUTTONS] =
  {
    // F/Bb/Eb

    {{"G", "B", "E", "G", "B", "E", "G", "B", "E", "G"},
     {"D", "F", "B", "D", "F", "B", "D", "F", "B", "D", "F"},
     {"C", "F", "A", "C", "F", "A", "C", "F", "A", "C"}},

    // G/C/F

    {{"A", "C", "F", "A", "C", "F", "A", "C", "F", "A"},
     {"E", "G", "C", "E", "G", "C", "E", "G", "C", "E", "G"},
     {"D", "G", "B", "D", "G", "B", "D", "G", "B", "D"}},

    // A/D/G

    {{"B", "D", "G", "B", "D", "G", "B", "D", "G", "B"},
     {"F", "A", "D", "F", "A", "D", "F", "A", "D", "F", "A"},
     {"E", "A", "C", "E", "A", "C", "E", "A", "C", "E"}},

    // C#/D/G

    {{"B", "D", "G", "B", "D", "G", "B", "D", "G", "B"},
     {"F", "A", "D", "F", "A", "D", "F", "A", "D", "F", "A"},
     {"G", "C", "F", "G", "C", "F", "G", "C", "F", "G"}},

    // B/C/C#

    {{"F", "G", "C", "F", "G", "C", "F", "G", "C", "F"},
     {"E", "G", "C", "E", "G", "C", "E", "G", "C", "E", "G"},
     {"F", "B", "D", "F", "B", "D", "F", "B", "D", "F"}},

    // C System

    {{"A", "B", "D", "F", "A", "B", "D", "F", "A", "B"},
     {"G", "B", "C", "E", "G", "B", "C", "E", "G", "B", "C"},
     {"A", "C", "E", "F", "A", "C", "E", "F", "A", "C"}},

    // B System

    {{"A", "C", "E", "F", "A", "C", "E", "F", "A", "C"},
     {"G", "B", "C", "E", "G", "B", "C", "E", "G", "B", "C"},
     {"A", "B", "D", "F", "A", "B", "D", "F", "A", "B"}}
  };

// Hilites

gboolean hilites[Length(keys)][ROWS][BUTTONS] =
  {
    // F/Bb/Eb

    {},

    // G/C/F

    {},

    // A/D/G

    {},

    // C#/D/G

    {},

    // B/C/C#

    {},

    // C System

    {{TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE},
     {FALSE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE},
     {FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE}},

    // B System

    {{FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE},
     {FALSE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE},
     {TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE}}
  };

int basskeys[BASSBUTTONS][2] =
    {{GDK_KEY_F1, GDK_KEY_1},
     {GDK_KEY_F2, GDK_KEY_2},
     {GDK_KEY_F3, GDK_KEY_3},
     {GDK_KEY_F4, GDK_KEY_4},
   
     {GDK_KEY_F5, GDK_KEY_5},
     {GDK_KEY_F6, GDK_KEY_6},
     {GDK_KEY_F7, GDK_KEY_7},
     {GDK_KEY_F8, GDK_KEY_8},

     {GDK_KEY_F9, GDK_KEY_9},
     {GDK_KEY_F10, GDK_KEY_0},
     {GDK_KEY_F11, GDK_KEY_minus},
     {GDK_KEY_F12, GDK_KEY_equal}};

// Midi notes for C Diatonic, G Chromatic

int notes[TYPES][BUTTONS][DIRECTIONS] =
  {{{52, 57}, // C Diatonic
    {55, 59},
    {60, 62},
    {64, 65},
    {67, 69},
    {72, 71},
    {76, 74},
    {79, 77},
    {84, 81},
    {88, 83},
    {91, 86}},
   {{55, 55}, // G Chromatic
    {58, 58},
    {61, 61},
    {64, 64},
    {67, 67},
    {70, 70},
    {73, 73},
    {76, 76},
    {79, 79},
    {82, 82},
    {85, 85}}};

// Chords

int chords[Length(keys)][BASSBUTTONS][DIRECTIONS][2] =
  {
    // F/Bb/Eb

    {{{41, 53}, {36, 48}}, {{65, 60}, {60, 67}},  //  F/C
     {{38, 50}, {43, 55}}, {{62, 69}, {67, 62}},  //  D/G
     {{46, 58}, {41, 53}}, {{70, 65}, {65, 60}},  // Bb/F
     {{43, 55}, {36, 48}}, {{67, 62}, {60, 67}},  //  G/C
     {{39, 51}, {46, 58}}, {{63, 70}, {70, 65}},  // Eb/Bb
     {{44, 56}, {44, 56}}, {{68, 63}, {68, 63}}}, // Ab/Ab

    // G/C/F

    {{{43, 55}, {38, 50}}, {{67, 62}, {62, 69}},  //  G/D
     {{40, 52}, {45, 57}}, {{64, 71}, {69, 64}},  //  E/A
     {{36, 48}, {43, 55}}, {{60, 67}, {67, 62}},  //  C/G
     {{45, 57}, {38, 50}}, {{69, 64}, {62, 69}},  //  A/D
     {{41, 53}, {36, 48}}, {{65, 60}, {60, 67}},  //  F/C
     {{46, 58}, {46, 58}}, {{70, 65}, {70, 65}}}, // Bb/Bb

    // A/D/G

    {{{45, 57}, {40, 52}}, {{69, 64}, {64, 71}},  //  A/E
     {{42, 54}, {47, 59}}, {{66, 61}, {71, 66}},  // F#/B
     {{38, 50}, {45, 57}}, {{62, 69}, {69, 64}},  //  D/A
     {{47, 59}, {40, 52}}, {{71, 66}, {64, 71}},  //  B/E
     {{43, 55}, {38, 50}}, {{67, 62}, {62, 69}},  //  G/D
     {{36, 48}, {36, 48}}, {{60, 67}, {60, 67}}}, //  C/C

    // C#/D/G

    {{{45, 57}, {40, 52}}, {{69, 64}, {64, 71}},  //  A/E
     {{42, 54}, {47, 59}}, {{66, 61}, {71, 66}},  // F#/B
     {{38, 50}, {45, 57}}, {{62, 69}, {69, 64}},  //  D/A
     {{47, 59}, {40, 52}}, {{71, 66}, {64, 71}},  //  B/E
     {{43, 55}, {38, 50}}, {{67, 62}, {62, 69}},  //  G/D
     {{36, 48}, {36, 48}}, {{60, 67}, {60, 67}}}, //  C/C

    // B/C/C#

    {{{42, 54}, {42, 54}}, {{47, 59}, {47, 59}},  // F#/B
     {{40, 52}, {40, 52}}, {{45, 57}, {45, 57}},  //  E/A
     {{38, 50}, {38, 50}}, {{43, 55}, {43, 55}},  //  D/G
     {{36, 48}, {36, 48}}, {{41, 53}, {41, 53}},  //  C/F
     {{46, 58}, {46, 58}}, {{39, 51}, {39, 51}},  // Bb/Eb
     {{44, 56}, {44, 56}}, {{37, 49}, {37, 49}}}, // Ab/Db

    // C System

    {{{42, 54}, {42, 54}}, {{47, 59}, {47, 59}},  // F#/B
     {{40, 52}, {40, 52}}, {{45, 57}, {45, 57}},  //  E/A
     {{38, 50}, {38, 50}}, {{43, 55}, {43, 55}},  //  D/G
     {{36, 48}, {36, 48}}, {{41, 53}, {41, 53}},  //  C/F
     {{46, 58}, {46, 58}}, {{39, 51}, {39, 51}},  // Bb/Eb
     {{44, 56}, {44, 56}}, {{37, 49}, {37, 49}}}, // Ab/Db

    // B System

    {{{42, 54}, {42, 54}}, {{47, 59}, {47, 59}},  // F#/B
     {{40, 52}, {40, 52}}, {{45, 57}, {45, 57}},  //  E/A
     {{38, 50}, {38, 50}}, {{43, 55}, {43, 55}},  //  D/G
     {{36, 48}, {36, 48}}, {{41, 53}, {41, 53}},  //  C/F
     {{46, 58}, {46, 58}}, {{39, 51}, {39, 51}},  // Bb/Eb
     {{44, 56}, {44, 56}}, {{37, 49}, {37, 49}}}, // Ab/Db
  };

// Buttons

gboolean buttons[ROWS][BUTTONS];
gboolean bassbuttons[BASSBUTTONS];

gboolean bellows;

// Display widgets

GtkWidget *display[ROWS][BUTTONS];
GtkWidget *bassdisplay[BASSBUTTONS];
GtkWidget *spacebar;

// Reverse buttons

gboolean reverse;

// Show notes

gboolean shownotes;

// Volume value

int volume = MAXVOL;

// Function declarations
int instrument_changed(GtkWidget*, MIDI_HANDLE);
int key_changed(GtkWidget*, GtkWindow*);
int reverse_changed(GtkWidget*, GtkWindow*);
int notes_changed(GtkWidget*, GtkWindow*);
int volume_changed(GtkWidget*, gdouble, GtkWindow*);
int quit_clicked(GtkWidget*, GtkWindow*);
int button_clicked(GtkWidget*, gboolean*);
int focus_out(GtkWidget*, GdkEventFocus*, MIDI_HANDLE);
int focus_in(GtkWidget*, GdkEventFocus*, Display*);
int key_press(GtkWidget*, GdkEventKey*, MIDI_HANDLE);
int key_release(GtkWidget*, GdkEventKey*, MIDI_HANDLE);
int change_display(GtkWidget*, GtkWindow*);
int send_change(MIDI_HANDLE, int, int);
int send_noteon(MIDI_HANDLE, int, int, int);
int send_noteoff(MIDI_HANDLE, int, int);
// Main function

int main(int argc, char *argv[])
{
    // Widgets

    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *wbox;
    GtkWidget *hbox;
    GtkWidget *bbox;
    GtkWidget *frame;
    GtkWidget *label;
    GtkWidget *combo;
    GtkWidget *quit;
    GtkWidget *notes;
    GtkWidget *volume;
    GtkWidget *reverse;
    GtkWidget *separator;

    // Initialise GTK

    gtk_init(&argc, &argv);

    // Midi

    MIDI_HANDLE handle;

    if (MIDI_Init() < 0)
	return 1;

    if (MIDI_OpenStream(&handle) < 0)
    {
    	MIDI_Shutdown();
    	return 1;
    }

    // Create main window

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Accordion");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // W box, this contains the fake status bar and the rest of the
    // display

    wbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), wbox);

    // Label, this label and separator are a fake status bar that can
    // have small text

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label),
		    "<small>Press the function keys F1-F12 as"
		    " accordion buttons and the space bar"
		    " as the bellows.</small>");
    gtk_box_pack_end(GTK_BOX(wbox), label, FALSE, FALSE, 0);

    // Separator

    separator = gtk_hseparator_new();
    gtk_box_pack_end(GTK_BOX(wbox), separator, FALSE, FALSE, 0);

    // V box, this contains three frames

    vbox = gtk_vbox_new(FALSE, MARGIN);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), MARGIN);
    gtk_box_pack_start(GTK_BOX(wbox), vbox, FALSE, FALSE, 0);

    // Frame, this contains all the tool widgets

    frame = gtk_frame_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

    // W Box

    wbox = gtk_vbox_new(FALSE, MARGIN);
    gtk_container_set_border_width(GTK_CONTAINER(wbox), MARGIN);
    gtk_container_add(GTK_CONTAINER(frame), wbox);

    // H box

    hbox = gtk_hbox_new(FALSE, MARGIN);
    gtk_box_pack_start(GTK_BOX(wbox), hbox, FALSE, FALSE, 0);

    // Instrument label

    label = gtk_label_new("Instrument:");
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

    // Instrument combo box

    combo = gtk_combo_box_text_new();

    for (int i = 0; i < Length(instruments); i++)
    {
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), instruments[i]);
	if (strcmp(instruments[i], " Accordion") == 0)
	    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), i);
    }

    gtk_box_pack_start(GTK_BOX(hbox), combo, FALSE, FALSE, 0);

    // Instrument changed

    g_signal_connect(G_OBJECT(combo), "changed",
		     G_CALLBACK(instrument_changed), handle);

    instrument_changed(combo, handle);

    // Key combo box

    combo = gtk_combo_box_text_new();
    for (int i = 0; i < Length(keys); i++)
    {
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), keys[i]);
	if (strcmp(keys[i], " A/D/G") == 0)
	    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), i);
    }
    gtk_box_pack_end(GTK_BOX(hbox), combo, FALSE, FALSE, 1);
    key_combo = combo;

    // Key changed

    g_signal_connect(G_OBJECT(combo), "changed",
		     G_CALLBACK(key_changed), window);

    key_changed(combo, GTK_WINDOW(window));

    // Key label

    label = gtk_label_new("Key:");
    gtk_box_pack_end(GTK_BOX(hbox), label, FALSE, FALSE, 1);

    // Reverse check button

    reverse = gtk_toggle_button_new_with_label("  Reverse  ");
    gtk_box_pack_end(GTK_BOX(hbox), reverse, FALSE, FALSE, 0);

    // Reverse toggled

    g_signal_connect(G_OBJECT(reverse), "toggled",
		     G_CALLBACK(reverse_changed), window);

    // H box

    hbox = gtk_hbox_new(FALSE, MARGIN);
    // gtk_widget_set_halign(GTK_WIDGET(hbox), GTK_ALIGN_FILL);
    gtk_box_pack_start(GTK_BOX(wbox), hbox, FALSE, FALSE, 0);

    // Volume label

    label = gtk_label_new("Volume:");
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

    // Volume scale

    volume = gtk_hscale_new_with_range(0.0, 1.0, 0.1);
    gtk_range_set_value(GTK_RANGE(volume), 1.0);
    gtk_scale_set_draw_value (GTK_SCALE(volume), FALSE);
    gtk_widget_set_size_request(volume, 240, 24);
    gtk_box_pack_start(GTK_BOX(hbox), volume, FALSE, FALSE, 0);

    // Volume changed

    g_signal_connect(G_OBJECT(volume), "value-changed",
		     G_CALLBACK(volume_changed), window);

    // Notes check button

    notes = gtk_toggle_button_new_with_label("  Notes  ");
    gtk_box_pack_start(GTK_BOX(hbox), notes, FALSE, FALSE, 0);

    // Notes toggled

    g_signal_connect(G_OBJECT(notes), "toggled",
		     G_CALLBACK(notes_changed), window);
    // Quit button

    quit = gtk_button_new_with_label("  Quit  ");
    gtk_box_pack_end(GTK_BOX(hbox), quit, FALSE, FALSE, 0);

    // Quit clicked

    g_signal_connect(G_OBJECT(quit), "clicked",
		     G_CALLBACK(quit_clicked), window);

    // Frame

    frame = gtk_frame_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

    // Label

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label),
			 "\n<big><b>Accordion</b>\n\n"
			 "Play Accordion on your keyboard</big>\n");

    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_container_add(GTK_CONTAINER(frame), label);

    // Frame

    frame = gtk_frame_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

    // Button box

    bbox = gtk_vbox_new(FALSE, MARGIN);
    gtk_container_set_border_width(GTK_CONTAINER(bbox), MARGIN);
    gtk_container_add(GTK_CONTAINER(frame), bbox);

    // Row of buttons

    hbox = gtk_hbutton_box_new();
    gtk_button_box_set_spacing(GTK_BUTTON_BOX(hbox), MARGIN);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(hbox), GTK_BUTTONBOX_CENTER);
    gtk_button_box_set_child_size(GTK_BUTTON_BOX(hbox), SIZE, SIZE);
    gtk_box_pack_start(GTK_BOX(bbox), hbox, FALSE, FALSE, 0);

    // Bass buttons

    for (int i = 0; i < Length(bassdisplay); i++)
    {
    	bassdisplay[i] = gtk_toggle_button_new();
    	gtk_container_add(GTK_CONTAINER(hbox), bassdisplay[i]);

    	// Button callback

    	g_signal_connect(G_OBJECT(bassdisplay[i]), "clicked",
    			 G_CALLBACK(button_clicked),
    			 &bassbuttons[i]);
    }

    // Buttons

    for (int i = 0; i < Length(display); i++)
    {
	// Row of buttons

	hbox = gtk_hbutton_box_new();
	gtk_button_box_set_spacing(GTK_BUTTON_BOX(hbox), MARGIN);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(hbox), GTK_BUTTONBOX_CENTER);
	gtk_button_box_set_child_size(GTK_BUTTON_BOX(hbox), SIZE, SIZE);
	gtk_box_pack_start(GTK_BOX(bbox), hbox, FALSE, FALSE, 0);

	if (i == 2)
	{
	    // Spacebar

	    spacebar = gtk_toggle_button_new();
	    gtk_container_add(GTK_CONTAINER(hbox), spacebar);

	    // Button callback

	    g_signal_connect(G_OBJECT(spacebar), "clicked",
			     G_CALLBACK(button_clicked), &bellows);
	}

	int k = (i == 1)? Length(display[i]): Length(display[i]) - 1;
    	for (int j = 0; j < k; j++)
    	{
    	    display[i][j] = gtk_toggle_button_new();
    	    gtk_container_add(GTK_CONTAINER(hbox), display[i][j]);

    	    // Button callback

    	    g_signal_connect(G_OBJECT(display[i][j]), "clicked",
    			     G_CALLBACK(button_clicked), &buttons[i][j]);
    	}

	if (i == 2)
	{
	    // Dummy button to keep things symmetrical

	    GtkWidget *dummy = gtk_hbox_new(FALSE, 0);
	    gtk_container_add(GTK_CONTAINER(hbox), dummy);
	}
    }

    // Get X display

    GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(window));
    GdkDisplay *gd = gdk_screen_get_display(screen);
    Display *disp = gdk_x11_display_get_xdisplay(gd);

    // Focus out callback

    g_signal_connect(G_OBJECT(window), "focus-out-event",
		     G_CALLBACK(focus_out), handle);

    // Focus in callback

    g_signal_connect(G_OBJECT(window), "focus-in-event",
		     G_CALLBACK(focus_out), disp);

    // Key pressed callback

    g_signal_connect(G_OBJECT(window), "key-press-event",
		     G_CALLBACK(key_press), handle);

    // Key released callback

    g_signal_connect(G_OBJECT(window), "key-release-event",
		     G_CALLBACK(key_release), handle);

    // Destroy window callback

    g_signal_connect(window, "destroy",
		     G_CALLBACK(gtk_main_quit), NULL);

    // Show the window

    gtk_widget_show_all(window);

    // Turn the auto repeat off

    XAutoRepeatOff(disp);

    // Set focus to the window

    gtk_window_set_focus(GTK_WINDOW(window), NULL);

    // Interact with user

    gtk_main();

    // Clean up

    MIDI_CloseStream(handle);
    MIDI_Shutdown();

    // Set auto repeat back

    XAutoRepeatOn(disp);

    // Make sure it gets there

    XSync(disp, FALSE);

    // Exit

    return 0;
}

// Quit callback

int quit_clicked(GtkWidget *widget, GtkWindow *window)
{
    // Create a message dialog
#ifdef QUERY_QUIT
    GtkWidget *dialog =
	gtk_message_dialog_new(GTK_WINDOW(window),
			       GTK_DIALOG_MODAL,
			       GTK_MESSAGE_QUESTION,
			       GTK_BUTTONS_OK_CANCEL,
			       "Quit");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
					     "Really?");
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (result == GTK_RESPONSE_OK)
#endif
	gtk_main_quit();
}

// Instrument changed

int instrument_changed(GtkWidget *widget, MIDI_HANDLE handle)
{
    int inst = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));

    for (int i = 0; i <= ROWS; i++)
	send_change(handle, i, inst);

    GtkWindow *window = (GtkWindow *)gtk_widget_get_window(widget);
    gtk_window_set_focus(GTK_WINDOW(window), NULL);
}

// Key changed

int key_changed(GtkWidget *widget, GtkWindow *window)
{
    key = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
    type = types[key];
    change_display(widget, window);

    gtk_window_set_focus(GTK_WINDOW(window), NULL);
}

// Reverse changed

int reverse_changed(GtkWidget *widget, GtkWindow *window)
{
    reverse = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
    change_display(widget, window);
    gtk_window_set_focus(GTK_WINDOW(window), NULL);
}

// Notes changed

int notes_changed(GtkWidget *widget, GtkWindow *window)
{
    shownotes = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
    change_display(widget, window);

    gtk_window_set_focus(GTK_WINDOW(window), NULL);
}

// Volume changed

int volume_changed(GtkWidget *widget, gdouble value, GtkWindow *window)
{
    volume = value * MAXVOL;
    gtk_window_set_focus(GTK_WINDOW(window), NULL);
}

// Focus out event

int focus_out(GtkWidget *widget, GdkEventFocus *event, MIDI_HANDLE handle)
{
    // Stop all notes, and reset buttons

    for (int i = 0; i < Length(bassbuttons); i++)
    {
	if (bassbuttons[i])
	{
	    int note = chords[key][i][bellows][0];
	    send_noteoff(handle, ROWS, note);
	    note = chords[key][i][bellows][1];
	    send_noteoff(handle, ROWS, note);

	    bassbuttons[i] = FALSE;
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bassdisplay[i]),
					 FALSE);
	}
    }

    for (int i = 0; i < Length(buttons); i++)
    {
	for (int j = 0; j < Length(buttons[i]); j++)
	{
	    if (buttons[i][j])
	    {
		buttons[i][j] = FALSE;

		int k;

		switch (i)
		{
		case 0:
		    k = (reverse)? Length(buttons[i]) - j - 2: j;
		    break;

		case 1:
		    k = (reverse)? Length(buttons[i]) - j - 1: j;
		    break;

		case 2:
		    k = (reverse)? Length(buttons[i]) - j - 1: j + 1;
		    break;
		}

		if ((type == CHROMATIC) && (hilites[key][i][j] == TRUE))
		    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(display[i][j]),
						 TRUE);

		else
		    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(display[i][j]),
						 FALSE);

		int note = notes[type][k][bellows] + keyvals[key][i];
		send_noteoff(handle, i, note);
	    }
	}
    }

    if (bellows)
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(spacebar),
				     FALSE);
    // Get X display

    GdkScreen *screen = gtk_widget_get_screen(GTK_WIDGET(spacebar));
    GdkDisplay *gd = gdk_screen_get_display(screen);
    Display *disp = gdk_x11_display_get_xdisplay(gd);

    // Turn auto repeat back on

    XAutoRepeatOn(disp);
}

// Focus in event

int focus_in(GtkWidget *widget, GdkEventFocus *event, Display *disp)
{

    // Turn auto repeat off

    XAutoRepeatOff(disp);
}

// Key press event

int key_press(GtkWidget *window, GdkEventKey *event, MIDI_HANDLE handle)
{
    switch (event->keyval)
    {
    	// Space bar

    case GDK_KEY_space:
    case GDK_KEY_Escape:
    case GDK_KEY_Shift_L:
    case GDK_KEY_Shift_R:
    case GDK_KEY_Control_L:
    case GDK_KEY_Control_R:
    case GDK_KEY_Alt_L:
    case GDK_KEY_Alt_R:

    	if (!bellows)
    	{
    	    // If there's a change of direction, stop and play all the
    	    // notes for buttons that are pressed

    	    bellows = TRUE;
    	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(spacebar),
    					 TRUE);

	    // Check buttons

    	    for (int i = 0; i < Length(buttons); i++)
    	    {
    		for (int j = 0; j < Length(buttons[i]); j++)
    		{
    		    if (buttons[i][j])
    		    {
			int k;

			switch (i)
			{
			case 0:
			    k = (reverse)? Length(buttons[i]) - j - 2: j;
			    break;

			case 1:
			    k = (reverse)? Length(buttons[i]) - j - 1: j;
			    break;

			case 2:
			    k = (reverse)? Length(buttons[i]) - j - 1: j + 1;
			    break;
			}

			// Play note

    			int note = notes[type][k][!bellows] + keyvals[key][i];
			send_noteoff(handle, i, note);

    			note = notes[type][k][bellows] + keyvals[key][i];
			send_noteon(handle, i, note, volume);
    		    }
    		}
    	    }

    	    for (int i = 0; i < Length(bassbuttons); i++)
    	    {
    		if (bassbuttons[i])
    		{
		    int k = (reverse)? Length(basskeys) - i - 1: i;

		    // Play chord

    		    int note = chords[key][k][!bellows][0];
		    send_noteoff(handle, ROWS, note);
    		    note = chords[key][k][!bellows][1];
		    send_noteoff(handle, ROWS, note);

    		    note = chords[key][k][bellows][0];
		    send_noteon(handle, ROWS, note, volume);
    		    note = chords[key][k][bellows][1];
		    send_noteon(handle, ROWS, note, volume);
    		}
    	    }
    	}
    	return 0;

    	// Check keyboard

    default:

	// Look up the key code in the keyboard table

	for (int i = 0; i < Length(keyboard); i++)
	{
	    for (int j = 0; j < Length(keyboard[i]); j++)
	    {
		if (event->keyval == keyboard[i][j] && !buttons[i][j])
		{
		    buttons[i][j] = TRUE;

		    // Play note

		    int k;

		    switch (i)
		    {
		    case 0:
			k = (reverse)? Length(buttons[i]) - j - 2: j;
			break;

		    case 1:
			k = (reverse)? Length(buttons[i]) - j - 1: j;
			break;

		    case 2:
			k = (reverse)? Length(buttons[i]) - j - 1: j + 1;
			break;
		    }

		    if ((type == CHROMATIC) && (hilites[key][i][k] == TRUE))
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(display[i][j]),
						     FALSE);

		    else
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(display[i][j]),
						     TRUE);

		    int note = notes[type][k][bellows] + keyvals[key][i];
		    send_noteon(handle, i, note, volume);

		    return 0;
		}
	    }
	}

    	for (int i = 0; i < Length(basskeys); i++)
    	{
	    if (((event->keyval == basskeys[i][0]) ||
		 (event->keyval == basskeys[i][1])) && !bassbuttons[i])
	    {
		bassbuttons[i] = TRUE;
   		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bassdisplay[i]),
    					     TRUE);

		int k = reverse? BASSBUTTONS - i - 1: i;

		// Play chord

    		int note = chords[key][k][bellows][0];
		send_noteon(handle, ROWS, note, volume);
		note = chords[key][k][bellows][1];
		send_noteon(handle, ROWS, note, volume);

		return 0;
    	    }
    	}
    }

    gtk_window_set_focus(GTK_WINDOW(window), NULL);
}

// Key release event

int key_release(GtkWidget *window, GdkEventKey *event, MIDI_HANDLE handle)
{
    char *s = "";

    switch (event->keyval)
    {
    case GDK_KEY_space:
    case GDK_KEY_Escape:
    case GDK_KEY_Shift_L:
    case GDK_KEY_Shift_R:
    case GDK_KEY_Control_L:
    case GDK_KEY_Control_R:
    case GDK_KEY_Alt_L:
    case GDK_KEY_Alt_R:

    	if (bellows)
    	{
    	    // If there's a change of direction, stop and play all the
    	    // notes for buttons that are pressed

    	    bellows = FALSE;
    	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(spacebar),
    					 FALSE);
	    // Check buttons

	    for (int i = 0; i < Length(buttons); i++)
	    {
		for (int j = 0; j < Length(buttons[i]); j++)
		{
		    if (buttons[i][j])
		    {
			// Play note

			int k;

			switch (i)
			{
			case 0:
			    k = (reverse)? Length(buttons[i]) - j - 2: j;
			    break;

			case 1:
			    k = (reverse)? Length(buttons[i]) - j - 1: j;
			    break;

			case 2:
			    k = (reverse)? Length(buttons[i]) - j - 1: j + 1;
			    break;
			}

			// Play note

			int note = notes[type][k][!bellows] + keyvals[key][i];
			send_noteoff(handle, i, note);

			note = notes[type][k][bellows] + keyvals[key][i];
			send_noteon(handle, i, note, volume);
		    }
		}
	    }

    	    for (int i = 0; i < Length(bassbuttons); i++)
    	    {
    		if (bassbuttons[i])
    		{
		    int k = (reverse)? Length(basskeys) - i - 1: i;

		    // Play chord

    		    int note = chords[key][k][!bellows][0];
		    send_noteoff(handle, ROWS, note);
    		    note = chords[key][k][!bellows][1];
		    send_noteoff(handle, ROWS, note);

    		    note = chords[key][k][bellows][0];
		    send_noteon(handle, ROWS, note, volume);
    		    note = chords[key][k][bellows][1];
		    send_noteon(handle, ROWS, note, volume);
    		}
    	    }
    	}
    	return 0;

    default:

	// Look up the key code in the keyboard table

	for (int i = 0; i < Length(keyboard); i++)
	{
	    for (int j = 0; j < Length(keyboard[i]); j++)
	    {
		if (event->keyval == keyboard[i][j] && buttons[i][j])
		{
		    buttons[i][j] = FALSE;

		    // Play note

		    int k;

		    switch (i)
		    {
		    case 0:
			k = (reverse)? Length(buttons[i]) - j - 2: j;
			break;

		    case 1:
			k = (reverse)? Length(buttons[i]) - j - 1: j;
			break;

		    case 2:
			k = (reverse)? Length(buttons[i]) - j - 1: j + 1;
			break;
		    }

			if ((type == CHROMATIC) && (hilites[key][i][k] == TRUE))
			    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(display[i][j]),
							 TRUE);

			else
			    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(display[i][j]),
							 FALSE);

		    int note = notes[type][k][bellows] + keyvals[key][i];
		    send_noteoff(handle, i, note);

		    return 0;
		}
	    }
	}

    	for (int i = 0; i < Length(basskeys); i++)
    	{
	    if (((event->keyval == basskeys[i][0]) ||
		 (event->keyval == basskeys[i][1])) && bassbuttons[i])
	    {
		bassbuttons[i] = FALSE;
   		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bassdisplay[i]),
    					     FALSE);

		int k = reverse? BASSBUTTONS - i - 1: i;

		// Play chord

    		int note = chords[key][k][bellows][0];
		send_noteoff(handle, ROWS, note);
		note = chords[key][k][bellows][1];
		send_noteoff(handle, ROWS, note);

		return 0;
    	    }
    	}
    }
}

// Change display

int change_display(GtkWidget *widget, GtkWindow *window)
{
    // Change display

    for (int i = 0; i < Length(notetops[0]); i++)
    {
	for (int j = 0; j < Length(notetops[0][0]); j++)
	{
	    int k;

	    switch (i)
	    {
	    case 0:
		k = (reverse)? Length(notetops[0][i]) - j - 2: j;
		break;

	    case 1:
		k = (reverse)? Length(notetops[0][i]) - j - 1: j;
		break;

	    case 2:
		k = (reverse)? Length(notetops[0][i]) - j - 2: j;
		break;
	    }

	    // Show notes

	    if (shownotes)
		gtk_button_set_label(GTK_BUTTON(display[i][j]),
				     notetops[key][i][k]);

	    else
		gtk_button_set_label(GTK_BUTTON(display[i][j]), "");

	    // Show hilites for chromatic

	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(display[i][j]),
					 hilites[key][i][k]);
	}
    }
}

// Change

int send_change(MIDI_HANDLE handle, int channel, int inst)
{
    char msg[2];

    msg[0] = CHANGE + channel;
    msg[1] = inst;

    return MIDI_WriteStream(handle, msg, sizeof(msg));
}

// Note on

int send_noteon(MIDI_HANDLE handle, int channel, int note, int volume)
{
    char msg[3];

    msg[0] = NOTEON + channel;
    msg[1] = note;
    msg[2] = volume;

    return MIDI_WriteStream(handle, msg, sizeof(msg));
}

// Note on

int send_noteoff(MIDI_HANDLE handle, int channel, int note)
{
    char msg[3];

    msg[0] = NOTEOFF + channel;
    msg[1] = note;
    msg[2] = 0;

    return MIDI_WriteStream(handle, msg, sizeof(msg));
}

// Reset the toggle button if clicked

int button_clicked(GtkWidget *button, gboolean *pressed)
{
//     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button),
//     				 *pressed);
}
