add_executable(imgui-sfml-example
  main.cpp
  Core/Component.cpp
  Core/Pin.cpp
  Core/Wire.cpp
  Core/Gates.cpp
  Core/IOComponents.cpp
  Simulation/Circuit.cpp
)

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
