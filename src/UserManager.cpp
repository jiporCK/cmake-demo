#include "UserManager.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <tabulate/table.hpp>

using json = nlohmann::json;
using namespace tabulate;

// ── User serialization ────────────────────────────────────────────────────────

User User::from_json(const json& j) {
    return User{
        j.at("id").get<int>(),
        j.at("name").get<std::string>(),
        j.at("email").get<std::string>(),
        j.at("role").get<std::string>()
    };
}

json User::to_json() const {
    return json{
        {"id",    id},
        {"name",  name},
        {"email", email},
        {"role",  role}
    };
}

// ── UserManager ───────────────────────────────────────────────────────────────

UserManager::UserManager(const std::string& filepath)
    : m_filepath(filepath) {}

// ── Persistence ───────────────────────────────────────────────────────────────

void UserManager::load() {
    std::ifstream file(m_filepath);
    if (!file.is_open()) {
        // No file yet — start fresh
        return;
    }

    try {
        json data = json::parse(file);
        m_users.clear();
        for (const auto& item : data["users"]) {
            m_users.push_back(User::from_json(item));
        }
        recalcNextId();
    } catch (const json::exception& e) {
        std::cerr << "[warning] Could not parse users file: " << e.what() << "\n";
    }
}

void UserManager::save() const {
    json data;
    data["users"] = json::array();
    for (const auto& u : m_users) {
        data["users"].push_back(u.to_json());
    }

    std::ofstream file(m_filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + m_filepath);
    }
    file << data.dump(4);   // pretty-print with 4-space indent
}

void UserManager::recalcNextId() {
    m_nextId = 1;
    for (const auto& u : m_users) {
        if (u.id >= m_nextId) m_nextId = u.id + 1;
    }
}

// ── CRUD ──────────────────────────────────────────────────────────────────────

void UserManager::addUser(const std::string& name,
                          const std::string& email,
                          const std::string& role) {
    // Duplicate email check
    for (const auto& u : m_users) {
        if (u.email == email) {
            std::cout << "[error] A user with email '" << email << "' already exists.\n";
            return;
        }
    }

    m_users.push_back(User{m_nextId++, name, email, role});
    save();
    std::cout << "[ok] User '" << name << "' added successfully.\n";
}

bool UserManager::deleteUser(int id) {
    auto it = std::find_if(m_users.begin(), m_users.end(),
                           [id](const User& u){ return u.id == id; });
    if (it == m_users.end()) return false;

    m_users.erase(it);
    save();
    return true;
}

bool UserManager::updateUser(int id,
                             const std::string& name,
                             const std::string& email,
                             const std::string& role) {
    for (auto& u : m_users) {
        if (u.id == id) {
            u.name  = name;
            u.email = email;
            u.role  = role;
            save();
            return true;
        }
    }
    return false;
}

// ── Queries ───────────────────────────────────────────────────────────────────

std::optional<User> UserManager::findById(int id) const {
    for (const auto& u : m_users) {
        if (u.id == id) return u;
    }
    return std::nullopt;
}

std::vector<User> UserManager::findByRole(const std::string& role) const {
    std::vector<User> result;
    for (const auto& u : m_users) {
        if (u.role == role) result.push_back(u);
    }
    return result;
}

const std::vector<User>& UserManager::allUsers() const {
    return m_users;
}

// ── Display ───────────────────────────────────────────────────────────────────

void UserManager::printTable(const std::vector<User>& users) const {
    if (users.empty()) {
        std::cout << "  (no users found)\n";
        return;
    }

    Table table;

    // Header row
    table.add_row({"ID", "Name", "Email", "Role"});

    // Style the header
    table[0].format()
        .font_style({FontStyle::bold})
        .font_color(Color::cyan)
        .border_top("-")
        .border_bottom("-")
        .border_left("|")
        .border_right("|")
        .corner("+");

    // Data rows
    for (const auto& u : users) {
        table.add_row({
            std::to_string(u.id),
            u.name,
            u.email,
            u.role
        });

        // Color-code rows by role
        size_t row = table.size() - 1;
        Color rowColor = Color::white;
        if (u.role == "admin")  rowColor = Color::red;
        if (u.role == "user")   rowColor = Color::green;
        if (u.role == "guest")  rowColor = Color::yellow;

        table[row].format().font_color(rowColor);
    }

    // Column widths
    table.column(0).format().width(6);
    table.column(1).format().width(20);
    table.column(2).format().width(30);
    table.column(3).format().width(10);

    std::cout << "\n" << table << "\n";
}

void UserManager::printAll() const {
    std::cout << "\n── All Users (" << m_users.size() << ")\n";
    printTable(m_users);
}