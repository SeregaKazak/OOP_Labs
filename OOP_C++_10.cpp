#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <cctype>

// Базовый класс пользователя
class User {
protected:
    std::string name;
    int id;
    int accessLevel;

public:
    User(std::string name, int id, int accessLevel)
        : name(std::move(name)), id(id), accessLevel(accessLevel) {
        if (this->name.empty()) throw std::invalid_argument("Имя не может быть пустым");
        if (id < 0) throw std::invalid_argument("ID не может быть отрицательным");
        if (accessLevel < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным");
    }

    virtual ~User() = default;

    // Геттеры
    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    // Сеттеры
    void setName(const std::string& newName) {
        if (newName.empty()) throw std::invalid_argument("Имя не может быть пустым");
        name = newName;
    }

    void setAccessLevel(int level) {
        if (level < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным");
        accessLevel = level;
    }

    virtual void displayInfo() const {
        std::cout << "ID: " << id
            << "\nИмя: " << name
            << "\nУровень доступа: " << accessLevel << "\n";
    }
};

// Производные классы
class Student : public User {
    std::string group;

public:
    Student(std::string name, int id, int accessLevel, std::string group)
        : User(std::move(name), id, accessLevel), group(std::move(group)) {
    }

    void displayInfo() const override {
        std::cout << "СТУДЕНТ\n";
        User::displayInfo();
        std::cout << "Группа: " << group << "\n\n";
    }
};

class Teacher : public User {
    std::string department;

public:
    Teacher(std::string name, int id, int accessLevel, std::string department)
        : User(std::move(name), id, accessLevel), department(std::move(department)) {
    }

    void displayInfo() const override {
        std::cout << "ПРЕПОДАВАТЕЛЬ\n";
        User::displayInfo();
        std::cout << "Кафедра: " << department << "\n\n";
    }
};

class Administrator : public User {
    std::string position;

public:
    Administrator(std::string name, int id, int accessLevel, std::string position)
        : User(std::move(name), id, accessLevel), position(std::move(position)) {
    }

    void displayInfo() const override {
        std::cout << "АДМИНИСТРАТОР\n";
        User::displayInfo();
        std::cout << "Должность: " << position << "\n\n";
    }
};

// Класс ресурса
class Resource {
    std::string name;
    int requiredAccessLevel;

public:
    Resource(std::string name, int requiredLevel)
        : name(std::move(name)), requiredAccessLevel(requiredLevel) {
        if (this->name.empty()) throw std::invalid_argument("Название ресурса не может быть пустым");
        if (requiredLevel < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным");
    }

    std::string getName() const { return name; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }
};

// Шаблонная система контроля доступа
template<typename T>
class AccessControlSystem {
    std::vector<std::unique_ptr<User>> users;
    std::vector<Resource> resources;

public:
    void addUser(std::unique_ptr<User> user) {
        users.push_back(std::move(user));
    }

    void addResource(const Resource& resource) {
        resources.push_back(resource);
    }

    bool checkAccess(int userId, const std::string& resourceName) const {
        auto userIt = std::find_if(users.begin(), users.end(),
            [userId](const auto& user) { return user->getId() == userId; });

        auto resIt = std::find_if(resources.begin(), resources.end(),
            [&resourceName](const auto& res) { return res.getName() == resourceName; });

        if (userIt == users.end() || resIt == resources.end())
            return false;

        return resIt->checkAccess(**userIt);
    }

    void displayAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) throw std::runtime_error("Не удалось открыть файл для записи");

        for (const auto& user : users) {
            file << user->getId() << ","
                << user->getName() << ","
                << user->getAccessLevel() << "\n";
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw std::runtime_error("Не удалось открыть файл для чтения");

        users.clear();
        std::string line;
        while (std::getline(file, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);

            if (pos1 == std::string::npos || pos2 == std::string::npos)
                continue;

            int id = std::stoi(line.substr(0, pos1));
            std::string name = line.substr(pos1 + 1, pos2 - pos1 - 1);
            int accessLevel = std::stoi(line.substr(pos2 + 1));

            // Упрощенное создание пользователей
            users.push_back(std::make_unique<User>(name, id, accessLevel));
        }
    }

    User* findUser(int id) const {
        auto it = std::find_if(users.begin(), users.end(),
            [id](const auto& user) { return user->getId() == id; });
        return it != users.end() ? it->get() : nullptr;
    }

    User* findUser(const std::string& name) const {
        auto it = std::find_if(users.begin(), users.end(),
            [&name](const auto& user) { return user->getName() == name; });
        return it != users.end() ? it->get() : nullptr;
    }

    void sortUsers(bool (*compare)(const User&, const User&)) {
        std::sort(users.begin(), users.end(),
            [compare](const auto& a, const auto& b) { return compare(*a, *b); });
    }
};

// Функции для сортировки
bool compareByAccessLevel(const User& a, const User& b) {
    return a.getAccessLevel() < b.getAccessLevel();
}

bool compareById(const User& a, const User& b) {
    return a.getId() < b.getId();
}

int main() {
    setlocale(LC_CTYPE, "rus");
    AccessControlSystem<void> system;

    try {
        // Создание пользователей
        system.addUser(std::make_unique<Student>("Иван Петров", 1, 1, "CS-101"));
        system.addUser(std::make_unique<Teacher>("Алексей Сидоров", 2, 3, "Информатика"));
        system.addUser(std::make_unique<Administrator>("Мария Иванова", 3, 5, "Директор"));

        // Добавление ресурсов
        system.addResource(Resource("Аудитория 101", 1));
        system.addResource(Resource("Лаборатория", 3));
        system.addResource(Resource("Серверная", 5));

        // Демонстрация полиморфизма
        system.displayAllUsers();

        // Проверка доступа
        std::cout << "Доступ студента в серверную: "
            << (system.checkAccess(1, "Серверная") ? "Да" : "Нет") << "\n";
        std::cout << "Доступ преподавателя в лабораторию: "
            << (system.checkAccess(2, "Лаборатория") ? "Да" : "Нет") << "\n";

        // Работа с файлами
        system.saveToFile("users.txt");
        system.loadFromFile("users.txt");

        // Поиск пользователя
        if (auto user = system.findUser("Алексей Сидоров")) {
            std::cout << "Найден пользователь: " << user->getName() << "\n";
        }

        // Сортировка пользователей
        system.sortUsers(compareByAccessLevel);
        std::cout << "\nПосле сортировки по уровню доступа:\n";
        system.displayAllUsers();

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}