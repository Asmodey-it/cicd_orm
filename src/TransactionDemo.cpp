#include "TransactionDemo.h"
#include "Database.h"
#include <iostream>
#include <stdexcept>

void runTransactionDemo() {
    auto storage = createStorage("cicd_queries.db");
    storage.open_forever();
    storage.on_open = [](sqlite3* db) {
        sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
        };
    storage.sync_schema();

    std::cout << "\n=== TRANSACTION DEMONSTRATION ===\n";

    try {
        bool committed = storage.transaction([&]() {
            Project p{ 0, "Microservice X", "https://github.com/variant19/micro-x", "active" };
            int projId = storage.insert(p);

            Build b{ 0, projId, "2026-11-11", "success", 150.0 };
            storage.insert(b);

            return true; 
            });
        std::cout << "Transaction COMMIT result: " << std::boolalpha << committed << '\n';
    }
    catch (const std::exception& e) {
        std::cerr << "Transaction failed: " << e.what() << '\n';
    }

    std::cout << "Projects count after COMMIT: " << storage.count<Project>() << '\n';

    std::cout << "\n--- ROLLBACK Scenario (Foreign Key Violation) ---\n";
    try {
        storage.transaction([&]() {
            Project p{ 0, "Broken Project", "https://github.com/variant19/broken", "active" };
            int projId = storage.insert(p);
            std::cout << "Project inserted inside transaction with ID: " << projId << '\n';

            Build invalidBuild{ 0, -1, "2026-11-11", "failed", 50.0 };
            storage.insert(invalidBuild); 

            return true;
            });
    }
    catch (const std::exception& e) {
        std::cout << "Transaction failed as expected: " << e.what() << '\n';
        std::cout << "Changes rolled back automatically!\n";
    }

    using namespace sqlite_orm;
    auto brokenProjects = storage.get_all<Project>(
        where(c(&Project::name) == "Broken Project")
    );
    std::cout << "Broken Project exists after ROLLBACK: " << std::boolalpha << !brokenProjects.empty() << '\n';
}