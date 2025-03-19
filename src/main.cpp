#include <iostream>
#include <string>
#include <vector>
#include "csv.hpp"
#include "usb_parser.h"

using namespace std;
using namespace csv;

int main() {
    // Certifique-se de que o caminho para o CSV esteja correto
    CSVReader reader("../USB-Beagle-Analyser/test/DumpClavier.csv");
    vector<Transaction> transactions;
    Transaction currentTransaction;
    bool capturando = false;
    size_t nivelInicial = 0;  // Nível de indentação da linha que inicia a transação

    for (CSVRow &row : reader) {
        string record = row["Record"].get<string>();
        if (record.empty())
            continue;

        size_t indent = countLeadingSpaces(record);
        string trimmed = record.substr(indent);

        // Se a linha não estiver indentada, pode ser o início de uma nova transação
        if (indent == 0) {
            DescriptorTypes dType = getDescriptorTypes(trimmed);
            RequestTypes rType = getRequestTypes(trimmed);

            // Se o registro iniciar uma transação (descritor ou requisição)
            if (dType != DescriptorTypes::Other || rType != RequestTypes::Other) {
                if (capturando) {
                    transactions.push_back(currentTransaction);
                    currentTransaction.rows.clear();
                }
                capturando = true;
                nivelInicial = indent;
                currentTransaction.descriptorType = dType;
                currentTransaction.requestType = rType;
                currentTransaction.rows.push_back(row);
                continue;
            }
            // Linha sem indentação que não inicia transação finaliza a transação atual
            if (capturando) {
                transactions.push_back(currentTransaction);
                currentTransaction.rows.clear();
                capturando = false;
            }
        } else {
            // Linhas indentadas fazem parte da transação atual (se estivermos capturando)
            if (capturando) {
                currentTransaction.rows.push_back(row);
            }
        }
    }
    // Caso o arquivo termine enquanto ainda estamos capturando uma transação
    if (capturando) {
        transactions.push_back(currentTransaction);
    }

    // Processa as transações capturadas
    for (const auto &trans : transactions) {
        // Processa transações de descritores
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
                    cout << "Transação de descritor desconhecida" << endl;
                    break;
            }
        }
        // Processa transações de requisições
        else if (trans.requestType != RequestTypes::Other) {
            switch (trans.requestType) {
                case RequestTypes::CLEAR_FEATURE:
                    cout << "Request type: Clear Feature" << endl;
                    break;
                case RequestTypes::SET_CONFIGURATION:
                    cout << "Request type: Set Configuration" << endl;
                    break;
                default:
                    cout << "Transação de requisição desconhecida" << endl;
                    break;
            }
        }
        else {
            cout << "Transação não reconhecida" << endl;
        }
        
        // Exibe os registros da transação
        for (const auto &r : trans.rows) {
            cout << r["Record"].get<string>() << endl;
        }
        cout << "--------------------------" << endl;
    }

    return 0;
}
