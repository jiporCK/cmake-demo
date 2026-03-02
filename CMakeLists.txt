cmake_minimum_required(VERSION 3.16)
project(UserManagement VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Fetch dependencies ────────────────────────────────────────────────────────
include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
)

FetchContent_Declare(
  tabulate
  GIT_REPOSITORY https://github.com/p-ranav/tabulate.git
  GIT_TAG        v1.5
)

FetchContent_MakeAvailable(nlohmann_json tabulate)

# ── Executable ───────────────────────────────────────────────────────────────
add_executable(UserManagement
  src/main.cpp
  src/UserManager.cpp
)

target_include_directories(UserManagement PRIVATE include)

target_link_libraries(UserManagement PRIVATE
  nlohmann_json::nlohmann_json
  tabulate::tabulate
)