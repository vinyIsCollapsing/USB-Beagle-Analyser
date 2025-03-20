// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include "usb_parser.h" // or the header where Transaction is defined

extern std::vector<Transaction> additionalINTransactions;

extern     std::vector<std::string> data0Packets = {
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4",
        "C3 00 00 00 00 00 00 00 00 BF F4"
    };

    // Vetor de pacotes DATA1 (valores de exemplo)
extern     std::vector<std::string> data1Packets = {
        "4B 00 00 0C 00 00 00 00 00 BF 38",
        "4B 00 00 0D 00 00 00 00 00 BE E9",
        "4B 00 00 0E 00 00 00 00 00 BE DA",
        "4B 00 00 0F 00 00 00 00 00 BF 0B",
        "4B 00 00 33 00 00 00 00 00 BA 37",
        "4B 00 00 11 00 00 00 00 00 BC B5",
        "4B 00 00 12 00 00 00 00 00 BC 86",
        "4B 00 00 13 00 00 00 00 00 BD 57",
        "4B 00 00 04 00 00 00 00 00 BE 70",
        "4B 00 00 15 00 00 00 00 00 BD 31",
        "4B 00 00 16 00 00 00 00 00 BD 02",
        "4B 00 00 17 00 00 00 00 00 BC D3",
        "4B 00 00 18 00 00 00 00 00 BC 2C",
        "4B 00 00 19 00 00 00 00 00 BD FD",
        "4B 00 00 1D 00 00 00 00 00 BC 79",
        "4B 00 00 1B 00 00 00 00 00 BC 1F",
        "4B 00 00 1C 00 00 00 00 00 BD A8",
        "4B 00 00 1A 00 00 00 00 00 BD CE",
    };

#endif // GLOBALS_H
