cmake_minimum_required(VERSION "3.11")

project(RegistrationAPI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${RegistrationAPI_SOURCE_DIR}/bin")

include(FetchContent)

# Download json and jwt-cpp libraries
FetchContent_Declare(
    jwt-cpp 
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG 08bcf77a687fb06e34138e9e9fa12a4ecbe12332 # v0.7.0 release
)
set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "disable building examples" FORCE)
fetchcontent_makeavailable(jwt-cpp)

# Download libpqxx library
FetchContent_Declare(
  libpqxx
  GIT_REPOSITORY https://github.com/jtv/libpqxx.git
)
set(PQXX_LIBRARIES pqxx)
FetchContent_MakeAvailable(libpqxx)

# Configure file with absolute paths
set(SETTINGS_PATH "\"${CMAKE_CURRENT_SOURCE_DIR}/settings/\"")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/Config.h)

set(SOURCE
    "src/PasswordChecker.cpp"
    "src/Command.cpp"
    "src/Client.cpp"
    "src/Query.cpp"
)

# build
add_executable(main ${RegistrationAPI_SOURCE_DIR}/main.cpp ${SOURCE})

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json pqxx jwt-cpp::jwt-cpp)
target_include_directories(main PRIVATE ${RegistrationAPI_SOURCE_DIR}/include ${RegistrationAPI_SOURCE_DIR}/include/exe)