#ifndef MENU_H
#define MENU_H

#include <vector>
#include "usb_parser.h"

// Basic submenu functions
void printTransaction(const Transaction &trans);
void showDescriptorDescriptions();
void showPacketDescriptions();
void showRequestTypeDescriptions();
void showUsbPacketFieldDescriptions();
void showDeviceDescriptorPacketDetails(const std::vector<Transaction>& transactions);
void showDetailedPacketDataRepresentation();

// Menu functions
void runAdvancedMenu(const std::vector<Transaction>& transactions);
void runMainMenu(const std::vector<Transaction>& transactions,
                 int countDevice, int countConfig, int countInterface,
                 int countEndpoint, int countString, int countRequest);

#endif // MENU_H
