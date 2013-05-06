////////////////////////////////////////////////////////////////////////////////
//
//  Accordion - An Android Accordion written in Java.
//
//  Copyright (C) 2013	Bill Farmer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Bill Farmer	 william j farmer [at] yahoo [dot] co [dot] uk.
//
///////////////////////////////////////////////////////////////////////////////

package org.billthefarmer.accordion;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

public class MainActivity extends Activity
    implements OnTouchListener
{

    // Button ids

    static final int buttons[][] =
    {{R.id.button_1, R.id.button_2,
      R.id.button_3, R.id.button_4,
      R.id.button_5, R.id.button_6,
      R.id.button_7, R.id.button_8,
      R.id.button_9, R.id.button_10,
      0},
     {R.id.button_11, R.id.button_12,
      R.id.button_13, R.id.button_14,
      R.id.button_15, R.id.button_16,
      R.id.button_17, R.id.button_18,
      R.id.button_19, R.id.button_20,
      R.id.button_21},
     {R.id.button_22, R.id.button_23,
      R.id.button_24, R.id.button_25,
      R.id.button_26, R.id.button_27,
      R.id.button_28, R.id.button_29,
      R.id.button_30, R.id.button_31,
      0}};

    // Bass button ids

    static final int basses[] =
    {R.id.bass_1, R.id.bass_2,
     R.id.bass_3, R.id.bass_4,
     R.id.bass_5, R.id.bass_6,
     R.id.bass_7, R.id.bass_8,
     R.id.bass_9, R.id.bass_10,
     R.id.bass_11, R.id.bass_12};

    // List of keys and offset values

    static final String keys[] =
    {" F/Bb/Eb", " G/C/F", " A/D/G", " C#/D/G", " B/C/C#",
     " C System", " B System"};

    static final int keyvals[][] =
    {{ 3, -2, -7},  // F/Bb/Eb
     { 5,  0, -5},  // G/C/F
     { 7,  2, -3},  // A/D/G
     { 7,  2,  1},  // C#/D/G
     { 1,  0, -1},  // B/C/C#
     { 1,  0, -1},  // C System
     { 2,  0, -2}}; // B System

    //      Eb  Bb   F   C   G   D   A
    //     { 3, -2,  5,  0, -5,  2, -3};

    // Midi notes for C Diatonic, G Chromatic

    static final byte notes[][][] =
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

    static final byte chords[][][][] =
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

    // Keyboard notes

    static final String notetops[][][] =
    {
	// F/Bb/Eb

	{{"G", "Bb", "Eb", "G", "Bb", "Eb", "G", "Bb", "Eb", "G"},
	 {"D", "F", "Bb", "D", "F", "Bb", "D", "F", "Bb", "D", "F"},
	 {"C", "F", "A", "C", "F", "A", "C", "F", "A", "C"}},

	// G/C/F

	{{"A", "C", "F", "A", "C", "F", "A", "C", "F", "A"},
	 {"E", "G", "C", "E", "G", "C", "E", "G", "C", "E", "G"},
	 {"D", "G", "B", "D", "G", "B", "D", "G", "B", "D"}},

	// A/D/G

	{{"B", "D", "G", "B", "D", "G", "B", "D", "G", "B"},
	 {"F#", "A", "D", "F#", "A", "D", "F#", "A", "D", "F#", "A"},
	 {"E", "A", "C#", "E", "A", "C#", "E", "A", "C#", "E"}},

	// C#/D/G

	{{"B", "D", "G", "B", "D", "G", "B", "D", "G", "B"},
	 {"F#", "A", "D", "F#", "A", "D", "F#", "A", "D", "F#", "A"},
	 {"G#", "C#", "F", "G#", "C#", "F", "G#", "C#", "F", "G#"}},

	// B/C/C#

	{{"F", "G#", "C#", "F", "G#", "C#", "F", "G#", "C#", "F"},
	 {"E", "G", "C", "E", "G", "C", "E", "G", "C", "E", "G"},
	 {"F#", "B", "D#", "F#", "B", "D#", "F#", "B", "D#", "F#"}},

	// C System

	{{"Ab", "B", "D", "F", "Ab", "B", "D", "F", "Ab", "B"},
	 {"G", "Bb", "C#", "E", "G", "Bb", "C#", "E", "G", "Bb", "C#"},
	 {"A", "C", "Eb", "F#", "A", "C", "Eb", "F#", "A", "C"}},

	// B System

	{{"A", "C", "Eb", "F#", "A", "C", "Eb", "F#", "A", "C"},
	 {"G", "Bb", "C#", "E", "G", "Bb", "C#", "E", "G", "Bb", "C#"},
	 {"Ab", "B", "D", "F", "Ab", "B", "D", "F", "Ab", "B"}}
    };

    // Hilites

    static final boolean hilites[][][] =
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

	{{true, false, false, false, true, false, false, false, true, false},
	 {false, true, true, false, false, true, true, false, false, true, true},
	 {false, false, true, true, false, false, true, true, false, false}},

	// b system

	{{false, false, true, true, false, false, true, true, false, false},
	 {false, true, true, false, false, true, true, false, false, true, true},
	 {true, false, false, false, true, false, false, false, true, false}}
    };

    // Button states

    boolean buttonStates[][] =
    {{false, false, false, false, false, false,
      false, false, false, false, false},
     {false, false, false, false, false, false,
      false, false, false, false, false},
     {false, false, false, false, false, false,
      false, false, false, false, false}};

    boolean bassStates[] =
    {false, false, false, false, false, false,
     false, false, false, false, false, false};

    boolean bellowsState = false;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
	super.onCreate(savedInstanceState);
	setContentView(R.layout.activity_main);
	setListener();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
	// Inflate the menu; this adds items to the action bar if it
	// is present.
	getMenuInflater().inflate(R.menu.main, menu);
	return true;
    }

    // On options item

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
	// Get id

	int id = item.getItemId();
	switch (id)
	{
	    // Settings

	case R.id.settings:
	Intent intent = new Intent(this, SettingsActivity.class);
	startActivity(intent);

	return true;

	default:
	    return false;
	}
    }

    @Override
    public boolean onTouch(View v, MotionEvent event)
    {
	int action = event.getAction();
	int id = v.getId();

	switch (action)
	{
	case MotionEvent.ACTION_DOWN:
	    switch (id)
	    {
	    case R.id.bellows:
		return onBellowsDown(v, event);

	    default:
		return onButtonDown(v, event);
	    }

	case MotionEvent.ACTION_UP:
	    switch (id)
	    {
	    case R.id.bellows:
		return onBellowsUp(v, event);

	    default:
		return onButtonUp(v, event);
	    }

	default:
	    return false;
	}
    }

    private boolean onBellowsDown(View v, MotionEvent event)
    {
	return false;
    }

    private boolean onBellowsUp(View v, MotionEvent event)
    {
	return false;
    }

    private boolean onButtonDown(View v, MotionEvent event)
    {
	return false;
    }

    private boolean onButtonUp(View v, MotionEvent event)
    {
	return false;
    }

    private void setListener()
    {
	View v;

	for (int i = 0; i < buttons.length; i++)
	{
	    for (int j = 0; j < buttons[i].length; j++)
	    {
		v = findViewById(buttons[i][j]);
		if (v != null)
		    v.setOnTouchListener(this);
	    }
	}

	for (int i = 0; i < basses.length; i++)
	{
	    v = findViewById(basses[i]);
	    if (v != null)
		v.setOnTouchListener(this);
	}

	v = findViewById(R.id.bellows);
	if (v != null)
	    v.setOnTouchListener(this);
    }
}
