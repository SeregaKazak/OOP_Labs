#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <memory>

// Вспомогательная функция для разделения строки
void split(const std::string& s, char delim, std::vector<std::string>& parts) {
    parts.clear();
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        parts.push_back(item);
    }
}

// Базовый класс Entity
class Entity {
public:
    virtual ~Entity() = default;
    virtual std::string getType() const = 0;
    virtual std::string getName() const = 0;
    virtual int getHealth() const = 0;
    virtual int getLevel() const = 0;
};

// Класс Player
class Player : public Entity {
public:
    Player(std::string name, int health, int level)
        : name_(std::move(name)), health_(health), level_(level) {
    }

    std::string getType() const override { return "Player"; }
    std::string getName() const override { return name_; }
    int getHealth() const override { return health_; }
    int getLevel() const override { return level_; }

private:
    std::string name_;
    int health_;
    int level_;
};

// Шаблонный класс GameManager
template<typename T>
class GameManager {
public:
    void addEntity(T entity) {
        entities_.push_back(entity);
    }

    const std::vector<T>& getEntities() const {
        return entities_;
    }

    void displayAll() const {
        for (const auto& entity : entities_) {
            std::cout << "Type: " << entity->getType()
                << ", Name: " << entity->getName()
                << ", Health: " << entity->getHealth()
                << ", Level: " << entity->getLevel() << std::endl;
        }
    }

    ~GameManager() {
        for (auto* entity : entities_) {
            delete entity;
        }
    }

private:
    std::vector<T> entities_;
};

// Функция сохранения в файл
void saveToFile(const GameManager<Entity*>& manager, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing.");
    }
    for (const auto& entity : manager.getEntities()) {
        file << entity->getType() << ","
            << entity->getName() << ","
            << entity->getHealth() << ","
            << entity->getLevel() << "\n";
    }
}

// Функция загрузки из файла
void loadFromFile(GameManager<Entity*>& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading.");
    }
    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> parts;
        split(line, ',', parts);
        if (parts.size() != 4) {
            std::cerr << "Invalid line format: " << line << std::endl;
            continue;
        }
        try {
            std::string type = parts[0];
            std::string name = parts[1];
            int health = std::stoi(parts[2]);
            int level = std::stoi(parts[3]);
            if (type == "Player") {
                manager.addEntity(new Player(name, health, level));
            }
            else {
                std::cerr << "Unknown entity type: " << type << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing line: " << e.what() << std::endl;
        }
    }
}

int main() {
    GameManager<Entity*> manager;
    manager.addEntity(new Player("Hero", 100, 0));
    manager.addEntity(new Player("Warrior", 150, 1));

    try {
        saveToFile(manager, "game_save.txt");
    }
    catch (const std::exception& e) {
        std::cerr << "Save failed: " << e.what() << std::endl;
    }

    GameManager<Entity*> loadedManager;
    try {
        loadFromFile(loadedManager, "game_save.txt");
    }
    catch (const std::exception& e) {
        std::cerr << "Load failed: " << e.what() << std::endl;
    }

    loadedManager.displayAll();

    return 0;
}