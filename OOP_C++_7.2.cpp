#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;
public:
    Character(std::string n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    void takeDamage(int damage) {
        int actualDamage = std::max(damage - defense, 0);
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    bool isAlive() const { return health > 0; }
    void displayInfo() const {
        std::cout << name << " Health: " << health << std::endl;
    }
    int getAttack() const { return attack; }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;
public:
    Monster(std::string n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    void takeDamage(int damage) {
        int actualDamage = std::max(damage - defense, 0);
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    bool isAlive() const { return health > 0; }
    void displayInfo() const {
        std::cout << name << " Health: " << health << std::endl;
    }
    int getAttack() const { return attack; }
};

// Глобальные объекты и мьютекс
Character* hero = nullptr;
Monster* enemy = nullptr;
std::mutex fightMutex;

void fight() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(fightMutex);
        if (!hero->isAlive() || !enemy->isAlive()) break;

        // Атака персонажа
        enemy->takeDamage(hero->getAttack());
        std::cout << "Hero attacks!" << std::endl;
        if (!enemy->isAlive()) break;

        // Атака монстра
        hero->takeDamage(enemy->getAttack());
        std::cout << "Monster counterattacks!" << std::endl;
    }
}

int main() {
    hero = new Character("Hero", 100, 20, 10);
    enemy = new Monster("Goblin", 50, 15, 5);

    std::thread fightThread(fight);
    fightThread.join();

    std::lock_guard<std::mutex> lock(fightMutex);
    std::cout << "\nBattle result:" << std::endl;
    hero->displayInfo();
    enemy->displayInfo();

    delete hero;
    delete enemy;
    return 0;
}