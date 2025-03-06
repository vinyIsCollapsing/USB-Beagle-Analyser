#include <iostream>
#include "csv.hpp"

using namespace csv;
using namespace std;

int main() {
    // Create a CSVReader that automatically reads the header row.
    CSVReader reader("../USB-Beagle-Analyser/test/DumpClavierABCDEFGHIJKLMNOPQRSTUVWXYZ.csv");

        // Iterate over each row in the CSV file.
    for (CSVRow& row : reader) {
        // Access the column "timestamp" directly by its header name.
        if (row["Level"].is_int()) {
            int ts = row["Level"].get<int>();
            cout << "Level: " << ts << endl;
        }

        int hexValue;
        if (row["Sp"].try_parse_hex(hexValue)) {
            cout << "Hex value is " << hexValue << endl;
        }

        long double decimalValue;
        if (row["Record"].try_parse_decimal(decimalValue, ',')) {
            cout << "Decimal value is " << decimalValue << endl;
        }
    }

    return 0;
}
