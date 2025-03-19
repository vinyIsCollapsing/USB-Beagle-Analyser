#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "csv.hpp"
#include "usb_parser.h"

using namespace std;
using namespace csv;

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

int main() {
    // Read the CSV file (adjust the path as necessary)
    CSVReader reader("../USB-Beagle-Analyser/test/DumpClavier.csv");
    vector<Transaction> transactions;
    Transaction currentTransaction;
    bool capturing = false;
    size_t initialIndent = 0;  // Indentation level of the line that starts a transaction

    // Process each CSV row and group lines into transactions
    for (CSVRow &row : reader) {
        string record = row["Record"].get<string>();
        if (record.empty())
            continue;

        size_t indent = countLeadingSpaces(record);
        string trimmed = record.substr(indent);

        // A non-indented line may indicate the start of a new transaction
        if (indent == 0) {
            DescriptorTypes dType = getDescriptorTypes(trimmed);
            RequestTypes rType = getRequestTypes(trimmed);

            // If the record starts a transaction (descriptor or request)
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
            // If a non-indented line doesn't start a transaction, finalize the current transaction
            if (capturing) {
                transactions.push_back(currentTransaction);
                currentTransaction.rows.clear();
                capturing = false;
            }
        } else {
            // Indented lines are part of the current transaction (if capturing)
            if (capturing) {
                currentTransaction.rows.push_back(row);
            }
        }
    }
    // In case the file ends while still capturing a transaction
    if (capturing) {
        transactions.push_back(currentTransaction);
    }

    // Interactive menu for displaying data
    int option = -1;
    while (option != 0) {
        cout << "\nDisplay Menu:\n";
        cout << "1 - Show Device Descriptor transactions\n";
        cout << "2 - Show Configuration Descriptor transactions\n";
        cout << "3 - Show Interface Descriptor transactions\n";
        cout << "4 - Show Endpoint Descriptor transactions\n";
        cout << "5 - Show String Descriptor transactions\n";
        cout << "6 - Show Request transactions (non-descriptors)\n";
        cout << "7 - Show all transactions\n";
        cout << "0 - Exit\n";
        cout << "Select an option: ";
        cin >> option;

        if (cin.fail()) {
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
                // Transactions where the descriptor type is not recognized but a request type is present
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
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
    return 0;
}
