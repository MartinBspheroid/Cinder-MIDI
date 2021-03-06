/*
 Copyright (c) 2013, Bruce Lane - All rights reserved.
 This code is intended for use with the Cinder C++ library: http://libcinder.org

 This file is part of Cinder-MIDI.

 Cinder-MIDI is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Cinder-MIDI is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Cinder-MIDI.  If not, see <http://www.gnu.org/licenses/>.
*/

// don't forget to add winmm.lib to the linker

#include "cinder/app/AppBasic.h"
#include <list>
using namespace ci;
using namespace ci::app;
using namespace std;

#include "MidiIn.h"
#include "MidiMessage.h"
#include "MidiConstants.h"

#define SLIDER_NOTE 1


class _TBOX_PREFIX_App : public AppBasic {
 public:
	void prepareSettings(Settings* settings);
	void setup();
	void update();
	void draw();
	
	void processMidiMessage(midi::Message* message);
	
	midi::Input mMidiIn;
	
	float sliderValue;
};

void _TBOX_PREFIX_App::prepareSettings(Settings* settings){
	settings->setFrameRate(50.0f);
	settings->setWindowSize(640, 480);
}

void _TBOX_PREFIX_App::setup(){
	
	if (mMidiIn.getNumPorts() > 0){
		mMidiIn.listPorts();
		mMidiIn.openPort(0);
		console() << "Opening MIDI port 0" << std::endl;
	}else {
		console() << "No MIDI Ports found!!!!" << std::endl;
	}
}

void _TBOX_PREFIX_App::update(){
	while (mMidiIn.hasWaitingMessages()) {
		midi::Message message;
		mMidiIn.getNextMessage(&message);
		
		processMidiMessage(&message);
		
	}
}

void _TBOX_PREFIX_App::draw(){
	gl::clear(Color(0,0,0), true);
	gl::color(Color(1, 1, 1));
	gl::drawSolidRect(Rectf(Vec2f(0, 0), Vec2f(sliderValue * getWindowWidth(), getWindowHeight())));
}

void _TBOX_PREFIX_App::processMidiMessage(midi::Message* message){
	console() << "midi port: " << message->port << " ch: " << message->channel << " status: " << message->status;
	console() << " byteOne: " << message->byteOne << " byteTwo: " << message->byteTwo << std::endl;
	
	switch (message->status) {
		case MIDI_CONTROL_CHANGE:
			if (message->byteOne == SLIDER_NOTE){
				sliderValue = message->byteTwo / 127.0f;
			}
			break;
	}
}



// This line tells Cinder to actually create the application
CINDER_APP_BASIC( _TBOX_PREFIX_App, RendererGl )
