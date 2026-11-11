cmake_minimum_required(VERSION 3.21)
project(CICD_ORM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Виправляємо назву на SqliteOrm (з великими літерами)
find_package(SqliteOrm CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_executable(CICD_ORM
    src/main.cpp
    src/SQLiteProjectRepository.cpp
    src/TestData.cpp
    src/RawSqliteDemo.cpp
    src/QueryDemo.cpp
    src/TransactionDemo.cpp
)

target_include_directories(CICD_ORM PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(CICD_ORM PRIVATE
    sqlite_orm::sqlite_orm
    unofficial::sqlite3::sqlite3
)