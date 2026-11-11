#include "TestData.h"
#include "Database.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <stdexcept>

void prepareTestData() {
    const std::string dbFile = "cicd_queries.db";
    std::filesystem::remove(dbFile); 

    auto storage = createStorage(dbFile);
    storage.on_open = [](sqlite3* db) {
        if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Cannot enable foreign keys");
        }
        };
    storage.open_forever();
    storage.sync_schema();

    std::vector<Project> projects = {
        {0, "Backend API", "https://github.com/variant19/backend", "active"},
        {0, "Web Frontend", "https://github.com/variant19/frontend", "active"},
        {0, "Auth Service", "https://github.com/variant19/auth", "maintenance"},
        {0, "Mobile App", "https://github.com/variant19/mobile", "active"},
        {0, "Legacy System", "https://github.com/variant19/legacy", "suspended"}
    };

    std::vector<int> projectIds;
    for (const auto& p : projects) {
        projectIds.push_back(storage.insert(p));
    }

    std::vector<Build> builds = {
        {0, projectIds[0], "2026-10-01", "success", 120.5},
        {0, projectIds[0], "2026-10-02", "failed",   45.0},
        {0, projectIds[0], "2026-10-03", "success", 110.0},

        {0, projectIds[1], "2026-10-01", "success", 300.2},
        {0, projectIds[1], "2026-10-02", "success", 290.0},
        {0, projectIds[1], "2026-10-03", "unstable",310.5},

        {0, projectIds[2], "2026-09-25", "success",  85.0},
        {0, projectIds[2], "2026-09-26", "failed",   20.0},
        {0, projectIds[2], "2026-10-01", "success",  90.0},

        {0, projectIds[3], "2026-10-02", "success", 210.0},
        {0, projectIds[3], "2026-10-03", "success", 195.5},
        {0, projectIds[3], "2026-10-04", "success", 205.0},

        {0, projectIds[4], "2026-08-10", "failed",  600.0},
        {0, projectIds[4], "2026-08-11", "failed",  580.0},
        {0, projectIds[4], "2026-08-12", "success", 610.0}
    };

    for (const auto& b : builds) {
        storage.insert(b);
    }

    std::cout << "Test database prepared successfully.\n";
    std::cout << "Projects count: " << storage.count<Project>() << '\n';
    std::cout << "Builds count: " << storage.count<Build>() << '\n';
}