#pragma once

struct InputCommands
{
	// Keyboard
	bool forward;	// W
	bool back;		// S
	bool right;		// D
	bool left;		// A
	bool up;		// E
	bool down;		// Q
	bool rotRight;	// C
	bool rotLeft;	// Z
	bool rotUp;		// P
	bool rotDown;	// O

	bool control;	// L-ctrl
	bool selectAll; // L-ctrl + A

	// Mouse
	float mouseXCoord;
	float mouseYCoord;
	
	bool LeftClick;
	bool RightClick;

	bool WindowLock;
	bool WindowLockToggler;
	
};
