#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <ctime>
#include <iomanip>

// Предварительное объявление класса Character
class Character;

// Шаблонный класс Logger
template <typename T>
class Logger {
public:
    Logger(const std::string& filename) : logFile(filename, std::ios::app) {}

    void log(const T& message) {
        if (logFile.is_open()) {
            time_t now = time(0);
            struct tm timeinfo;
            localtime_s(&timeinfo, &now);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
            logFile << "[" << buffer << "] " << message << std::endl;
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

private:
    std::ofstream logFile;
};

// Класс Inventory
class Inventory {
private:
    std::vector<std::string> items;

public:
    void addItem(const std::string& item) {
        items.push_back(item);
    }

    void removeItem(const std::string& item) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (*it == item) {
                items.erase(it);
                return;
            }
        }
    }

    void display() const {
        std::cout << "Inventory: ";
        for (const auto& item : items) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    const std::vector<std::string>& getItems() const {
        return items;
    }
};

// Базовый класс Monster
class Monster {
protected:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    virtual ~Monster() = default;

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    bool isAlive() const {
        return health > 0;
    }

    virtual void attackTarget(Character& target) = 0;

    virtual void displayInfo() const {
        std::cout << name << " | HP: " << health
            << " | ATK: " << attack
            << " | DEF: " << defense << std::endl;
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
};

// Класс Character
class Character {
private:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int level;
    int experience;
    Inventory inventory;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), maxHealth(h), attack(a), defense(d), level(1), experience(0) {
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) {
            health = 0;
            throw std::runtime_error(name + " has been defeated!");
        }
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        std::cout << name << " healed for " << amount << " HP!" << std::endl;
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {
            level++;
            experience -= 100;
            attack += 2;
            defense += 1;
            maxHealth += 10;
            health = maxHealth;
            std::cout << name << " leveled up to level " << level << "!" << std::endl;
        }
    }

    void useHealthPotion() {
        // Проверим, есть ли зелье в инвентаре
        bool hasPotion = false;
        for (const auto& item : inventory.getItems()) {
            if (item == "Health Potion") {
                hasPotion = true;
                break;
            }
        }
        if (hasPotion) {
            heal(20);
            inventory.removeItem("Health Potion");
        }
        else {
            std::cout << "No health potions available!" << std::endl;
        }
    }

    void addToInventory(const std::string& item) {
        inventory.addItem(item);
    }

    void displayInfo() const {
        std::cout << "Name: " << name
            << " | HP: " << health << "/" << maxHealth
            << " | ATK: " << attack
            << " | DEF: " << defense
            << " | LVL: " << level
            << " | EXP: " << experience << std::endl;
        inventory.display();
    }

    void attackMonster(Monster& monster) {
        int damage = attack - monster.getDefense();
        if (damage > 0) {
            monster.takeDamage(damage);
            std::cout << name << " attacks " << monster.getName() << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks but it's ineffective against " << monster.getName() << "!" << std::endl;
        }
    }

    // Геттеры
    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getDefense() const { return defense; }
    const Inventory& getInventory() const { return inventory; }

    // Методы для сохранения/загрузки
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << name << "\n"
                << health << "\n"
                << maxHealth << "\n"
                << attack << "\n"
                << defense << "\n"
                << level << "\n"
                << experience << "\n";
            for (const auto& item : inventory.getItems()) {
                file << item << "\n";
            }
            file.close();
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            std::getline(file, name);
            file >> health >> maxHealth >> attack >> defense >> level >> experience;
            file.ignore(); // Пропустить символ новой строки после чисел

            inventory = Inventory(); // Очищаем инвентарь
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    inventory.addItem(line);
                }
            }
            file.close();
        }
    }
};

// Производные классы монстров (реализация после определения Character)
class Goblin : public Monster {
public:
    Goblin() : Monster("Goblin", 30, 8, 2) {}

    void attackTarget(Character& target) override {
        int damage = attack - target.getDefense();
        if (damage > 0) {
            target.takeDamage(damage);
            std::cout << name << " attacks for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks but it's ineffective!" << std::endl;
        }
    }
};

class Dragon : public Monster {
public:
    Dragon() : Monster("Dragon", 100, 20, 10) {}

    void attackTarget(Character& target) override {
        int damage = attack - target.getDefense();
        if (damage > 0) {
            target.takeDamage(damage);
            std::cout << name << " breathes fire for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << "'s fire has no effect!" << std::endl;
        }
    }
};

class Skeleton : public Monster {
public:
    Skeleton() : Monster("Skeleton", 40, 10, 5) {}

    void attackTarget(Character& target) override {
        int damage = attack - target.getDefense();
        if (damage > 0) {
            target.takeDamage(damage);
            std::cout << name << " shoots arrows for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << "'s arrows bounce off!" << std::endl;
        }
    }
};

// Класс Game
class Game {
private:
    Character player;
    Logger<std::string> logger;
    std::vector<std::unique_ptr<Monster>> monsters;

public:
    Game(const std::string& playerName)
        : player(playerName, 100, 15, 5), logger("game_log.txt") {
        monsters.push_back(std::make_unique<Goblin>());
        monsters.push_back(std::make_unique<Skeleton>());
        monsters.push_back(std::make_unique<Dragon>());
    }

    void startBattle() {
        for (auto& monster : monsters) {
            if (!monster->isAlive()) continue;

            std::cout << "A wild " << monster->getName() << " appears!" << std::endl;

            while (player.getHealth() > 0 && monster->isAlive()) {
                player.displayInfo();
                monster->displayInfo();

                std::cout << "\n1. Attack\n2. Use Health Potion\n> ";
                int choice;
                std::cin >> choice;

                try {
                    if (choice == 1) {
                        player.attackMonster(*monster);
                        logger.log(player.getName() + " attacked " + monster->getName());
                    }
                    else if (choice == 2) {
                        player.useHealthPotion();
                        logger.log(player.getName() + " used health potion");
                    }
                    else {
                        std::cout << "Invalid choice! Skipping turn." << std::endl;
                        logger.log(player.getName() + " made an invalid choice");
                    }

                    if (monster->isAlive()) {
                        monster->attackTarget(player);
                        logger.log(monster->getName() + " attacked " + player.getName());
                    }
                }
                catch (const std::exception& e) {
                    logger.log(e.what());
                    std::cerr << e.what() << std::endl;
                    return;
                }
            }

            if (player.getHealth() <= 0) {
                std::cout << "You have been defeated!" << std::endl;
                break;
            }

            if (!monster->isAlive()) {
                std::cout << "You defeated " << monster->getName() << "!" << std::endl;
                player.gainExperience(30);
                player.addToInventory("Health Potion");
                logger.log(player.getName() + " defeated " + monster->getName());
            }
        }

        if (player.getHealth() > 0) {
            std::cout << "Congratulations! You defeated all monsters!" << std::endl;
        }
    }

    void saveGame(const std::string& filename) {
        player.saveToFile(filename);
        logger.log("Game saved to " + filename);
    }

    void loadGame(const std::string& filename) {
        player.loadFromFile(filename);
        logger.log("Game loaded from " + filename);
    }
};

int main() {
    Game game("Hero");

    try {
        game.startBattle();
        game.saveGame("save.txt");
    }
    catch (const std::exception& e) {
        std::cerr << "Game over: " << e.what() << std::endl;
    }

    return 0;
}