#pragma once

#include "application.h"

class game : public application
{
public:
    game(HINSTANCE h_instance);
    ~game();

    bool initialize() override;

private:
    void update(const timer& timer) override;
    void render(const timer& timer) override;
    
};