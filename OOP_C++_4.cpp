#include <iostream>
#include <string>
#include <vector>
#include <memory> 

class Inventory {
private:
    std::vector<std::unique_ptr<std::string>> items; 
    size_t capacity; 

public:
    Inventory() : capacity(10) {}

    void addItem(const std::string& item) {
        items.push_back(std::make_unique<std::string>(item));
    }

    void displayInventory() const {
        std::cout << "Inventory:\n";
        if (items.empty()) {
            std::cout << "  Empty\n";
        }
        else {
            for (size_t i = 0; i < items.size(); ++i) {
                std::cout << "  " << i + 1 << ": " << *items[i] << "\n";
            }
        }
    }
};

int main() {
    Inventory playerInventory;

    playerInventory.addItem("Sword");
    playerInventory.addItem("Health Potion");
    playerInventory.addItem("Shield");
    playerInventory.displayInventory();

    return 0;
}

