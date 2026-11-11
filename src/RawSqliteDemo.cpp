#include "RawSqliteDemo.h"
#include <iostream>
#include <stdexcept>
#include <sqlite3.h>

int insertProjectRaw(sqlite3* db, const Project& project) {
    const char* sql = "INSERT INTO Projects (name, repositoryUrl, status) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_text(stmt, 1, project.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, project.repositoryUrl.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, project.status.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string err = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        throw std::runtime_error(err);
    }

    int id = static_cast<int>(sqlite3_last_insert_rowid(db));
    sqlite3_finalize(stmt); 
    return id;
}

std::optional<Project> findProjectByIdRaw(sqlite3* db, int id) {
    const char* sql = "SELECT id, name, repositoryUrl, status FROM Projects WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }

    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        Project p;
        p.id = sqlite3_column_int(stmt, 0);
        p.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        p.repositoryUrl = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        p.status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        sqlite3_finalize(stmt);
        return p;
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

void runRawSQLiteDemo() {
    std::cout << "\n=== RAW SQLITE C API DEMONSTRATION ===\n";

    sqlite3* db = nullptr;
    if (sqlite3_open("cicd_raw.db", &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << '\n';
        return;
    }

    const char* createTableSql = R"(
        CREATE TABLE IF NOT EXISTS Projects (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            repositoryUrl TEXT NOT NULL,
            status TEXT NOT NULL
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createTableSql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << (errMsg ? errMsg : "Unknown") << '\n';
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return;
    }
    std::cout << "Raw SQLite table 'Projects' is ready.\n";

    Project newProject{ 0, "Auth Service C++", "https://github.com/variant19/auth-service", "active" };
    int id = insertProjectRaw(db, newProject);
    std::cout << "Inserted Project ID (Raw API): " << id << '\n';

    auto found = findProjectByIdRaw(db, id);
    if (found) {
        std::cout << "Found Project via Raw API:\n"
            << "  ID: " << found->id << "\n"
            << "  Name: " << found->name << "\n"
            << "  Repo: " << found->repositoryUrl << "\n"
            << "  Status: " << found->status << '\n';
    }
    else {
        std::cout << "Project not found.\n";
    }

    sqlite3_close(db);
}