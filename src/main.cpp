#include "UserManager.hpp"

#include <iostream>
#include <string>
#include <limits>

// ── Helpers ───────────────────────────────────────────────────────────────────

static void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string prompt(const std::string& label) {
    std::cout << "  " << label << ": ";
    std::string value;
    std::getline(std::cin, value);
    return value;
}

static std::string promptRole() {
    while (true) {
        std::string role = prompt("Role (admin / user / guest)");
        if (role == "admin" || role == "user" || role == "guest") return role;
        std::cout << "  [error] Role must be 'admin', 'user', or 'guest'.\n";
    }
}

// ── Menu actions ──────────────────────────────────────────────────────────────

static void menuAdd(UserManager& mgr) {
    std::cout << "\n── Add User ─────────────────────────\n";
    std::string name  = prompt("Name");
    std::string email = prompt("Email");
    std::string role  = promptRole();
    mgr.addUser(name, email, role);
}

static void menuDelete(UserManager& mgr) {
    std::cout << "\n── Delete User ──────────────────────\n";
    std::cout << "  User ID: ";
    int id; std::cin >> id; clearInput();

    if (mgr.deleteUser(id)) {
        std::cout << "[ok] User #" << id << " deleted.\n";
    } else {
        std::cout << "[error] No user found with ID " << id << ".\n";
    }
}

static void menuUpdate(UserManager& mgr) {
    std::cout << "\n── Update User ──────────────────────\n";
    std::cout << "  User ID to update: ";
    int id; std::cin >> id; clearInput();

    auto user = mgr.findById(id);
    if (!user) {
        std::cout << "[error] No user found with ID " << id << ".\n";
        return;
    }

    std::cout << "  Leave a field blank to keep current value.\n";
    std::string name  = prompt("Name  [" + user->name  + "]");
    std::string email = prompt("Email [" + user->email + "]");
    std::string role  = prompt("Role  [" + user->role  + "] (admin/user/guest)");

    if (name.empty())  name  = user->name;
    if (email.empty()) email = user->email;
    if (role.empty())  role  = user->role;

    if (role != "admin" && role != "user" && role != "guest") {
        std::cout << "[error] Invalid role.\n";
        return;
    }

    if (mgr.updateUser(id, name, email, role)) {
        std::cout << "[ok] User #" << id << " updated.\n";
    }
}

static void menuSearch(UserManager& mgr) {
    std::cout << "\n── Search ───────────────────────────\n";
    std::cout << "  1) Find by ID\n"
              << "  2) Filter by role\n"
              << "  Choice: ";
    int choice; std::cin >> choice; clearInput();

    if (choice == 1) {
        std::cout << "  User ID: ";
        int id; std::cin >> id; clearInput();
        auto user = mgr.findById(id);
        if (user) {
            mgr.printTable({*user});
        } else {
            std::cout << "  [not found]\n";
        }
    } else if (choice == 2) {
        std::string role = promptRole();
        auto users = mgr.findByRole(role);
        std::cout << "── Users with role '" << role << "' (" << users.size() << ")\n";
        mgr.printTable(users);
    }
}

// ── Main ──────────────────────────────────────────────────────────────────────

int main() {
    const std::string DATA_FILE = "users.json";

    UserManager mgr(DATA_FILE);
    mgr.load();

    std::cout << "====================================\n";
    std::cout << "   User Management Tool  v1.0\n";
    std::cout << "====================================\n";

    while (true) {
        std::cout << "\n"
                  << "  1) List all users\n"
                  << "  2) Add user\n"
                  << "  3) Update user\n"
                  << "  4) Delete user\n"
                  << "  5) Search / filter\n"
                  << "  0) Exit\n"
                  << "\n  Choice: ";

        int choice;
        if (!(std::cin >> choice)) break;
        clearInput();

        switch (choice) {
            case 1: mgr.printAll();     break;
            case 2: menuAdd(mgr);       break;
            case 3: menuUpdate(mgr);    break;
            case 4: menuDelete(mgr);    break;
            case 5: menuSearch(mgr);    break;
            case 0:
                std::cout << "Goodbye!\n";
                return 0;
            default:
                std::cout << "  [error] Invalid choice.\n";
        }
    }

    return 0;
}