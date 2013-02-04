#include "input.h"

//Need implementation for mouse and to modulize input capture 

InputClass::InputClass()
{
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


void InputClass::Initialize()
{
	int i;
	

	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		m_keys[i] = false;
	}

	// Initialize all the buttons to being released and not pressed.
	for(i=0; i<4; i++)
	{
		m_keys[i] = false;
	}

	m_xPos = 0;
	m_yPos = 0;

	return;
}


void InputClass::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}


void InputClass::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

void InputClass::MousePos(int x, int y)
{
	// Update the mouses stored position
	m_xPos = x;
	m_yPos = y;
	return;
}

void InputClass::ButtonDown(unsigned int input)
{
	// If a button is pressed then save that state in the button array.
	m_keys[input] = true;
	return;
}

void InputClass::ButtonUp(unsigned int input)
{
	// If a button is released then clear that state in the button array.
	m_keys[input] = false;
	return;
}


bool InputClass::IsButtonDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_Buttons[key];
}

int InputClass::MouseX()
{
	return m_xPos;
}

int InputClass::MouseY()
{
	return m_yPos;
}

