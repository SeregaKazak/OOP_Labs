#include <iostream>
#include <vector>
#include <string>

template <typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    void addEntity(T entity) {
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }
};

class Entity {
public:
    virtual void displayInfo() const = 0;
};

class Player : public Entity {
private:
    std::string name;
    int health;
    int experience;

public:
    Player(std::string name, int health, int experience) : name(name), health(health), experience(experience) {}

    void displayInfo() const override {
        std::cout << "Player: " << name << ", Health: " << health << ", Exp: " << experience << std::endl;
    }
};

class Enemy : public Entity {
private:
    std::string name;
    int health;
    std::string type;

public:
    Enemy(std::string name, int health, std::string type) : name(name), health(health), type(type) {}

    void displayInfo() const override {
        std::cout << "Enemy: " << name << " (" << type << "), Health: " << health << std::endl;
    }
};

template <typename T>
class Queue {
private:
    std::vector<T> data;

public:
    void push(const T& item) {
        data.push_back(item);
    }

    T pop() {
        if (data.empty()) {
            throw std::out_of_range("Queue is empty");
        }
        T item = data.front();
        data.erase(data.begin());
        return item;
    }

    bool isEmpty() const {
        return data.empty();
    }
};

int main() {
    GameManager<Entity*> manager;
    manager.addEntity(new Player("Hero", 100, 0));
    manager.addEntity(new Enemy("Goblin", 50, "Goblin"));
    manager.displayAll();

    Queue<std::string> stringQueue;
    stringQueue.push("Hello");
    stringQueue.push("World");
    try {
        std::cout << "String Queue: " << stringQueue.pop() << " " << stringQueue.pop() << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }


    Queue<int> intQueue;
    intQueue.push(1);
    intQueue.push(2);
    intQueue.push(3);
    try {
        std::cout << "Int Queue: " << intQueue.pop() << " " << intQueue.pop() << " " << intQueue.pop() << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }



    return 0;
}

