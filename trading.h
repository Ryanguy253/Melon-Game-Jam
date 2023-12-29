#pragma once
#ifndef TRADING_H
#define TRADING_H

#include <string>
#include <iostream>
#include<stdio.h>
#include "raylib.h"
#include"shop.h"
using namespace std;

bool npc_said_first_sentence = false;
bool npc_said_second_sentence = false;

int item_id;



const char* getDialog(int dialogue) {
	switch(dialogue) {
	case 1:
		//Charm of wisdom (+charisma) 
		return "Ghost : I crave the wisdom of the ancients, an Echoing Echo Amulet.";
		break;
	case 2:
		//aurora Veil
		return "Ghost : I desire the protection of an Aurora Veil.";
		break;
		// soulweave locket
	case 3:
		return "Ghost : I seek a locket, a Soulweave perhaps.";
		break;
		// timekeeper's hourglass
	case 4:
		return "Ghost : I covet a Transcendent Timekeeper Hourglass.";
		break;
	case 5:
		//Dreamweaver Dreamcatcher
		return "Ghost : I yearn for a Dreamcatcher to weave dreams of futures.";
		break;
	case 6:
		//Vitalis Essence Pendant:
		return "Ghost : I seek the Vitalis Essence Pendant.";
		break;
	case 7:
		//Quicksilver Sylph Amulet:
		return "Ghost : I yearn for the Quicksilver Sylph.";
		break;
	case 8:
		//Oceanic Whisper Conch:
		return "Ghost : The Whisper Conch beckons to me.";
		break;
	case 9:
		//Metamorphos Bloom Cocoon:
		return "Ghost : I yearn for the Bloom Cocoon .";
		break;
	case 10:
		//Celestial Wayfarer Compass:
		return "Ghost : I yearn for the Wayfarer Compass that points towards destiny.";
		break;
	}
}
#endif // !TRADING_H