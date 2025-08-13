cmake_minimum_required(VERSION 3.14)
project(LlamawareAgent VERSION 2.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

if(MSVC)
    add_compile_options(/W4 /WX /MP /EHsc)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

# Find dependencies
# Find libcurl
find_package(CURL REQUIRED)

# Use Homebrew's OpenSSL on macOS/arm64
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architectures for macOS" FORCE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version" FORCE)
    
    # Use Homebrew's OpenSSL on macOS/arm64
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        # Try to find Homebrew's OpenSSL
        find_program(HOMEBREW NAMES brew)
        if(HOMEBREW)
            execute_process(
                COMMAND ${HOMEBREW} --prefix openssl@3
                OUTPUT_VARIABLE OPENSSL_ROOT_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            set(OPENSSL_ROOT_DIR ${OPENSSL_ROOT_DIR} CACHE PATH "OpenSSL root directory")
            
            # Also find Homebrew's curl if available
            execute_process(
                COMMAND ${HOMEBREW} --prefix curl
                OUTPUT_VARIABLE CURL_ROOT_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            if(EXISTS ${CURL_ROOT_DIR})
                set(CURL_INCLUDE_DIRS ${CURL_ROOT_DIR}/include)
                set(CURL_LIBRARIES ${CURL_ROOT_DIR}/lib/libcurl.dylib)
                message(STATUS "Using Homebrew's curl: ${CURL_ROOT_DIR}")
            endif()
        endif()
    endif()
    
    # Find OpenSSL with the updated path
    find_package(OpenSSL REQUIRED)
    
    message(STATUS "Using OpenSSL: ${OPENSSL_INCLUDE_DIR} : ${OPENSSL_LIBRARIES}")
    
    # Add compiler and linker flags
    add_compile_options(-march=armv8.4-a+fp16+rcpc+dotprod+crypto)
    add_link_options(-Wl,-no_weak_imports)
    
    # Set rpath for macOS
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_MACOSX_RPATH TRUE)
else()
    # On other platforms, use system OpenSSL
    find_package(OpenSSL REQUIRED)
    
    # Ensure we have libcurl
    if(NOT CURL_FOUND)
        find_package(CURL REQUIRED)
    endif()
endif()

# Make sure we have the curl include directory
if(NOT CURL_INCLUDE_DIRS)
    set(CURL_INCLUDE_DIRS ${CURL_INCLUDE_DIR})
endif()

message(STATUS "Using curl: ${CURL_INCLUDE_DIRS} : ${CURL_LIBRARIES}")

# Find nlohmann_json
find_package(nlohmann_json 3.9.1 REQUIRED)

# Fetch CPR with explicit configuration
include(FetchContent)

# Clear any cached CPR variables
unset(CPR_LIBRARIES CACHE)
unset(CPR_INCLUDE_DIRS CACHE)

# Configure CPR
set(CPR_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CPR_BUILD_TESTS_SSL OFF CACHE BOOL "" FORCE)
set(CPR_ENABLE_SSL ON CACHE BOOL "" FORCE)
set(CPR_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(CPR_FORCE_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE)

# Always use OpenSSL on all platforms
set(CPR_USE_OPENSSL ON CACHE BOOL "" FORCE)
set(CPR_USE_OPENSSL_DEFAULT_IMPLEMENTATION ON CACHE BOOL "" FORCE)
set(CPR_USE_OPENSSL_AUTO_IMPLEMENTATION OFF CACHE BOOL "" FORCE)
set(CPR_FORCE_OPENSSL_BACKEND ON CACHE BOOL "" FORCE)
set(CPR_FORCE_DARWINSSL_BACKEND OFF CACHE BOOL "" FORCE)

# Fetch and make available
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(cpr)

# Ensure proper linking on all platforms
target_link_libraries(cpr PRIVATE 
    OpenSSL::SSL 
    OpenSSL::Crypto
    z
)

# On macOS, add additional settings
if(APPLE)
    # Set minimum deployment target
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version" FORCE)
    
    # Set rpath
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_MACOSX_RPATH TRUE)
    
    # Add compiler and linker flags
    target_compile_options(cpr PRIVATE
        -march=armv8.4-a+fp16+rcpc+dotprod+crypto
        -Wno-deprecated-declarations
    )
    
    target_link_options(cpr PRIVATE
        -Wl,-no_weak_imports
    )
    
    # Add these flags to the main target as well
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8.4-a+fp16+rcpc+dotprod+crypto -Wno-deprecated-declarations")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-no_weak_imports")
    
    # Add OpenSSL include directories
    target_include_directories(cpr PRIVATE ${OPENSSL_INCLUDE_DIR})
    
    # Ensure we're using OpenSSL
    target_compile_definitions(cpr PRIVATE
        CURL_USE_OPENSSL=1
        CURL_USE_DARWINSSL=0
        CURL_USE_SECTRANSP=0
    )
endif()

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
    src/services/git_service.cpp
    src/services/codebase_service.cpp
    src/services/multi_file_service.cpp
    src/services/context_service.cpp
    src/services/checkpoint_service.cpp
    src/services/mcp_service.cpp
    src/services/theme_service.cpp
    src/services/auth_service.cpp
    src/services/error_service.cpp
    src/services/sandbox_service.cpp
)

set(UTILS_SOURCES
    src/utils/ui.cpp
    src/utils/config.cpp
    src/utils/version.cpp
    src/utils/validation.cpp
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
    MACOSX_RPATH ON
    INSTALL_RPATH "@executable_path/../lib"
    BUILD_WITH_INSTALL_RPATH FALSE
    SKIP_BUILD_RPATH FALSE
)

# Include directories
target_include_directories(llamaware-agent PRIVATE 
    ${CURL_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)

# Common linking for all platforms
if(TARGET nlohmann_json::nlohmann_json)
    target_link_libraries(llamaware-agent PRIVATE 
        ${CURL_LIBRARIES}
        cpr::cpr 
        nlohmann_json::nlohmann_json
        OpenSSL::SSL
        OpenSSL::Crypto
        z
        ${CMAKE_DL_LIBS}
    )
    
    # On macOS, add additional settings
    if(APPLE)
        target_link_options(llamaware-agent PRIVATE
            -Wl,-rpath,@loader_path/../lib
            -Wl,-rpath,/usr/local/lib
            -Wl,-rpath,/opt/homebrew/lib
        )
    endif()
else()
    target_include_directories(llamaware-agent PRIVATE ${nlohmann_json_INCLUDE_DIRS})
    target_link_libraries(llamaware-agent PRIVATE 
        cpr::cpr 
        ${OPENSSL_LIBRARIES}
        z
    )
    target_include_directories(llamaware-agent PRIVATE ${OPENSSL_INCLUDE_DIR})
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
set(CPACK_PACKAGE_CONTACT "bniladridas@gmail.com")

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
