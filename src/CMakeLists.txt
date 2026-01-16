add_executable(LogicSimulator
  main.cpp
        Core/Components/Source/Component.cpp
        Core/Components/Source/Pin.cpp
        Core/Components/Source/Wire.cpp
        Core/Components/Source/ComponentFactory.cpp Core/Components/Include/ComponentFactory.h
  "Core/Components/Source/Button.cpp" "Core/Components/Include/Button.h" "Core/Components/Include/Gate.h" "Core/Components/Source/Gate.cpp"
  "Core/Components/Source/AndGate.cpp" "Core/Components/Include/AndGate.h"
  "Core/Components/Source/OrGate.cpp" "Core/Components/Include/OrGate.h"
  "Core/Components/Source/NotGate.cpp" "Core/Components/Include/NotGate.h"
  "Core/Components/Source/NandGate.cpp" "Core/Components/Include/NandGate.h"
  "Core/Components/Source/NorGate.cpp" "Core/Components/Include/NorGate.h"
  "Core/Components/Source/XorGate.cpp" "Core/Components/Include/XorGate.h"
  "Core/Components/Source/XnorGate.cpp" "Core/Components/Include/XnorGate.h"
  "Core/Components/Source/InputComponent.cpp" "Core/Components/Include/InputComponent.h"
  "Core/Components/Source/OutputComponent.cpp" "Core/Components/Include/OutputComponent.h"
  "Core/Components/CustomComponent/CustomComponent.cpp" "Core/Components/CustomComponent/CustomComponent.h"
  "Core/Components/Source/ClockComponent.cpp" "Core/Components/Include/ClockComponent.h"
  "Core/Components/Source/LEDComponent.cpp" "Core/Components/Include/LEDComponent.h"
  "Core/Components/Source/SRFlipFlop.cpp" "Core/Components/Include/SRFlipFlop.h"
  "Core/Components/Source/DFlipFlop.cpp" "Core/Components/Include/DFlipFlop.h"
  "Core/Components/Source/JKFlipFlop.cpp" "Core/Components/Include/JKFlipFlop.h"
  "Core/Components/Source/TFlipFlop.cpp" "Core/Components/Include/TFlipFlop.h"
  "Core/Components/Source/FlipFlop.cpp" "Core/Components/Include/FlipFlop.h"
  "Core/Components/CustomComponent/CustomComponentManager.cpp" "Core/Components/CustomComponent/CustomComponentManager.h"
  "Core/Components/CustomComponent/CustomComponentDefinition.h"
        Simulation/Circuit.cpp
        Core/Components/Source/ResourceManager.cpp
        Core/Components/Include/ResourceManager.h
        Serialization/CircuitSerializer.cpp
        UI/Source/ComponentPreview.cpp UI/Include/ComponentPreview.h
        UI/Source/UIManager.cpp UI/Include/UIManager.h
        UI/Source/PinPlotter.cpp UI/Include/PinPlotter.h
        UI/Include/ToggableImGuiWindow.h)

# Yep, that's it!
target_link_libraries(LogicSimulator
  PUBLIC
    ImGui-SFML::ImGui-SFML
    tinyxml2
    nfd
)

target_include_directories(LogicSimulator
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command(TARGET LogicSimulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:LogicSimulator>/assets)

include(Install.cmake)
