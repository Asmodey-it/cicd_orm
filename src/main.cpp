#include <iostream>
#include "RawSqliteDemo.h"
#include "TestData.h"
#include "QueryDemo.h"
#include "TransactionDemo.h"

int main() {
    try {

        runRawSQLiteDemo();

        prepareTestData();

        runQueryDemo();

        runTransactionDemo();

        std::cout << "\n=== ALL LAB 8 TASKS COMPLETED SUCCESSFULLY! ===\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "Application error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}