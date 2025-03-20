#include <iostream>
#include <vector>
#include "csv.hpp"
#include "usb_parser.h"
#include "menu.h"

using namespace std;
using namespace csv;

int main() {
    // Read the CSV file (adjust the path as necessary)
    CSVReader reader("../USB-Beagle-Analyser/test/DumpClavier.csv");
    vector<Transaction> transactions;
    Transaction currentTransaction;
    bool capturing = false;
    size_t initialIndent = 0;  // Indentation level for a new transaction

    // Process CSV rows and group them into transactions
    for (CSVRow &row : reader) {
        string record = row["Record"].get<string>();
        if (record.empty())
            continue;
        size_t indent = countLeadingSpaces(record);
        string trimmed = record.substr(indent);
        if (indent == 0) {
            DescriptorTypes dType = getDescriptorTypes(trimmed);
            RequestTypes rType = getRequestTypes(trimmed);
            if (dType != DescriptorTypes::Other || rType != RequestTypes::Other) {
                if (capturing) {
                    transactions.push_back(currentTransaction);
                    currentTransaction.rows.clear();
                }
                capturing = true;
                initialIndent = indent;
                currentTransaction.descriptorType = dType;
                currentTransaction.requestType = rType;
                currentTransaction.rows.push_back(row);
                continue;
            }
            if (capturing) {
                transactions.push_back(currentTransaction);
                currentTransaction.rows.clear();
                capturing = false;
            }
        } else {
            if (capturing) {
                currentTransaction.rows.push_back(row);
            }
        }
    }
    if (capturing) {
        transactions.push_back(currentTransaction);
    }

    // Compute counts for each transaction type
    int countDevice = 0, countConfig = 0, countInterface = 0, countEndpoint = 0, countString = 0, countRequest = 0;
    for (const auto &trans : transactions) {
        if (trans.descriptorType != DescriptorTypes::Other) {
            switch (trans.descriptorType) {
                case DescriptorTypes::DeviceDescriptor: countDevice++; break;
                case DescriptorTypes::ConfigurationDescriptor: countConfig++; break;
                case DescriptorTypes::InterfaceDescriptor: countInterface++; break;
                case DescriptorTypes::EndpointDescriptor: countEndpoint++; break;
                case DescriptorTypes::StringDescriptor: countString++; break;
                default: break;
            }
        } else if (trans.requestType != RequestTypes::Other) {
            countRequest++;
        }
    }

    // Run the main menu
    runMainMenu(transactions, countDevice, countConfig, countInterface, countEndpoint, countString, countRequest);

    return 0;
}
