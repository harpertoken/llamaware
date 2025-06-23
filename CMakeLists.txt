cmake_minimum_required(VERSION 3.14)
project(agent)

set(CMAKE_CXX_STANDARD 17)

# Find dependencies
find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)

# Add include directory for header-only dotenv-cpp
include_directories(include)

add_executable(agent agent.cpp)

# Link libraries
if(TARGET nlohmann_json::nlohmann_json)
    target_link_libraries(agent PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
else()
    target_include_directories(agent PRIVATE ${nlohmann_json_INCLUDE_DIRS})
    target_link_libraries(agent PRIVATE cpr::cpr)
endif() 