cmake_minimum_required(VERSION 3.14)
project(LlamawareAgent VERSION 0.1 LANGUAGES CXX)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

if(MSVC)
    add_compile_options(/W4 /WX /MP /EHsc)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:DebugDLL>")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
    endif()
endif()

# Clang-Tidy configuration
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
    message(STATUS "Clang-Tidy found: ${CLANG_TIDY_EXE}")
 else()
     message(STATUS "Clang-Tidy not found. Please install it via your system's package manager (e.g., 'brew install llvm' or 'apt install clang-tidy').")
 endif()

# Find dependencies
# Find zlib with platform-specific logic
if(APPLE)
    # On macOS, prefer pkg-config as it's more reliable with Homebrew
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ZLIB REQUIRED IMPORTED_TARGET zlib)
    message(STATUS "Found zlib using pkg-config: ${ZLIB_LINK_LIBRARIES}")
    set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
    set(ZLIB_LIBRARIES PkgConfig::ZLIB)
    set(ZLIB_TARGET PkgConfig::ZLIB)
elseif(WIN32)
    # On Windows, first try to find zlib via vcpkg
    find_package(ZLIB REQUIRED)
    
    if(ZLIB_FOUND)
        message(STATUS "Found zlib via vcpkg: ${ZLIB_LIBRARIES}")
        # Create an imported target for zlib if it doesn't exist
        if(NOT TARGET zlib_imported)
            add_library(zlib_imported INTERFACE IMPORTED)
            target_link_libraries(zlib_imported INTERFACE ${ZLIB_LIBRARIES})
            target_include_directories(zlib_imported INTERFACE ${ZLIB_INCLUDE_DIRS})
        endif()
        set(ZLIB_TARGET zlib_imported)
    else()
        message(FATAL_ERROR "zlib not found. Please install it using vcpkg: vcpkg install zlib:x64-windows")
    endif()
else()
    # Linux/other Unix-like systems
    find_package(ZLIB REQUIRED)
    if(ZLIB_FOUND)
        set(ZLIB_TARGET ZLIB::ZLIB)
    else()
        find_package(PkgConfig)
        if(PKG_CONFIG_FOUND)
            pkg_check_modules(ZLIB REQUIRED IMPORTED_TARGET zlib)
            set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
            set(ZLIB_TARGET PkgConfig::ZLIB)
        else()
            message(FATAL_ERROR "zlib not found and pkg-config not available")
        endif()
    endif()
endif()

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
    
    # On Windows, rely on vcpkg for zlib
    if(WIN32)
        find_package(ZLIB REQUIRED)
        message(STATUS "Using zlib: ${ZLIB_LIBRARIES}")
    endif()
endif()

# Make sure we have the curl include directory
if(NOT CURL_INCLUDE_DIRS)
    set(CURL_INCLUDE_DIRS ${CURL_INCLUDE_DIR})
endif()

message(STATUS "Using curl: ${CURL_INCLUDE_DIRS} : ${CURL_LIBRARIES}")

# Find nlohmann_json
find_package(nlohmann_json 3.9.1 REQUIRED)

# Find CPR - using 1.12.0 which is installed on the system
find_package(cpr 1.12.0 REQUIRED)

if(NOT TARGET cpr::cpr)
    # Try to find the installed CPR package
    find_package(cpr 1.12.0 QUIET)
    
    if(TARGET cpr::cpr)
        # Use the found target
        message(STATUS "Using system CPR target")
    else()
        # Fallback: Create our own target
        message(STATUS "Creating custom CPR target")
        add_library(cpr::cpr INTERFACE IMPORTED)
        find_library(CPR_LIBRARY cpr)
        if(CPR_LIBRARY)
            message(STATUS "Found CPR library: ${CPR_LIBRARY}")
            target_link_libraries(cpr::cpr INTERFACE ${CPR_LIBRARY} CURL::libcurl OpenSSL::SSL OpenSSL::Crypto ${ZLIB_TARGET})
            target_include_directories(cpr::cpr INTERFACE /opt/homebrew/include)
        else()
            message(FATAL_ERROR "Could not find CPR library. Please install it via 'brew install cpr' or set CPR_DIR to the directory containing cpr-config.cmake")
        endif()
    endif()
endif()

# Enable testing
enable_testing()

# Fetch Google Test
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Only fetch and build gtest if not already found
if(NOT TARGET gtest)
    message(STATUS "Downloading and building Google Test...")
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
        GIT_SHALLOW TRUE
    )
    
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    
    # Make sure we're not overriding compiler flags
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
    set(BUILD_GTEST ON CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(googletest)
    
    # Create an interface library for gtest to make it easier to link
    if(NOT TARGET gtest_all_tests)
        add_library(gtest_all_tests INTERFACE)
        target_link_libraries(gtest_all_tests
            INTERFACE
                GTest::GTest
                GTest::Main
        )
    endif()
endif()

# Only set up the cpr target if we're building it ourselves
if(TARGET cpr)
    # Ensure proper linking on all platforms for our local cpr build
    target_link_libraries(cpr PRIVATE 
        OpenSSL::SSL 
        OpenSSL::Crypto
        ${ZLIB_TARGET}  # Use the appropriate zlib target for this platform
    )
endif()

# On macOS, add additional settings
if(APPLE)
    # Set minimum deployment target
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version" FORCE)
    
    # Set rpath
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_MACOSX_RPATH TRUE)
    
    # Add compiler and linker flags to all targets
    add_compile_options(
        -march=armv8.4-a+fp16+rcpc+dotprod+crypto
        -Wno-deprecated-declarations
    )
    
    add_link_options(
        -Wl,-no_weak_imports
    )
    
    # Add these flags to the main target as well
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8.4-a+fp16+rcpc+dotprod+crypto -Wno-deprecated-declarations")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-no_weak_imports")
    
    # Add OpenSSL include directories to all targets
    include_directories(${OPENSSL_INCLUDE_DIR})
    
    # Add OpenSSL definitions to all targets
    add_compile_definitions(
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

# Create library with all sources
add_library(llamaware_lib ${ALL_SOURCES})
target_include_directories(llamaware_lib PUBLIC 
    include
    ${CURL_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
)
target_link_libraries(llamaware_lib PUBLIC 
    ${CURL_LIBRARIES}
    cpr::cpr 
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    OpenSSL::Crypto
    ${ZLIB_TARGET}
    ${CMAKE_DL_LIBS}
)

# Test executable
add_executable(llamaware-tests tests/main_test.cpp)
target_include_directories(llamaware-tests PRIVATE include)
target_link_libraries(llamaware-tests llamaware_lib gtest_main)
add_test(NAME basic_tests COMMAND llamaware-tests)

# Main executable
add_executable(llamaware-agent src/main.cpp)
target_link_libraries(llamaware-agent llamaware_lib)

# Set target properties
set_target_properties(llamaware-agent PROPERTIES
    OUTPUT_NAME "llamaware-agent"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    MACOSX_RPATH ON
    INSTALL_RPATH "@executable_path/../lib"
    BUILD_WITH_INSTALL_RPATH FALSE
    SKIP_BUILD_RPATH FALSE
)

# On macOS, add additional settings
if(APPLE)
    target_link_options(llamaware-agent PRIVATE
        -Wl,-rpath,@loader_path/../lib
        -Wl,-rpath,/usr/local/lib
        -Wl,-rpath,/opt/homebrew/lib
    )
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

# Clang-Tidy target
if(CLANG_TIDY_EXE)
    add_custom_target(clang-tidy
        COMMAND ${CLANG_TIDY_EXE} --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy -p ${CMAKE_BINARY_DIR} ${ALL_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy on all source files"
    )
endif()

# Code coverage target (requires lcov)
find_program(LCOV_EXE lcov)
find_program(GENHTML_EXE genhtml)
if(LCOV_EXE)
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
        COMMAND ctest --output-on-failure
        COMMAND ${LCOV_EXE} --capture --directory ${CMAKE_BINARY_DIR} --output-file ${CMAKE_BINARY_DIR}/coverage/coverage.info --exclude '/usr/*' --exclude '*/tests/*' --exclude '*/_deps/*'
        COMMAND ${LCOV_EXE} --list ${CMAKE_BINARY_DIR}/coverage/coverage.info
        COMMAND ${GENHTML_EXE} ${CMAKE_BINARY_DIR}/coverage/coverage.info --output-directory ${CMAKE_BINARY_DIR}/coverage/html
        DEPENDS llamaware-tests
        COMMENT "Running tests and generating code coverage report"
    )
endif()

# Add preflight target (comprehensive pre-deployment checks)
add_custom_target(preflight
    COMMAND ${CMAKE_SOURCE_DIR}/package/scripts/preflight.sh
    DEPENDS llamaware-agent
    COMMENT "Running comprehensive preflight checks"
)
