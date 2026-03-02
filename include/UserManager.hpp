#pragma once

#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>
#include <tabulate/table.hpp>

// ── User model ───────────────────────────────────────────────────────────────

struct User {
    int         id;
    std::string name;
    std::string email;
    std::string role;   // "admin" | "user" | "guest"

    // Serialization helpers
    static User from_json(const nlohmann::json& j);
    nlohmann::json to_json() const;
};

// ── UserManager ──────────────────────────────────────────────────────────────

class UserManager {
public:
    explicit UserManager(const std::string& filepath);

    // CRUD
    void                    addUser(const std::string& name,
                                    const std::string& email,
                                    const std::string& role);
    bool                    deleteUser(int id);
    bool                    updateUser(int id,
                                       const std::string& name,
                                       const std::string& email,
                                       const std::string& role);

    // Queries
    std::optional<User>     findById(int id)               const;
    std::vector<User>       findByRole(const std::string& role) const;
    const std::vector<User>& allUsers()                    const;

    // Persistence
    void load();
    void save() const;

    // Display
    void printTable(const std::vector<User>& users) const;
    void printAll()                                 const;

private:
    std::string       m_filepath;
    std::vector<User> m_users;
    int               m_nextId{1};

    void recalcNextId();
};