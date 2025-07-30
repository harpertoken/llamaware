cmake_minimum_required(VERSION 3.14)
project(LlamawareAgent VERSION 2.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build configuration
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Find dependencies
find_package(nlohmann_json REQUIRED)

# Fetch CPR using FetchContent to avoid system installation conflicts
include(FetchContent)
FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
    GIT_SHALLOW TRUE
)
set(CPR_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE)
set(BUILD_CPR_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cpr)

# Include directories
include_directories(include)

# Source files organized by modules
set(CORE_SOURCES
    src/core/agent.cpp
)

set(SERVICE_SOURCES
    src/services/command_service.cpp
    src/services/file_service.cpp
    src/services/web_service.cpp
    src/services/ai_service.cpp
)

set(UTILS_SOURCES
    src/utils/ui.cpp
    src/utils/config.cpp
    src/utils/version.cpp
)

set(DATA_SOURCES
    src/data/memory_manager.cpp
)

set(ALL_SOURCES
    ${CORE_SOURCES}
    ${SERVICE_SOURCES}
    ${UTILS_SOURCES}
    ${DATA_SOURCES}
)

# Main executable
add_executable(llamaware-agent src/main.cpp ${ALL_SOURCES})

# Set target properties
set_target_properties(llamaware-agent PROPERTIES
    OUTPUT_NAME "llamaware-agent"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Link libraries
if(TARGET nlohmann_json::nlohmann_json)
    target_link_libraries(llamaware-agent PRIVATE 
        cpr::cpr 
        nlohmann_json::nlohmann_json
    )
else()
    target_include_directories(llamaware-agent PRIVATE ${nlohmann_json_INCLUDE_DIRS})
    target_link_libraries(llamaware-agent PRIVATE cpr::cpr)
endif()

# Installation
install(TARGETS llamaware-agent
    RUNTIME DESTINATION bin
)

# Install configuration files
install(FILES .env.example
    DESTINATION etc/llamaware
    RENAME config.env
)

# CPack configuration for package generation
set(CPACK_PACKAGE_NAME "llamaware-agent")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Professional AI Agent with Command Execution")
set(CPACK_PACKAGE_DESCRIPTION "A powerful, modular AI agent built with modern C++ that provides seamless interaction with language models and comprehensive system operations.")
set(CPACK_PACKAGE_VENDOR "bniladridas")
set(CPACK_PACKAGE_CONTACT "your-email@example.com")

# Platform-specific packaging
if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
elseif(UNIX)
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libcpr1, nlohmann-json3-dev")
    set(CPACK_RPM_PACKAGE_REQUIRES "cpr-devel, json-devel")
elseif(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
endif()

include(CPack)

# Add custom check target for testing and validation
add_custom_target(check
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target llamaware-agent
    COMMAND echo "Running automated tests..."
    COMMAND printf "2\\nversion\\nhelp\\nexit\\n" | ${CMAKE_BINARY_DIR}/bin/llamaware-agent
    COMMAND echo "Checking binary exists..."
    COMMAND test -f ${CMAKE_BINARY_DIR}/bin/llamaware-agent
    COMMAND echo "Validating executable permissions..."
    COMMAND test -x ${CMAKE_BINARY_DIR}/bin/llamaware-agent
    COMMAND echo "All checks passed!"
    DEPENDS llamaware-agent
    COMMENT "Running comprehensive project validation"
)

# Add quick test target
add_custom_target(test-quick
    COMMAND printf "2\\nexit\\n" | ${CMAKE_BINARY_DIR}/bin/llamaware-agent
    DEPENDS llamaware-agent
    COMMENT "Quick functionality test"
)

# Add preflight target (comprehensive pre-deployment checks)
add_custom_target(preflight
    COMMAND ${CMAKE_SOURCE_DIR}/package/scripts/preflight.sh
    DEPENDS llamaware-agent
    COMMENT "Running comprehensive preflight checks"
)
