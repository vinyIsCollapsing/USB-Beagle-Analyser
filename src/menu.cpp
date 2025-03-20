#include "menu.h"
#include "usb_parser.h"
#include <iostream>
#include <limits>
#include <string>
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

// Shows a submenu with descriptor descriptions.
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

// Shows a submenu with packet type descriptions.
void showPacketDescriptions() {
    cout << "\nPacket Type Descriptions:\n";
    cout << "1 - SETUP packet: Sent by the host to initiate a control transfer; contains request details.\n";
    cout << "2 - DATA0 packet: Data packet with a DATA0 PID, used during control and bulk transfers.\n";
    cout << "3 - DATA1 packet: Data packet with a DATA1 PID, used alternately with DATA0 to maintain synchronization.\n";
    cout << "4 - ACK packet: Acknowledgment packet indicating a packet was successfully received.\n";
    cout << "5 - NAK packet: Negative acknowledgment, indicating the device is busy or not ready.\n";
    cout << "6 - IN packet: Packet directed from the device to the host.\n";
    cout << "7 - OUT packet: Packet directed from the host to the device.\n";
    cout << "\nPress Enter to return to the advanced menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Shows a submenu with request type descriptions.
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

// Shows a submenu with USB packet field descriptions.
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
    cout << "  - Signaled by a Single Ended Zero (SE0) for approximately 2 bit times followed by a J state for 1 bit time.\n\n";
    
    cout << "Press Enter to return to the advanced menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Shows a submenu with detailed Device Descriptor packet details.
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

// Shows a submenu with detailed USB packet data representation.
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
    cout << "  - Contains an 11-bit frame number (for full speed, sent every 1 ms) or the appropriate timing for high speed.\n";
    cout << "  - PID for SOF is typically \"0101\" (with complement \"1010\").\n\n";

    cout << "Note: The Sync field (not shown in these examples) is transmitted LSBit first and is used to synchronize clocks.\n";
    cout << "\nPress Enter to return to the advanced menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Advanced menu that shows only options 8 to 13
void runAdvancedMenu(const vector<Transaction>& transactions) {
    int advancedOption = -1;
    while (advancedOption != 0) {
        cout << "\nAdvanced Details Menu:\n";
        cout << "8  - Show descriptor descriptions\n";
        cout << "9  - Show packet type descriptions\n";
        cout << "10 - Show request type descriptions\n";
        cout << "11 - Show detailed Device Descriptor packet details\n";
        cout << "12 - Show USB packet field descriptions\n";
        cout << "13 - Show detailed USB packet data representation\n";
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
            case 0:
                cout << "Returning to main menu..." << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

// Main menu that shows basic options plus an option to access the advanced menu.
void runMainMenu(const vector<Transaction>& transactions,
                 int countDevice, int countConfig, int countInterface,
                 int countEndpoint, int countString, int countRequest) {
    int option = -1;
    while (option != 0) {
        cout << "\nMain Menu:\n";
        cout << "1 - Show Device Descriptor transactions (" << countDevice << " occurrences)" << endl;
        cout << "2 - Show Configuration Descriptor transactions (" << countConfig << " occurrences)" << endl;
        cout << "3 - Show Interface Descriptor transactions (" << countInterface << " occurrences)" << endl;
        cout << "4 - Show Endpoint Descriptor transactions (" << countEndpoint << " occurrences)" << endl;
        cout << "5 - Show String Descriptor transactions (" << countString << " occurrences)" << endl;
        cout << "6 - Show Request transactions (" << countRequest << " occurrences)" << endl;
        cout << "7 - Show all transactions (" << transactions.size() << " total)" << endl;
        cout << "8 - Advanced Details Menu\n";  // Option to call the advanced menu (which shows options 8-13)
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
                runAdvancedMenu(transactions);
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}
