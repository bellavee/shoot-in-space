#pragma once
#include <d3d12.h>
#include <bitset>
#include <queue>
#include <map>

namespace Engine {
    enum class Keys 
    {
        Space = 0x20,
        Enter = 0xd,
        Shift = 0xa0,
        A = 0x41,
        D = 0x44,
        Q = 0x51,
        S = 0x53,
        Z = 0x5a,
    };
    enum class KeyState
    {
        Release,
        Press,
        Hold,
        Invalid,
    };

    int usedKeys[8] = { 0x20 , 0xd, 0xa0, 0x41, 0x44, 0x51, 0x53, 0x5a};

    class Keyboard
    {
    public:
        Keyboard();
        bool keyDown(char);
        void updateKeys();

    private:
        std::map<int, Engine::KeyState> mKeyStates;
    };
}
   