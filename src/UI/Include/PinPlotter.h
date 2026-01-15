#pragma once
#include <deque>
#include <string>
#include <vector>
#include "../../Core/Pin.h"
#include "ToggableImGuiWindow.h"

class PinPlotter : public ToggableImGuiWindow {
public:
    static constexpr size_t MIN_HISTORY = 50;
    static constexpr size_t MAX_HISTORY = 500;
    static constexpr size_t DEFAULT_HISTORY = 200;
    static constexpr size_t MAX_PINS = 5;

    PinPlotter() = default;
    ~PinPlotter() override = default;

    bool addPin(Pin* pin, const std::string& label);
    void removePin(Pin* pin);
    void clear();
    void recordValues();
    void renderUI() override;
    
    size_t getPinCount() const { return monitoredPins.size(); }

private:
    struct MonitoredPin {
        Pin* pin;
        std::string label;
        std::deque<float> history;
    };
    
    std::vector<MonitoredPin> monitoredPins;
    int historySize = static_cast<int>(DEFAULT_HISTORY);
};
