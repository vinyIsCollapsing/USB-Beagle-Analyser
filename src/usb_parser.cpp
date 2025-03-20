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

RequestTypes getRequestTypes(const std::string &requestStr) {
    if (requestStr.find("Clear Feature") != std::string::npos)
        return RequestTypes::CLEAR_FEATURE;
    else if (requestStr.find("Set Configuration") != std::string::npos)
        return RequestTypes::SET_CONFIGURATION;
    else if (requestStr.find("Get Device Descriptor") != std::string::npos)
        return RequestTypes::GET_DESCRIPTOR;
    else if (requestStr.find("Get Configuration Descriptor") != std::string::npos)
        return RequestTypes::GET_DESCRIPTOR;
    else if (requestStr.find("Get String Descriptor") != std::string::npos)
        return RequestTypes::GET_DESCRIPTOR;
    else if (requestStr.find("Get Interface") != std::string::npos)
        return RequestTypes::GET_INTERFACE;
    else if (requestStr.find("Get Status") != std::string::npos)
        return RequestTypes::GET_STATUS;
    else if (requestStr.find("Set Address") != std::string::npos)
        return RequestTypes::SET_ADDRESS;
    else if (requestStr.find("Set Descriptor") != std::string::npos)
        return RequestTypes::SET_DESCRIPTOR;
    else if (requestStr.find("Set Feature") != std::string::npos)
        return RequestTypes::SET_FEATURE;
    else if (requestStr.find("Set Interface") != std::string::npos)
        return RequestTypes::SET_INTERFACE;
    else if (requestStr.find("SYNCH_FRAME") != std::string::npos || requestStr.find("SYNCH") != std::string::npos)
        return RequestTypes::SYNCH_FRAME;
    else if (requestStr.find("Control Transfer") != std::string::npos)
        return RequestTypes::ControlTransfer;
    else if (requestStr.find("Get Device Status") != std::string::npos)
        return RequestTypes::GetDeviceStatus;
    else
        return RequestTypes::Other;
}


size_t countLeadingSpaces(const string &str) {
    size_t pos = str.find_first_not_of(' ');
    return (pos == string::npos) ? str.size() : pos;
}
