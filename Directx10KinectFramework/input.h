#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


////////////////////////////////////////////////////////////////////////////////
// Class: Input
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

	
	void MousePos(int,int);
	void ButtonDown(unsigned int);
	void ButtonUp(unsigned int);

	bool IsButtonDown(unsigned int); 
	int MouseX();
	int MouseY();

private:
	bool m_keys[256];

	bool m_Buttons[4];

	int m_xPos;
	int m_yPos;
};

#endif