#include "SQLiteProjectRepository.h"
#include <stdexcept>
#include <sqlite3.h>

SQLiteProjectRepository::SQLiteProjectRepository(const std::string& databaseFile)
    : storage_(createStorage(databaseFile)) {

    storage_.on_open = [](sqlite3* db) {
        if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Cannot enable foreign key constraints");
        }
        };

    storage_.open_forever();
    storage_.sync_schema();
}

int SQLiteProjectRepository::add(const Project& project) {
    return storage_.insert(project);
}

std::optional<Project> SQLiteProjectRepository::findById(int id) {
    return storage_.get_optional<Project>(id);
}

std::vector<Project> SQLiteProjectRepository::findAll() {
    return storage_.get_all<Project>();
}

bool SQLiteProjectRepository::update(const Project& project) {
    auto existing = storage_.get_optional<Project>(project.id);
    if (!existing) {
        return false; 
    }
    storage_.update(project);
    return true;
}

bool SQLiteProjectRepository::remove(int id) {
    auto existing = storage_.get_optional<Project>(id);
    if (!existing) {
        return false; 
    }
    storage_.remove<Project>(id);
    return true;
}