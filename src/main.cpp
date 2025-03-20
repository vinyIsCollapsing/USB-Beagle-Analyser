#include "csv.hpp"
#include "usb_parser.h"
#include "menu.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace csv;

// Global vector to hold additional IN transactions.
vector<Transaction> additionalINTransactions;

int main() {
    CSVReader reader("../USB-Beagle-Analyser/test/DumpClavier.csv");
    vector<Transaction> transactions;
    Transaction currentTransaction;
    bool capturing = false;
    size_t initialIndent = 0;

    // Process CSV rows and group them into transactions.
    for (CSVRow &row : reader) {
        // Dentro do seu loop que processa cada CSVRow...
string record = row["Record"].get<string>();
if (record.empty())
    continue;

size_t indent = countLeadingSpaces(record);
string trimmed = record.substr(indent);

if (indent == 0) {
    // Processamento normal de início de transação:
    DescriptorTypes dType = getDescriptorTypes(trimmed);
    RequestTypes rType = getRequestTypes(trimmed);
    
    if (dType != DescriptorTypes::Other || rType != RequestTypes::Other) {
        if (capturing) {
            transactions.push_back(currentTransaction);
            currentTransaction.rows.clear();
            // Opcional: Limpe também os vetores dos pacotes se for necessário.
            currentTransaction.data0Packets.clear();
            currentTransaction.data1Packets.clear();
        }
        capturing = true;
        initialIndent = indent;
        currentTransaction.descriptorType = dType;
        currentTransaction.requestType = rType;
        currentTransaction.rows.push_back(row);
        // Continue para próxima linha (não se preocupe com DATA0/DATA1 aqui)
        continue;
    }
    // Se não for reconhecido, verifique se é uma transação IN adicional, etc.
    // (Seu código já existente para tratar additionalINTransactions permanece.)
    
    if (capturing) {
        transactions.push_back(currentTransaction);
        currentTransaction.rows.clear();
        currentTransaction.data0Packets.clear();
        currentTransaction.data1Packets.clear();
        capturing = false;
    }
} else {
    if (capturing) {
        // Se for uma linha de continuação da transação:
        currentTransaction.rows.push_back(row);
        // Verifica se é um pacote DATA0 ou DATA1 e extrai os dados:
        if (record.find("DATA0 packet") != string::npos) {
            size_t commaPos = record.find(',');
            string hexData = (commaPos != string::npos) ? record.substr(commaPos + 1) : record;
            // Remove espaços à esquerda:
            hexData.erase(hexData.begin(), std::find_if(hexData.begin(), hexData.end(), [](int ch) {
                return !std::isspace(ch);
            }));
            currentTransaction.data0Packets.push_back(hexData);
        } else if (record.find("DATA1 packet") != string::npos) {
            size_t commaPos = record.find(',');
            string hexData = (commaPos != string::npos) ? record.substr(commaPos + 1) : record;
            hexData.erase(hexData.begin(), std::find_if(hexData.begin(), hexData.end(), [](int ch) {
                return !std::isspace(ch);
            }));
            currentTransaction.data1Packets.push_back(hexData);
        }
    }
}

    }
    if (capturing) {
        transactions.push_back(currentTransaction);
    }

    // Then run your main menu (which now can use both transactions and additionalINTransactions).
    runMainMenu(transactions,
                /*countDevice*/ 0, /*countConfig*/ 0,
                /*countInterface*/ 0, /*countEndpoint*/ 0,
                /*countString*/ 0, /*countRequest*/ 0);

    return 0;
}
