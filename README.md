# User Management Tool

A simple CLI-based user management system built with **C++17** and **CMake**.  
Uses `nlohmann/json` (pulled automatically via `FetchContent`) to persist users in a local JSON file.

---

## Features

- List all users in a formatted table
- Add a user (name, email, role)
- Update an existing user by ID
- Delete a user by ID
- Search by ID or filter by role (`admin` / `user` / `guest`)
- All data is saved automatically to `users.json`

---

## Project Structure

```
user-management/
├── CMakeLists.txt          # Build config + FetchContent
├── include/
│   └── UserManager.h       # User struct + UserManager class
├── src/
│   ├── UserManager.cpp     # Implementation
│   └── main.cpp            # Interactive menu
└── README.md
```

---

## Build & Run

### Requirements
- CMake 3.16+
- A C++17 compiler (GCC, Clang, or MSVC)
- Internet connection (first build only — to fetch nlohmann/json)

### Steps

```bash
# 1. Create a build directory
mkdir build && cd build

# 2. Configure
cmake ..

# 3. Build
cmake --build .

# 4. Run
./user_management      # Linux / macOS
user_management.exe    # Windows
```

---

## Data File

Users are stored in `users.json` in the directory where you run the program:

```json
{
    "users": [
        {
            "id": 1,
            "name": "Alice Smith",
            "email": "alice@example.com",
            "role": "admin"
        }
    ]
}
```

---

## CMake FetchContent (Key Learning Point)

Both `nlohmann/json` and `tabulate` are fetched automatically — no manual installation needed:

```cmake
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
```

### Table display (tabulate)
Users are displayed in a styled, color-coded table:
- 🔴 **admin** — red
- 🟢 **user** — green
- 🟡 **guest** — yellow