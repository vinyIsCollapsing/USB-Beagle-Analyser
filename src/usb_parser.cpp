#include "usb_parser.h"
#include <string>

using namespace std;

DescriptorTypes getDescriptorTypes(const string &descriptorStr) {
    if (descriptorStr.find("Device Descriptor") != string::npos)
        return DescriptorTypes::DeviceDescriptor;
    else if (descriptorStr.find("Configuration Descriptor") != string::npos)
        return DescriptorTypes::ConfigurationDescriptor;
    else if (descriptorStr.find("Interface Descriptor") != string::npos)
        return DescriptorTypes::InterfaceDescriptor;
    else if (descriptorStr.find("Endpoint Descriptor") != string::npos)
        return DescriptorTypes::EndpointDescriptor;
    else if (descriptorStr.find("String Descriptor") != string::npos)
        return DescriptorTypes::StringDescriptor;
    else
        return DescriptorTypes::Other;
}

RequestTypes getRequestTypes(const string &requestStr) {
    if (requestStr.find("Clear Feature") != string::npos)
        return RequestTypes::CLEAR_FEATURE;
    else if (requestStr.find("Set Configuration") != string::npos)
        return RequestTypes::SET_CONFIGURATION;
    // Outras verificações podem ser adicionadas conforme necessário
    else
        return RequestTypes::Other;
}

size_t countLeadingSpaces(const string &str) {
    size_t pos = str.find_first_not_of(' ');
    return (pos == string::npos) ? str.size() : pos;
}
