#include "usb_parser.h"
#include <string>
#include <algorithm>

// Existing functions to determine descriptor/request types.
DescriptorTypes getDescriptorTypes(const std::string &descriptorStr) {
    if (descriptorStr.find("Device Descriptor") != std::string::npos)
        return DescriptorTypes::DeviceDescriptor;
    else if (descriptorStr.find("Configuration Descriptor") != std::string::npos)
        return DescriptorTypes::ConfigurationDescriptor;
    else if (descriptorStr.find("Interface Descriptor") != std::string::npos)
        return DescriptorTypes::InterfaceDescriptor;
    else if (descriptorStr.find("Endpoint Descriptor") != std::string::npos)
        return DescriptorTypes::EndpointDescriptor;
    else if (descriptorStr.find("String Descriptor") != std::string::npos)
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

size_t countLeadingSpaces(const std::string &str) {
    size_t pos = str.find_first_not_of(' ');
    return (pos == std::string::npos) ? str.size() : pos;
}

// New helper function: checks if the summary line represents an IN txn row
// that was not classified as a descriptor or request.
bool isAdditionalINTransaction(const std::string &summary) {
    // Check if the summary contains "IN txn" and that neither the descriptor nor request functions recognize it.
    if (summary.find("IN txn") != std::string::npos &&
        getDescriptorTypes(summary) == DescriptorTypes::Other &&
        getRequestTypes(summary) == RequestTypes::Other) {
        return true;
    }
    return false;
}

/*
 * Example usage:
 *
 * In your CSV parsing loop (where you iterate over rows and build Transaction objects),
 * add an extra branch to check for additional IN txn rows.
 *
 * For example:
 *
 *   for (const auto &row : csvRows) {
 *       std::string summary = row[row.size() - 1]; // Adjust index based on your CSV format.
 *
 *       // Check if row qualifies as a descriptor transaction.
 *       DescriptorTypes descType = getDescriptorTypes(summary);
 *       RequestTypes reqType = getRequestTypes(summary);
 *
 *       // If both are Other but the row is an IN txn, capture it as an additional IN transaction.
 *       if (descType == DescriptorTypes::Other && reqType == RequestTypes::Other &&
 *           isAdditionalINTransaction(summary)) {
 *           // You might create a new Transaction or add the row to a special category.
 *           Transaction txn;
 *           txn.descriptorType = DescriptorTypes::Other; // Or add a new enum value if desired.
 *           txn.requestType = RequestTypes::Other;       // Or, for example, RequestTypes::INTxn.
 *           txn.rows.push_back(row);
 *           additionalINTransactions.push_back(txn);  // Assume you have a vector<Transaction> additionalINTransactions.
 *       }
 *       else {
 *           // Process normally...
 *       }
 *   }
 *
 * Then you can later display or process all the additional IN txn rows as needed.
 */

