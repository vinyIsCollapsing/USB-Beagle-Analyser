#include <iostream>
#include <string>
#include <vector>
#include "csv.hpp"

using namespace csv;
using namespace std;

// Define os tipos de descritores
enum class DescriptorTypes {
    DeviceDescriptor,
    ConfigurationDescriptor,
    InterfaceDescriptor,
    EndpointDescriptor,
    StringDescriptor,
    Other   // Para descritores que não se encaixam nos casos principais
};

// Define os tipos de requisições
enum class RequestTypes {
    CLEAR_FEATURE,
    GET_CONFIGURATION,
    GET_DESCRIPTOR,
    GET_INTERFACE,
    GET_STATUS,
    SET_ADDRESS,
    SET_CONFIGURATION,
    SET_DESCRIPTOR,
    SET_FEATURE,
    SET_INTERFACE,
    SYNCH_FRAME,
    ControlTransfer,
    GetDeviceStatus,
    Other   // Para transações que não se encaixam nos casos principais
};

// Função que converte a string do registro para o DescriptorTypes
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

// Função que converte a string do registro para o RequestTypes
RequestTypes getRequestTypes(const std::string &requestStr) {
    if (requestStr.find("Clear Feature") != std::string::npos)
        return RequestTypes::CLEAR_FEATURE;
    else if (requestStr.find("Set Configuration") != std::string::npos)
        return RequestTypes::SET_CONFIGURATION;
    // Outras verificações podem ser adicionadas conforme necessário
    else
        return RequestTypes::Other;
}

// Estrutura para armazenar uma transação
struct Transaction {
    DescriptorTypes descriptorType = DescriptorTypes::Other;
    RequestTypes requestType = RequestTypes::Other;
    vector<CSVRow> rows;
};

// Função auxiliar para contar os espaços iniciais (indentação)
size_t countLeadingSpaces(const std::string &str) {
    size_t pos = str.find_first_not_of(' ');
    return (pos == std::string::npos) ? str.size() : pos;
}

int main() {
    // Lê o arquivo CSV (certifique-se que o caminho esteja correto)
    CSVReader reader("../USB-Beagle-Analyser/test/DumpClavier.csv");
    vector<Transaction> transactions;
    Transaction currentTransaction;
    bool capturando = false;
    size_t nivelInicial = 0;  // Nível de indentação da linha que inicia a transação

    // Percorre cada linha do CSV
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
                // Finaliza a transação anterior, se houver
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
            // Se encontrar uma linha sem indentação que não inicia transação, finaliza a transação atual
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
