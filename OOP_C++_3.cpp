#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character: " << character.name << ", HP: " << character.health
            << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }
};


class Weapon {
private:
    std::string name;
    int damage;
    double weight;

public:
    Weapon(const std::string& n, int d, double w)
        : name(n), damage(d), weight(w) {
    }

    int getDamage() const { return damage; }
    std::string getName() const { return name; }
    double getWeight() const { return weight; }

    Weapon operator+(const Weapon& other) const {
        return Weapon(this->name + " + " + other.name, this->damage + other.damage, this->weight + other.weight);
    }

    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
        os << "Weapon: " << weapon.getName() << ", Damage: " << weapon.getDamage()
            << ", Weight: " << weapon.getWeight();
        return os;
    }
};

int main() {
    Character hero1("Hero", 100, 20, 10);
    Character hero2("Hero", 100, 20, 10);
    Character hero3("Warrior", 150, 25, 15);

    if (hero1 == hero2) {
        std::cout << "Hero1 and Hero2 are the same!\n";
    }
    if (!(hero1 == hero3)) {
        std::cout << "Hero1 and Hero3 are different!\n";
    }

    std::cout << hero1 << std::endl;

    Weapon sword("Sword", 20, 2.5);
    Weapon bow("Bow", 15, 1.5);
    Weapon axe("Axe", 30, 4.0);

    Weapon combinedWeapon = sword + bow;
    std::cout << "Combined weapon: " << combinedWeapon << std::endl;

    if (axe > sword) {
        std::cout << "Axe has more damage than Sword!\n";
    }
    else {
        std::cout << "Sword has more damage than Axe!\n";
    }

    return 0;
}
