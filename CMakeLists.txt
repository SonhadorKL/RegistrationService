cmake_minimum_required(VERSION "3.11")

project(RegistrationAPI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${RegistrationAPI_SOURCE_DIR}/bin")

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
    )
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  libpqxx
  GIT_REPOSITORY https://github.com/jtv/libpqxx.git
)
set(PQXX_LIBRARIES pqxx)
FetchContent_MakeAvailable(libpqxx)

set(SETTINGS_PATH "\"${CMAKE_CURRENT_SOURCE_DIR}/settings/\"")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Config.h)

find_package(jwt-cpp CONFIG REQUIRED)

set(SOURCE
    "src/PasswordChecker.cpp"
    "src/ArgParser.cpp"
    "src/Client.cpp"
    "src/Query.cpp"
)

add_executable(main ${RegistrationAPI_SOURCE_DIR}/main.cpp ${SOURCE})

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json pqxx jwt-cpp::jwt-cpp)
target_include_directories(main PRIVATE ${RegistrationAPI_SOURCE_DIR}/include ${RegistrationAPI_SOURCE_DIR}/include/exe)