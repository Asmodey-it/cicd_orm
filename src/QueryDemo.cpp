#include "QueryDemo.h"
#include "Database.h"
#include <iostream>

void runQueryDemo() {
    using namespace sqlite_orm;
    auto storage = createStorage("cicd_queries.db");
    storage.sync_schema();

    std::cout << "\n=== ORM QUERY DEMONSTRATION ===\n";

    std::cout << "\n1. Active projects:\n";
    auto activeProjects = storage.get_all<Project>(
        where(c(&Project::status) == "active")
    );
    for (const auto& p : activeProjects) {
        std::cout << "  ID: " << p.id << " | Name: " << p.name << " | Status: " << p.status << '\n';
    }

    std::cout << "\n2. Projects sorted by name:\n";
    auto sortedProjects = storage.get_all<Project>(
        order_by(&Project::name).asc()
    );
    for (const auto& p : sortedProjects) {
        std::cout << "  ID: " << p.id << " | Name: " << p.name << '\n';
    }

    std::cout << "\n3. Top 3 longest builds (Limit + Order By):\n";
    auto topBuilds = storage.get_all<Build>(
        order_by(&Build::durationSeconds).desc(),
        limit(3)
    );
    for (const auto& b : topBuilds) {
        std::cout << "  Build ID: " << b.id << " | Status: " << b.status << " | Duration: " << b.durationSeconds << "s\n";
    }

    std::cout << "\n4. Aggregations:\n";
    std::cout << "  Total builds count: " << storage.count<Build>() << '\n';

    auto totalDuration = storage.sum(&Build::durationSeconds);
    std::cout << "  Total duration of all builds: " << (totalDuration ? *totalDuration : 0.0) << "s\n";

    auto avgDuration = storage.avg(&Build::durationSeconds);
    std::cout << "  Average build duration: " << avgDuration << "s\n";

    std::cout << "  Total duration by project (GROUP BY):\n";
    auto durationByProject = storage.select(
        columns(
            &Build::projectId,
            sum(&Build::durationSeconds)
        ),
        group_by(&Build::projectId)
    );
    for (const auto& row : durationByProject) {
        auto projId = std::get<0>(row);
        // Використовуємо посилання, щоб уникнути копіювання унікального вказівника
        const auto& sumDuration = std::get<1>(row);
        std::cout << "    Project ID: " << projId << " | Total Duration: " << (sumDuration ? *sumDuration : 0.0) << "s\n";
    }

    std::cout << "\n5. INNER JOIN (Project name + Successful build duration):\n";
    auto joinResults = storage.select(
        columns(
            &Project::name,
            &Build::status,
            &Build::durationSeconds
        ),
        inner_join<Build>(
            on(c(&Project::id) == &Build::projectId)
        ),
        where(c(&Build::status) == "success"),
        order_by(&Build::durationSeconds).desc()
    );

    for (const auto& row : joinResults) {
        auto projName = std::get<0>(row);
        auto buildStatus = std::get<1>(row);
        auto duration = std::get<2>(row);
        std::cout << "  Project: " << projName << " | Status: " << buildStatus << " | Duration: " << duration << "s\n";
    }
}