add_executable(imgui-sfml-example
  main.cpp
  Core/Component.cpp
  Core/Pin.cpp
  Core/Wire.cpp
  Simulation/Circuit.cpp
 "Core/Components/Source/Button.cpp" "Core/Components/Include/Button.h" "Core/Components/Include/Gate.h"
"Core/Components/Source/Gate.cpp" "Core/Components/Source/AndGate.cpp" "Core/Components/Include/AndGate.h" "Core/Components/Source/OrGate.cpp" "Core/Components/Include/OrGate.h")

# Yep, that's it!
target_link_libraries(imgui-sfml-example
  PUBLIC
    ImGui-SFML::ImGui-SFML
)

target_include_directories(imgui-sfml-example
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


include(Install.cmake)
