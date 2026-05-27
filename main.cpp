//
// Created by Charly CATIN--RICO on 27/05/2026.
//
#include <iostream>
#include <vector>

// fonction add
void addTask(std::vector<std::string> &nl) {

    std::string task;
    std::cout << "Enter task : " << std::endl;
    std::getline(std::cin, task);
    nl.push_back(task);

}



// fonction supp
void delTask(std::vector<std::string> &nl) {

    // check size
    if (nl.empty()) {
        std::cout << "No tasks" << std::endl;
        return;
    }

    try {
        std::string s;
        std::cout << "Enter index of the task you want to delete : " << std::endl;
        std::getline(std::cin, s);
        const int n = stoi(s);
        nl.erase(nl.begin() + n);   // utilise le begin car il faut un iterateur de position et pas un simple nombre
    }

    catch (const std::invalid_argument& error) {
        std::cout << "must be a number between 0 and " << nl.size() << std::endl;
    }

}


// fonction lire tout
void seeTasks( std::vector<std::string> nl) {
    for (size_t i = 0; i < nl.size(); i++) {
        std::cout << i << " - " << nl[i] << std::endl;
    }

}



int main() {

    std::vector<std::string> nameList = {};     // liste des taches à réaliser
    std::string userInput;

    // boucle principale
    while (true) {
        std::cout << "\n1 : see tasks \n2 : add task \n3 : del task \nq : quit" << std::endl;
        std::getline(std::cin, userInput);

        if (userInput == "1") {
            // see tasks function
            seeTasks(nameList);
        }

        else if (userInput == "2") {
            // add task function
            addTask(nameList);
        }

        else if (userInput == "3") {
            // del task function
            delTask(nameList);
        }

        else if (userInput == "q") {
            break;
        }
    }

    return 0;
}