#include <iostream>
#include <string>
#include <vector>
#include "csv.hpp"

using namespace csv;
using namespace std;

// Define os tipos de transação
enum class TransactionType {
    GetDeviceDescriptor,
    SetAddress,
    GetConfigurationDescriptor,
    GetStringDescriptor,
    SetConfiguration,
    ControlTransfer,
    GetDeviceStatus,
    Other // Para transações que não se enquadram nos casos principais
};

// Função que converte a string do registro para o TransactionType
TransactionType getTransactionType(const std::string &transactionStr) {
    if(transactionStr.find("Get Device Descriptor") != std::string::npos)
        return TransactionType::GetDeviceDescriptor;
    else if(transactionStr.find("Set Address") != std::string::npos)
        return TransactionType::SetAddress;
    else if(transactionStr.find("Get Configuration Descriptor") != std::string::npos)
        return TransactionType::GetConfigurationDescriptor;
    else if(transactionStr.find("Get String Descriptor") != std::string::npos)
        return TransactionType::GetStringDescriptor;
    else if(transactionStr.find("Set Configuration") != std::string::npos)
        return TransactionType::SetConfiguration;
    else if(transactionStr.find("Control Transfer") != std::string::npos)
        return TransactionType::ControlTransfer;
    else if(transactionStr.find("Get Device Status") != std::string::npos)
        return TransactionType::GetDeviceStatus;
    else
        return TransactionType::Other;
}

// Estrutura para armazenar uma transação
struct Transaction {
    TransactionType type;
    vector<CSVRow> rows;
};

// Função auxiliar para contar os espaços iniciais (indentação)
size_t countLeadingSpaces(const std::string &str) {
    return str.find_first_not_of(' ');
}

int main() {
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

        // Se a linha não está indentada, pode ser o início de uma nova transação
        if (indent == 0) {
            TransactionType type = getTransactionType(trimmed);
            // Se for um tipo de transação que nos interessa
            if (type != TransactionType::Other) {
                // Se já estivermos capturando uma transação, finalize-a
                if (capturando) {
                    transactions.push_back(currentTransaction);
                    currentTransaction.rows.clear();
                }
                capturando = true;
                nivelInicial = indent;
                currentTransaction.type = type;
                currentTransaction.rows.push_back(row);
                continue;
            }
        }
        // Se estamos capturando uma transação, as linhas indentadas são parte dela
        if (capturando) {
            if (indent > nivelInicial) {
                currentTransaction.rows.push_back(row);
            } else {
                // Linha sem indentação indica o fim da transação atual
                transactions.push_back(currentTransaction);
                currentTransaction.rows.clear();
                capturando = false;
            }
        }
    }
    // Caso o arquivo termine enquanto ainda estamos capturando uma transação
    if (capturando) {
        transactions.push_back(currentTransaction);
    }

    // Exemplo de uso do switch/case para processar as transações
    for (const auto &trans : transactions) {
        switch (trans.type) {
            case TransactionType::GetDeviceDescriptor:
                cout << "Traitement de transaction: Get Device Descriptor" << endl;
                /*
                for (const auto& row : trans.rows) {
                    cout << row["Record"].get<string>() << endl;
                }
                cout << "--------------------------" << endl;
                */
                break;
            case TransactionType::SetAddress:
                cout << "Traitement de transaction: Set Address" << endl;
                
                break;
            case TransactionType::GetConfigurationDescriptor:
                cout << "Traitement de transaction: Get Configuration Descriptor" << endl;
                
                break;
            case TransactionType::GetStringDescriptor:
                cout << "Traitement de transaction: Get String Descriptor" << endl;
                
                break;
            case TransactionType::SetConfiguration:
                cout << "Traitement de transaction: Set Configuration" << endl;
                
                break;
            case TransactionType::ControlTransfer:
                cout << "Traitement de transaction: Control Transfer" << endl;
                
                break;
            case TransactionType::GetDeviceStatus:
                cout << "Traitement de transaction: Get Device Status" << endl;
                
                break;
            default:
                cout << "Transaction n'est pas connu" << endl;
                break;
        }
    }

    return 0;
}
