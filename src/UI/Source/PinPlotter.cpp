#include "../Include/PinPlotter.h"
#include "imgui.h"
#include <algorithm>
#include <cfloat>

bool PinPlotter::addPin(Pin *pin, const std::string &label) {
    if (!pin) return false;

    if (monitoredPins.size() >= MAX_PINS) return false;

    auto it = std::find_if(monitoredPins.begin(), monitoredPins.end(),
                           [pin](const MonitoredPin &mp) { return mp.pin == pin; });
    if (it != monitoredPins.end()) return false;

    MonitoredPin mp;
    mp.pin = pin;
    mp.label = label;
    monitoredPins.push_back(mp);

    visible = true;

    return true;
}

void PinPlotter::removePin(Pin *pin) {
    auto it = std::find_if(monitoredPins.begin(), monitoredPins.end(),
                           [pin](const MonitoredPin &mp) { return mp.pin == pin; });
    if (it != monitoredPins.end()) {
        monitoredPins.erase(it);
    }
}

void PinPlotter::clear() {
    monitoredPins.clear();
}

void PinPlotter::recordValues() {
    for (auto &mp: monitoredPins) {
        if (mp.pin) {
            auto value = static_cast<float>(mp.pin->getValue());
            mp.history.push_back(value);

            while (mp.history.size() > static_cast<size_t>(historySize)) {
                mp.history.pop_front();
            }
        }
    }
}

void PinPlotter::renderUI() {
    if (!visible) return;

    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 150), ImVec2(FLT_MAX, FLT_MAX));

    if (ImGui::Begin("Wykres pinów", &visible)) {
        if (monitoredPins.empty()) {
            ImGui::TextDisabled("Brak monitorowanych pinów.");
            ImGui::TextDisabled("Kliknij prawym na komponent i wybierz 'Wykres pinu...'");
        } else {
            ImGui::Text("Monitorowane piny: %zu / %zu", monitoredPins.size(), MAX_PINS);
            ImGui::Separator();

            for (size_t i = 0; i < monitoredPins.size();) {
                auto &mp = monitoredPins[i];
                ImGui::PushID(static_cast<int>(i));

                bool keepOpen = true;
                if (ImGui::CollapsingHeader(mp.label.c_str(), &keepOpen, ImGuiTreeNodeFlags_DefaultOpen)) {
                    int currentValue = mp.pin ? mp.pin->getValue() : 0;
                    ImGui::Text("Aktualna: %d", currentValue);

                    if (!mp.history.empty()) {
                        std::vector values(mp.history.begin(), mp.history.end());

                        ImGui::PlotLines("##plot", values.data(), static_cast<int>(values.size()),
                                         0, nullptr, 0.0f, 1.5f, ImVec2(-1, 60));
                    } else {
                        ImGui::TextDisabled("Brak danych");
                    }
                }

                ImGui::PopID();

                if (!keepOpen) {
                    monitoredPins.erase(monitoredPins.begin() + i);
                } else {
                    ++i;
                }
            }

            ImGui::Separator();
            ImGui::SliderInt("Próbki", &historySize, MIN_HISTORY, MAX_HISTORY);
            if (ImGui::Button("Wyczyść wszystko")) {
                clear();
            }
        }
    }
    ImGui::End();
}