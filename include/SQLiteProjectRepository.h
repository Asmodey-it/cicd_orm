#pragma once

#include <string>
#include "IProjectRepository.h"
#include "Database.h"

class SQLiteProjectRepository : public IProjectRepository {
public:
    explicit SQLiteProjectRepository(const std::string& databaseFile);

    int add(const Project& project) override;
    std::optional<Project> findById(int id) override;
    std::vector<Project> findAll() override;
    bool update(const Project& project) override;
    bool remove(int id) override;

private:
    using Storage = decltype(createStorage(""));
    Storage storage_;
};