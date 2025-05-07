#include <iostream>
#include <string>

class Person {
private:
    std::string name;
    int age;
    std::string email;
    std::string address; // Добавленное поле

public:
    // Геттеры
    std::string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    std::string getEmail() const {
        return email;
    }

    std::string getAddress() const { // Новый геттер для address
        return address;
    }

    // Сеттеры
    void setName(const std::string& newName) {
        if (!newName.empty()) {
            name = newName;
        }
        else {
            std::cerr << "Error: Name cannot be empty!" << std::endl;
        }
    }

    void setAge(int newAge) {
        if (newAge >= 0 && newAge <= 120) {
            age = newAge;
        }
        else {
            std::cerr << "Error: Age must be between 0 and 120!" << std::endl;
        }
    }

    void setEmail(const std::string& newEmail) {
        if (newEmail.find('@') != std::string::npos) {
            email = newEmail;
        }
        else {
            std::cerr << "Error: Invalid email format!" << std::endl;
        }
    }

    void setAddress(const std::string& newAddress) { // Новый сеттер для address
        if (!newAddress.empty()) {
            address = newAddress;
        }
        else {
            std::cerr << "Error: Address cannot be empty!" << std::endl;
        }
    }

    // Обновлённый метод для вывода информации
    void displayInfo() const {
        std::cout << "Name: " << name << ", Age: " << age
            << ", Email: " << email << ", Address: " << address << std::endl;
    }
};

int main() {
    Person person;

    // Установка корректных значений
    person.setName("Kazak Sergey");
    person.setAge(18);
    person.setEmail("Kazak@prime.com");
    person.setAddress("Kazacskaja 104"); // Устанавливаем адрес

    // Вывод информации через геттеры
    std::cout << "Name: " << person.getName() << std::endl;
    std::cout << "Age: " << person.getAge() << std::endl;
    std::cout << "Email: " << person.getEmail() << std::endl;
    std::cout << "Address: " << person.getAddress() << std::endl;

    // Попытка установки некорректных значений
    person.setName("");
    person.setAge(150);
    person.setEmail("invalid-email");
    person.setAddress(""); // Пустой адрес

    // Вывод полной информации
    person.displayInfo();

    return 0;
}