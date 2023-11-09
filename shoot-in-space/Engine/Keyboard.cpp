#include "Keyboard.h"


namespace Engine 
{
	Keyboard::Keyboard()
	{
		for (int i = 0; i < sizeof(usedKeys); i++)
		{
			mKeyStates[usedKeys[i]] = KeyState::Release;
		}
	}

	void Keyboard::updateKeys()
	{
		for (int i = 0; i < sizeof(usedKeys); i++)
		{
			if (mKeyStates[usedKeys[i]] == KeyState::Release || mKeyStates[usedKeys[i]] == KeyState::Invalid)
			{
				mKeyStates[usedKeys[i]] = KeyState::Press;
			}
			else
			{
				mKeyStates[usedKeys[i]] = KeyState::Release;
			}
		}
	}
	bool Keyboard::keyDown(char vKeyCode)
	{
		return (mKeyStates[vKeyCode] == KeyState::Press);
	}
}
