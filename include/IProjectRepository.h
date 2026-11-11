#pragma once

#include <optional>
#include <vector>
#include "Project.h"

class IProjectRepository {
public:
    virtual ~IProjectRepository() = default;

    virtual int add(const Project& project) = 0;

    virtual std::optional<Project> findById(int id) = 0;
    virtual std::vector<Project> findAll() = 0;

    virtual bool update(const Project& project) = 0;

    virtual bool remove(int id) = 0;
};