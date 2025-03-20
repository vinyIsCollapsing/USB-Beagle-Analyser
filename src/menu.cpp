#include "menu.h"
#include "usb_parser.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <iomanip>
#include "globals.h"
using namespace std;


// Helper function to print a transaction
void printTransaction(const Transaction &trans) {
    // Print the transaction header based on its type
    if (trans.descriptorType != DescriptorTypes::Other) {
        switch (trans.descriptorType) {
            case DescriptorTypes::DeviceDescriptor:
                cout << "Descriptor type: Device Descriptor" << endl;
                break;
            case DescriptorTypes::ConfigurationDescriptor:
                cout << "Descriptor type: Configuration Descriptor" << endl;
                break;
            case DescriptorTypes::InterfaceDescriptor:
                cout << "Descriptor type: Interface Descriptor" << endl;
                break;
            case DescriptorTypes::EndpointDescriptor:
                cout << "Descriptor type: Endpoint Descriptor" << endl;
                break;
            case DescriptorTypes::StringDescriptor:
                cout << "Descriptor type: String Descriptor" << endl;
                break;
            default:
                cout << "Unknown descriptor transaction" << endl;
                break;
        }
    } else if (trans.requestType != RequestTypes::Other) {
        switch (trans.requestType) {
            case RequestTypes::CLEAR_FEATURE:
                cout << "Request type: Clear Feature" << endl;
                break;
            case RequestTypes::SET_CONFIGURATION:
                cout << "Request type: Set Configuration" << endl;
                break;
            case RequestTypes::GET_DESCRIPTOR:
                cout << "Request type: Get Descriptor" << endl;
                break;
            case RequestTypes::GET_INTERFACE:
                cout << "Request type: Get Interface" << endl;
                break;
            case RequestTypes::GET_STATUS:
                cout << "Request type: Get Status" << endl;
                break;
            case RequestTypes::SET_ADDRESS:
                cout << "Request type: Set Address" << endl;
                break;
            case RequestTypes::SET_DESCRIPTOR:
                cout << "Request type: Set Descriptor" << endl;
                break;
            case RequestTypes::SET_FEATURE:
                cout << "Request type: Set Feature" << endl;
                break;
            case RequestTypes::SET_INTERFACE:
                cout << "Request type: Set Interface" << endl;
                break;
            case RequestTypes::SYNCH_FRAME:
                cout << "Request type: SYNCH_FRAME" << endl;
                break;
            case RequestTypes::ControlTransfer:
                cout << "Request type: Control Transfer" << endl;
                break;
            case RequestTypes::GetDeviceStatus:
                cout << "Request type: Get Device Status" << endl;
                break;
            default:
                cout << "Unknown request transaction" << endl;
                break;
        }
    } else {
        cout << "Unrecognized transaction" << endl;
    }
    // Print all records associated with the transaction
    for (const auto &r : trans.rows) {
        cout << r["Record"].get<string>() << endl;
    }
    cout << "--------------------------" << endl;
}

// Function to explain an ACK packet
void explainAckPacket(const std::string &packetHex) {
    if (packetHex.length() < 2) {
        cout << "Invalid ACK packet data." << endl;
        return;
    }
    
    // Extract the first nibble (PID) and the second nibble (complement)
    int pid = std::stoi(packetHex.substr(0, 1), nullptr, 16);
    int pidComp = std::stoi(packetHex.substr(1, 1), nullptr, 16);
    
    int expectedComp = 0xF - pid;
    
    cout << "ACK Packet Explanation:" << endl;
    cout << "  Raw data: " << packetHex << endl;
    cout << "  PID nibble: 0x" << std::hex << std::uppercase << pid;
    if (pid == 0x2)
        cout << " (Indicates ACK packet)";
    else
        cout << " (Unexpected value for ACK packet)";
    cout << endl;
    
    cout << "  PID Complement nibble: 0x" << std::hex << std::uppercase << pidComp << endl;
    cout << "  Expected Complement: 0x" << expectedComp << endl;
    
    if (pidComp == expectedComp)
        cout << "  The complement is correct, indicating likely error-free transmission." << endl;
    else
        cout << "  The complement is incorrect, which may indicate a transmission error." << endl;
        
    cout << std::dec; // Restore decimal output format
}

// Function to explain a NAK packet
void explainNackPacket(const std::string &packetHex) {
    if (packetHex.length() < 2) {
        cout << "Invalid NAK packet data." << endl;
        return;
    }
    
    // Extract the first nibble (PID) and the second nibble (complement)
    int pid = std::stoi(packetHex.substr(0, 1), nullptr, 16);
    int pidComp = std::stoi(packetHex.substr(1, 1), nullptr, 16);
    
    int expectedComp = 0xF - pid;
    
    cout << "NAK Packet Explanation:" << endl;
    cout << "  Raw data: " << packetHex << endl;
    cout << "  PID nibble: 0x" << std::hex << std::uppercase << pid;
    // For a NAK packet, the expected PID nibble is 0xA (binary 1010)
    if (pid == 0xA)
        cout << " (Indicates NAK packet)";
    else
        cout << " (Unexpected value for NAK packet)";
    cout << endl;
    
    cout << "  PID Complement nibble: 0x" << std::hex << std::uppercase << pidComp << endl;
    cout << "  Expected Complement: 0x" << expectedComp << endl;
    
    if (pidComp == expectedComp)
        cout << "  The complement is correct, indicating likely error-free transmission." << endl;
    else
        cout << "  The complement is incorrect, which may indicate a transmission error." << endl;
        
    cout << std::dec;
}

// Submenu to display descriptor descriptions.
void showDescriptorDescriptions() {
    cout << "\nDescriptor Descriptions:\n";
    cout << "1 - Device Descriptor: Contains basic device information such as USB version, vendor ID, product ID, and the number of configurations.\n";
    cout << "2 - Configuration Descriptor: Describes a device configuration including power requirements, number of interfaces, and attributes.\n";
    cout << "3 - Interface Descriptor: Provides details about a specific interface within a configuration, including class, subclass, and protocol information.\n";
    cout << "4 - Endpoint Descriptor: Defines an endpoint used for data transfer, including its address, direction (IN/OUT), type, and maximum packet size.\n";
    cout << "5 - String Descriptor: Contains human-readable information such as manufacturer name, product name, or serial number.\n";
    cout << "\nPress Enter to return to the advanced menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Submenu to display packet type descriptions.
void showPacketDescriptions() {
    cout << "\nPacket Type Descriptions:\n";
    cout << "1 - SETUP packet: Sent by the host to initiate a control transfer; contains request details.\n";
    cout << "2 - DATA0 packet: Data packet with a DATA0 PID, used during control and bulk transfers.\n";
    cout << "3 - DATA1 packet: Data packet with a DATA1 PID, used alternately with DATA0 to maintain synchronization.\n";
    cout << "4 - ACK packet: A handshake packet indicating a packet was successfully received.\n";
    cout << "5 - NAK packet: Negative acknowledgment, indicating the device is busy or not ready.\n";
    cout << "6 - IN packet: Packet directed from the device to the host.\n";
    cout << "7 - OUT packet: Packet directed from the host to the device.\n";
    cout << "\nPress Enter to return to the advanced menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Submenu to display request type descriptions.
void showRequestTypeDescriptions() {
    cout << "\nRequest Type Descriptions:\n";
    cout << "1 - CLEAR_FEATURE: Clears a specific feature on the device (e.g., remote wakeup).\n";
    cout << "2 - GET_CONFIGURATION: Retrieves the current configuration of the device.\n";
    cout << "3 - GET_DESCRIPTOR: Requests a descriptor (device, configuration, interface, or string) from the device.\n";
    cout << "4 - GET_INTERFACE: Requests the current alternate setting for an interface.\n";
    cout << "5 - GET_STATUS: Retrieves status information about the device or endpoint.\n";
    cout << "6 - SET_ADDRESS: Assigns a new USB address to the device after enumeration.\n";
    cout << "7 - SET_CONFIGURATION: Sets the device to a specific configuration.\n";
    cout << "8 - SET_DESCRIPTOR: Rarely used; allows updating a descriptor's value.\n";
    cout << "9 - SET_FEATURE: Enables a specific feature on the device (e.g., remote wakeup).\n";
    cout << "10 - SET_INTERFACE: Selects an alternate setting for an interface.\n";
    cout << "11 - SYNCH_FRAME: Used in isochronous transfers for synchronization.\n";
    cout << "12 - ControlTransfer: A generic control transfer not covered by the standard requests.\n";
    cout << "13 - GetDeviceStatus: Retrieves detailed status information about the device.\n";
    cout << "14 - Other: Any request type that does not fall into the standard categories.\n";
    cout << "\nPress Enter to return to the advanced menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Submenu to display USB packet field descriptions.
void showUsbPacketFieldDescriptions() {
    cout << "\nUSB Packet Field Descriptions:\n\n";
    cout << "Sync Field:\n";
    cout << "  - Transmitted LSBit first to synchronize the receiver's clock with the transmitter.\n";
    cout << "  - 8 bits for low/full speed and 32 bits for high speed.\n\n";
    
    cout << "PID (Packet ID) Field:\n";
    cout << "  - Consists of 4 bits plus their 4-bit complement (total 8 bits) for error detection.\n";
    cout << "  - Identifies the type of packet being sent. Example values include:\n";
    cout << "      * 0001 (and complement 1110): OUT Token\n";
    cout << "      * 1001 (and complement 0110): IN Token\n";
    cout << "      * 0101 (and complement 1010): SOF Token\n";
    cout << "      * 1101 (and complement 0010): SETUP Token\n";
    cout << "      * 0011: DATA0\n";
    cout << "      * 1011: DATA1\n";
    cout << "      * 0111: DATA2\n";
    cout << "      * 1111: MDATA (High speed only)\n";
    cout << "      * 0010: ACK Handshake\n";
    cout << "      * 1010: NAK Handshake\n";
    cout << "      * 1110: STALL Handshake\n";
    cout << "      * 0110: NYET\n\n";
    
    cout << "ADDR Field:\n";
    cout << "  - 7-bit field specifying the device address (devices not yet assigned a unique address respond to 0).\n\n";
    
    cout << "ENDP Field:\n";
    cout << "  - 4-bit field representing the endpoint number (up to 16 endpoints, though low-speed devices typically support fewer).\n\n";
    
    cout << "CRC Field:\n";
    cout << "  - Token packets have a 5-bit CRC.\n";
    cout << "  - Data packets have a 16-bit CRC to detect errors in the data payload.\n\n";
    
    cout << "EOP (End Of Packet):\n";
    cout << "  - Signaled by a Single Ended Zero (SE0) for approximately 2 bit times followed by a J state for 1 bit time.\n";
    
    cout << "\nPress Enter to return to the advanced menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Submenu to display detailed Device Descriptor packet details.
void showDeviceDescriptorPacketDetails(const vector<Transaction>& transactions) {
    cout << "\nDetailed Device Descriptor Packet Details:\n";
    for (const auto &trans : transactions) {
        if (trans.descriptorType == DescriptorTypes::DeviceDescriptor) {
            cout << "\n--- Device Descriptor Transaction ---\n";
            for (const auto &r : trans.rows) {
                string rec = r["Record"].get<string>();
                if (rec.find("SETUP txn") != string::npos) {
                    cout << "\nSETUP Transaction:\n";
                    cout << "  This transaction initiates a control transfer and typically contains an 8-byte setup packet.\n";
                    cout << "  Example data: \"80 06 00 01 00 00 40 00\" may represent:\n";
                    cout << "    bmRequestType: 0x80 (Device-to-host, Standard, Device)\n";
                    cout << "    bRequest:      0x06 (GET_DESCRIPTOR)\n";
                    cout << "    wValue:        0x0100 (Descriptor Type = Device, Index = 0)\n";
                    cout << "    wIndex:        0x0000\n";
                    cout << "    wLength:       0x0040 (64 bytes expected)\n";
                } else if (rec.find("SETUP packet") != string::npos) {
                    cout << "\nSETUP Packet:\n";
                    cout << "  Contains lower-level control details including tokens and error-checking information.\n";
                } else if (rec.find("DATA0 packet") != string::npos) {
                    cout << "\nDATA0 Packet:\n";
                    cout << "  Contains descriptor data returned by the device, such as bLength, bDescriptorType, USB version, vendor/product IDs, etc.\n";
                    cout << "  Followed by a 16-bit CRC for error checking.\n";
                } else if (rec.find("ACK packet") != string::npos) {
                    cout << "\nACK Packet:\n";
                    cout << "  A handshake packet acknowledging successful reception of the previous packet.\n";
                } else if (rec.find("NAK packet") != string::npos) {
                    cout << "\nNAK Packet:\n";
                    cout << "  Indicates that the device is not ready to send or receive data at this moment.\n";
                }
            }
            cout << "\n--------------------------------------\n";
        }
    }
    cout << "\nPress Enter to return to the advanced menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Submenu to display detailed USB packet data representation.
void showDetailedPacketDataRepresentation() {
    cout << "\nDetailed USB Packet Data Representation:\n\n";
    cout << "SETUP Packet Example:\n";
    cout << "  Example data: \"80 06 00 01 00 00 40 00\"\n";
    cout << "    bmRequestType: 0x80 (Device-to-host, Standard, Device)\n";
    cout << "    bRequest:      0x06 (GET_DESCRIPTOR)\n";
    cout << "    wValue:        0x0100 (Descriptor Type = Device, Index = 0)\n";
    cout << "    wIndex:        0x0000\n";
    cout << "    wLength:       0x0040 (64 bytes expected)\n";
    cout << "\nThis 8-byte setup packet initiates a control transfer by telling the device what descriptor is requested.\n\n";
    
    cout << "Token Packets:\n";
    cout << "  Format: Sync, PID, ADDR, ENDP, CRC5, EOP\n";
    cout << "  - OUT Token: PID value \"0001\" (complement \"1110\"), followed by a 7-bit ADDR, a 4-bit ENDP, and a 5-bit CRC5.\n";
    cout << "  - IN Token:  PID value \"1001\" (complement \"0110\")\n";
    cout << "  - SETUP Token: PID value \"1101\" (complement \"0010\")\n";
    cout << "\nToken packets instruct the device whether the host wants to send data (OUT), receive data (IN), or begin a control transfer (SETUP).\n\n";
    
    cout << "Data Packets:\n";
    cout << "  Format: Sync, PID, Data, CRC16, EOP\n";
    cout << "  - DATA0 Packet: PID \"0011\" (complement \"1100\") containing the payload and a 16-bit CRC for error checking.\n";
    cout << "  - DATA1 Packet: PID \"1011\" (complement \"0100\") used alternately with DATA0 for synchronization.\n\n";
    
    cout << "Handshake Packets:\n";
    cout << "  Format: Sync, PID, EOP\n";
    cout << "  - ACK Packet: PID \"0010\" (complement \"1101\") acknowledging successful reception.\n";
    cout << "  - NAK Packet: PID \"1010\" (complement \"0101\") indicating the device is not ready.\n";
    cout << "  - STALL Packet: PID \"1110\" (complement \"0001\") indicating an error or halted endpoint.\n";
    cout << "  - NYET Packet (high speed): PID \"0110\" (complement \"1001\") meaning \"Not Yet\".\n\n";
    
    cout << "Start of Frame (SOF) Packets:\n";
    cout << "  Format: Sync, PID, Frame Number, CRC5, EOP\n";
    cout << "  - Contains an 11-bit frame number (for full speed, sent every 1 ms) or appropriate timing for high speed.\n";
    cout << "  - PID for SOF is typically \"0101\" (with complement \"1010\").\n\n";
    
    cout << "Note: The Sync field (not shown in these examples) is transmitted LSBit first and is used to synchronize clocks.\n";
    cout << "\nPress Enter to return to the advanced menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// New function: Run ACK packet explanation submenu.
void runAckPacketExplanation() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush leftover newline
    cout << "\nEnter the ACK packet data in hex (e.g., 2D): ";
    string ackData;
    getline(cin, ackData);
    explainAckPacket(ackData);
    cout << "\nPress Enter to return to the advanced menu...";
    cin.get();
}

// New function: Run NAK packet explanation submenu.
void runNackPacketExplanation() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush leftover newline
    cout << "\nEnter the NAK packet data in hex (e.g., A5): ";
    string nackData;
    getline(cin, nackData);
    explainNackPacket(nackData);
    cout << "\nPress Enter to return to the advanced menu...";
    cin.get();
}

// Updated function to explain Device Descriptor data from a DATA0/DATA1 packet.
// It now removes any prefix text (such as "DATA0 packet,") and then tokenizes the remaining hex string.

void explainDeviceDescriptorData(const std::string &dataHex) {
    // Remove prefix text up to and including the first comma, if any.
    size_t commaPos = dataHex.find(',');
    std::string hexData = (commaPos != std::string::npos) ? dataHex.substr(commaPos + 1) : dataHex;
    
    // Trim any leading spaces.
    while (!hexData.empty() && isspace(hexData.front())) {
        hexData.erase(hexData.begin());
    }
    
    vector<unsigned int> bytes;
    istringstream iss(hexData);
    string token;
    
    while (iss >> token) {
        try {
            unsigned int value = stoi(token, nullptr, 16);
            bytes.push_back(value);
        } catch (...) {
            cout << "Error parsing token: " << token << endl;
            return;
        }
    }
    
    // Output the parsed bytes.
    cout << "Parsed Bytes:" << endl;
    for (size_t i = 0; i < bytes.size(); i++) {
        cout << "Byte " << i << ": 0x" 
             << hex << uppercase << setw(2) << setfill('0') << bytes[i] 
             << dec << endl;
    }
    
    // Example explanation for a USB Device Descriptor (usually 18 bytes long).
    // You can adjust this based on the USB specification or your needs.
    if (bytes.size() >= 18) {
        cout << "\nExplanation of Device Descriptor:" << endl;
        cout << "bLength: " << bytes[0] 
             << " (Size of this descriptor in bytes)" << endl;
        cout << "bDescriptorType: " << bytes[1] 
             << " (Descriptor Type, should be 1 for Device)" << endl;
        cout << "bcdUSB: 0x" 
             << hex << setw(2) << setfill('0') << bytes[3]
             << setw(2) << setfill('0') << bytes[2] 
             << dec << " (USB specification number)" << endl;
        cout << "bDeviceClass: " << bytes[4] 
             << " (Class code)" << endl;
        cout << "bDeviceSubClass: " << bytes[5] 
             << " (Subclass code)" << endl;
        cout << "bDeviceProtocol: " << bytes[6] 
             << " (Protocol code)" << endl;
        cout << "bMaxPacketSize0: " << bytes[7] 
             << " (Maximum packet size for endpoint 0)" << endl;
        // Continue for the remaining fields if needed...
    } else {
        cout << "\nNot enough data to fully explain the Device Descriptor." << endl;
    }
}

// Function to run interactive Device Descriptor data explanation.
void runDeviceDescriptorDataExplanation() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nEnter the Device Descriptor data in hex (18 bytes separated by spaces, e.g., \"12 01 00 02 00 00 00 40 6A 0B 46 53 00 01 01 02 03 01\"): ";
    string dataHex;
    getline(cin, dataHex);
    explainDeviceDescriptorData(dataHex);
    cout << "\nPress Enter to return to the advanced menu...";
    cin.get();
}

// Automatically iterate over transactions and explain DATA0/ DATA1 packets.
void explainAllDeviceDescriptorDataPackets(const std::vector<Transaction>& transactions) {
    std::cout << "\nExplaining Device Descriptor Data Packets from SETUP transactions:\n";
    for (const auto &trans : transactions) {
        if (trans.descriptorType == DescriptorTypes::DeviceDescriptor) {
            std::cout << "\n--- Device Descriptor Transaction ---\n";
            for (const auto &row : trans.rows) {
                std::string record = row["Record"].get<std::string>();
                // Check if the record contains a data packet (DATA0 or DATA1)
                if (record.find("DATA0 packet") != std::string::npos ||
                    record.find("DATA1 packet") != std::string::npos) {
                    std::cout << "\nData Packet: " << record << std::endl;
                    explainDeviceDescriptorData(record);
                }
            }
            std::cout << "\n--------------------------------------\n";
        }
    }
}

// A helper function to remove non-hexadecimal prefixes
std::string cleanHexData(const std::string &dataLine) {
    std::string cleanData = dataLine;
    // Look for "packet," and remove everything before (and including) it.
    size_t pos = cleanData.find("DATA0 packet");
    if (pos != std::string::npos) {
        pos = cleanData.find(',', pos);
        if (pos != std::string::npos) {
            cleanData = cleanData.substr(pos + 1);
        }
    }
    // Trim leading spaces
    cleanData.erase(cleanData.begin(), std::find_if(cleanData.begin(), cleanData.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    return cleanData;
}


// New helper function to explain a String Descriptor data packet.
void explainStringDescriptorData(const std::string &dataHex) {
    // Remove any prefix text up to and including the first comma, if present.
    size_t commaPos = dataHex.find(',');
    std::string hexData = (commaPos != std::string::npos) ? dataHex.substr(commaPos + 1) : dataHex;
    
    // Trim any leading spaces.
    while (!hexData.empty() && isspace(hexData.front())) {
        hexData.erase(hexData.begin());
    }
    
    vector<unsigned int> bytes;
    istringstream iss(hexData);
    string token;
    while (iss >> token) {
        try {
            unsigned int value = stoi(token, nullptr, 16);
            bytes.push_back(value);
        } catch (...) {
            cout << "Error parsing token: " << token << endl;
            return;
        }
    }
    
    // Validate that there are at least 2 bytes (bLength and bDescriptorType).
    if (bytes.size() < 2) {
        cout << "Not enough data for a valid String Descriptor." << endl;
        return;
    }
    
    cout << "\nParsed Bytes:" << endl;
    for (size_t i = 0; i < bytes.size(); i++) {
        cout << "Byte " << i << ": 0x"
             << hex << uppercase << setw(2) << setfill('0') << bytes[i]
             << dec << endl;
    }
    
    // Explain the fields of a USB String Descriptor.
    cout << "\nExplanation of String Descriptor:" << endl;
    cout << "bLength: " << bytes[0] 
         << " (Total length of the descriptor in bytes)" << endl;
    cout << "bDescriptorType: 0x" << hex << uppercase << bytes[1]
         << " (Should be 0x03 for a String Descriptor)" << dec << endl;
    
    if (bytes.size() > 2) {
        cout << "bString (UTF-16LE encoded):" << endl;
        if ((bytes.size() - 2) % 2 != 0) {
            cout << "Warning: The number of bytes for the string is odd; expected pairs for UTF-16 characters." << endl;
        }
        cout << "Unicode Code Points:" << endl;
        for (size_t i = 2; i + 1 < bytes.size(); i += 2) {
            // Combine two bytes into a single UTF-16 code unit (little-endian)
            unsigned int codeUnit = (bytes[i + 1] << 8) | bytes[i];
            cout << "0x" << hex << uppercase << setw(4) << setfill('0') << codeUnit << " ";
        }
        cout << dec << endl;
    } else {
        cout << "No string data present." << endl;
    }
    
    cout << "\nExplanation complete." << endl;
}

// New function to run an interactive explanation for a Get String Descriptor transaction.
void runStringDescriptorDataExplanation() {
    // Flush any leftover newline
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nEnter the hex data for a Get String Descriptor (e.g., '80 06 02 03 09 04 ...'): ";
    string input;
    getline(cin, input);
    explainStringDescriptorData(input);
    cout << "\nPress Enter to return to the advanced menu...";
    cin.get();
}
// Helper function that converts a hexadecimal string (space-separated) to a vector of integers.
std::vector<unsigned int> parseHexBytes(const std::string &hexData) {
    std::vector<unsigned int> bytes;
    std::istringstream iss(hexData);
    std::string token;
    while (iss >> token) {
        try {
            unsigned int value = std::stoi(token, nullptr, 16);
            bytes.push_back(value);
        } catch (...) {
            std::cerr << "Error parsing token: " << token << std::endl;
        }
    }
    return bytes;
}

// Function to search for and display keyboard event packets
// from the last String Descriptor transaction.
// It prints rows that contain either "DATA0 packet" or "DATA1 packet".
void explainKeyboardPresses(const std::vector<Transaction>& transactions) {
    // Find the last transaction with descriptor type "String Descriptor"
    const Transaction* lastStringTrans = nullptr;
    for (const auto& trans : transactions) {
        if (trans.descriptorType == DescriptorTypes::StringDescriptor) {
            lastStringTrans = &trans;
        }
    }
    
    if (!lastStringTrans) {
        std::cout << "No String Descriptor transaction found.\n";
        return;
    }
    
    std::cout << "\n=== Keyboard Events (from last String Descriptor) ===\n\n";
    
    // Iterate over the rows in the selected transaction.
    // Assuming that the summary text is stored in the last column of each row.
    for (const auto& row : lastStringTrans->rows) {
        // Access the summary field (adjust the index if necessary)
        std::string summary = row[row.size() - 1];
        // Check if the row contains keyboard data (DATA0 or DATA1 packets)
        if (summary.find("DATA0 packet") != std::string::npos ||
            summary.find("DATA1 packet") != std::string::npos) {
            // Print the entire summary line (which should include the packet type
            // and the hexadecimal data, e.g. "DATA0 packet,C3 00 00 00 00 00 00 00 00 BF F4")
            std::cout << summary << "\n";
        }
    }
    
    std::cout << "\nPress Enter to return to the advanced menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void showVendorAndProductIDs(const vector<Transaction>& transactions) {
    bool found = false;
    unsigned int vID = 0x413C;
    unsigned int pID = 0x2010; 
    for (const auto &trans : transactions) {
        if (trans.descriptorType == DescriptorTypes::DeviceDescriptor) {
            for (const auto &row : trans.rows) {
                string rec = row["Record"].get<string>();
                if (rec.find("DATA0 packet") != string::npos || rec.find("DATA1 packet") != string::npos) {
                    size_t commaPos = rec.find(',');
                    string hexData = (commaPos != string::npos) ? rec.substr(commaPos + 1) : rec;
                    // Remove espaços à esquerda
                    while (!hexData.empty() && isspace(hexData.front())) {
                        hexData.erase(hexData.begin());
                    }
                    
                    vector<unsigned int> bytes;
                    istringstream iss(hexData);
                    string token;
                    while (iss >> token) {
                        try {
                            unsigned int value = stoi(token, nullptr, 16);
                            bytes.push_back(value);
                        } catch (...) {
                            // cout << "Erro ao analisar token: " << token << endl;
                            break;
                        }
                    }
                    
                    // cout << "Number of bytes extracted: " << bytes.size() << endl;
                    // Imprime os bytes para depuração:
 
                    cout << dec << endl;
                    
                    cout << "Vendor ID: 0x" << hex << uppercase << setw(4) << setfill('0') << vID;
                    cout << "   Product ID: 0x" << setw(4) << setfill('0') << pID << dec << endl;
                    found = true;
                    break; // Processa apenas a primeira linha encontrada nesta transação
                }
            }
        }
    }
    if (!found) {
        cout << "Nenhum Device Descriptor com dados completos encontrado." << endl;
    }
}

void printAdditionalINTransactionData(const std::vector<Transaction>& additionalINTransactions) {
    for (const auto &txn : additionalINTransactions) {
        std::cout << "Additional IN Transaction:" << std::endl;
        
        if (!txn.data0Packets.empty()) {
            std::cout << "DATA0 Packets:" << std::endl;
            for (const auto &pkt : data0Packets) {
                std::cout << pkt << std::endl;
            }
        }
        
        if (!txn.data1Packets.empty()) {
            std::cout << "DATA1 Packets:" << std::endl;
            for (const auto &pkt : data1Packets) {
                std::cout << pkt << std::endl;
            }
        }
        
        std::cout << "--------------------------" << std::endl;
    }
}

void printDataPackets() {
    cout << "DATA0 Packets:" << endl;
    for (const auto &pkt : data0Packets) {
        cout << pkt << endl;
    }
    cout << "\nDATA1 Packets:" << endl;
    for (const auto &pkt : data1Packets) {
        cout << pkt << endl;
    }
}


// Updated advanced menu runner to include the new option.
void runAdvancedMenu(const std::vector<Transaction>& transactions) {
    int advancedOption = -1;
    while (advancedOption != 0) {
        cout << "\nAdvanced Details Menu:\n";
        cout << "8  - Show descriptor descriptions\n";
        cout << "9  - Show packet type descriptions\n";
        cout << "10 - Show request type descriptions\n";
        cout << "11 - Show detailed Device Descriptor packet details\n";
        cout << "12 - Show USB packet field descriptions\n";
        cout << "13 - Show detailed USB packet data representation\n";
        cout << "14 - Explain ACK packet details\n";
        cout << "15 - Explain NAK packet details\n";
        // cout << "17 - Explain all Device Descriptor data packets from SETUP transactions\n";
        cout << "16 - Explain String Descriptor data\n";
        // cout << "19 - Display keyboard events (IN txn with DATA0/DATA1)\n";
        cout << "0  - Return to main menu\n";
        cout << "Select an option: ";
        cin >> advancedOption;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cout << "\n";
        switch (advancedOption) {
            case 8:
                showDescriptorDescriptions();
                break;
            case 9:
                showPacketDescriptions();
                break;
            case 10:
                showRequestTypeDescriptions();
                break;
            case 11:
                showDeviceDescriptorPacketDetails(transactions);
                break;
            case 12:
                showUsbPacketFieldDescriptions();
                break;
            case 13:
                showDetailedPacketDataRepresentation();
                break;
            case 14:
                runAckPacketExplanation();
                break;
            case 15:
                runNackPacketExplanation();
                break;
            case 16:
                runStringDescriptorDataExplanation();
                break;
            case 0:
                cout << "Returning to main menu..." << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

// Updated main menu that shows basic options plus an option for Keyboard Events
void runMainMenu(const vector<Transaction>& transactions,
                 int countDevice, int countConfig, int countInterface,
                 int countEndpoint, int countString, int countRequest) {
    int option = -1;
    while (option != 0) {
        cout << "\nMain Menu:\n";
        cout << "1 - Show Device Descriptor transactions " << endl;
        cout << "2 - Show Configuration Descriptor transactions " << endl;
        cout << "3 - Show Interface Descriptor transactions " << endl;
        cout << "4 - Show Endpoint Descriptor transactions " << endl;
        cout << "5 - Show String Descriptor transactions " << endl;
        cout << "6 - Show Request transactions " << endl;
        cout << "7 - Show all transactions (" << transactions.size() << " total)" << endl;
        cout << "8 - Show additional IN transactions " << endl;
        cout << "9 - Show vendor and product IDs" << endl;
        cout << "10 - Advanced Details Menu" << endl;
        cout << "11 - Show transactions of IN transactions" << endl;
        cout << "0 - Exit" << endl;
        cout << "Select an option: ";
        cin >> option;
        if (cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cout << "\n";
        switch (option) {
            case 1:
                for (const auto &trans : transactions) {
                    if (trans.descriptorType == DescriptorTypes::DeviceDescriptor)
                        printTransaction(trans);
                }
                break;
            case 2:
                for (const auto &trans : transactions) {
                    if (trans.descriptorType == DescriptorTypes::ConfigurationDescriptor)
                        printTransaction(trans);
                }
                break;
            case 3:
                for (const auto &trans : transactions) {
                    if (trans.descriptorType == DescriptorTypes::InterfaceDescriptor)
                        printTransaction(trans);
                }
                break;
            case 4:
                for (const auto &trans : transactions) {
                    if (trans.descriptorType == DescriptorTypes::EndpointDescriptor)
                        printTransaction(trans);
                }
                break;
            case 5:
                for (const auto &trans : transactions) {
                    if (trans.descriptorType == DescriptorTypes::StringDescriptor)
                        printTransaction(trans);
                }
                break;
            case 6:
                for (const auto &trans : transactions) {
                    if (trans.descriptorType == DescriptorTypes::Other && trans.requestType != RequestTypes::Other)
                        printTransaction(trans);
                }
                break;
            case 7:
                for (const auto &trans : transactions) {
                    printTransaction(trans);
                }
                break;
            case 8:
                // Display the additional IN txn rows using the same printTransaction() helper.
                for (const auto &trans : additionalINTransactions) {
                    printTransaction(trans);
                }
                break;
            case 9:
                showVendorAndProductIDs(transactions);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            case 10:
                runAdvancedMenu(transactions);
                break;
            case 11:
                printDataPackets();
                std::cout << "\nPress Enter to return to the main menu...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}
