#include <iostream>
#include <string>

int main() {
    std::string input;
    while (std::getline(std::cin, input)) {
        if (input == "Kalaganov Kirill I993")
            std::cout << "Test pass!" << std::endl;
        else
            std::cout << "Test denied" << std::endl;
    }
    return 0;
}
