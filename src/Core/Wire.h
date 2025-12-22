#pragma once
#include "Pin.h"
#include <SFML/Graphics/RenderWindow.hpp>

class Wire {
public:
    Wire(Pin* start, Pin* end);
    
    void update();
    void draw(sf::RenderWindow& window);

    Pin* getStartPin() const { return m_start; }
    Pin* getEndPin() const { return m_end; }

private:
    Pin* m_start;
    Pin* m_end;
};
