#ifndef MENU_H
#define MENU_H

#include <vector>
#include "usb_parser.h"

// Basic menu and submenu functions
void printTransaction(const Transaction &trans);
void showDescriptorDescriptions();
void showPacketDescriptions();
void showRequestTypeDescriptions();
void showUsbPacketFieldDescriptions();
void showDeviceDescriptorPacketDetails(const std::vector<Transaction>& transactions);
void showDetailedPacketDataRepresentation();

// Functions to explain specific packet types
void explainAckPacket(const std::string &packetHex);
void explainNackPacket(const std::string &packetHex);
void explainDeviceDescriptorData(const std::string &dataHex);

// Submenu runners for packet explanations
void runAckPacketExplanation();
void runNackPacketExplanation();
void runDeviceDescriptorDataExplanation(); // versão interativa (usuário digita os dados)

// New function: automatically iterate over transactions and explain DATA0/ DATA1 packets
void explainAllDeviceDescriptorDataPackets(const std::vector<Transaction>& transactions);
std::string cleanHexData(const std::string &dataLine);

void explainStringDescriptorData(const std::string &dataHex);
void runStringDescriptorDataExplanation();
void showVendorAndProductIDs(const std::vector<Transaction>& transactions);
void printAdditionalINTransactionData(const std::vector<Transaction>& additionalINTransactions);

// Main menu runners
void runAdvancedMenu(const std::vector<Transaction>& transactions);
void runMainMenu(const std::vector<Transaction>& transactions,
                 int countDevice, int countConfig, int countInterface,
                 int countEndpoint, int countString, int countRequest);

#endif // MENU_H
