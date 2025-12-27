#pragma once
#include "Pin.h"
#include <SFML/Graphics/RenderWindow.hpp>

class Wire {
public:
    Wire(Pin* start, Pin* end);
    
    void update();
    void draw(sf::RenderWindow& window);

    Pin* getStartPin() const { return start; }
    Pin* getEndPin() const { return end; }

private:
    Pin* start;
    Pin* end;
};
