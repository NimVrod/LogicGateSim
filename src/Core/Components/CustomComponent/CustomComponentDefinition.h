#pragma once
#include <string>

// Forward declaration to avoid circular dependencies
class Circuit;

// CustomComponentDefinition stores the template for creating CustomComponent instances.
// It contains the serialized circuit XML and metadata about the component.
struct CustomComponentDefinition {
    std::string name; // Display name (e.g., "Half Adder")
    std::string circuitXml; // Serialized sub-circuit
    int numInputs; // Number of input pins
    int numOutputs; // Number of output pins

    CustomComponentDefinition() : numInputs(0), numOutputs(0) {
    }

    CustomComponentDefinition(const std::string &name, const std::string &xml, int inputs, int outputs)
        : name(name), circuitXml(xml), numInputs(inputs), numOutputs(outputs) {
    }
};