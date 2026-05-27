//
// Created by Charly CATIN--RICO on 27/05/2026.
//
#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>


// la fonction de callback pour recupere rle resultat de la db
int callback(void* NotUsed, int col, char** val, char** azColName) {

    // On boucle sur chaque colonne de la ligne en cours
    for (int i = 0; i < col; i++) {
        // Affiche : "NomDeLaColonne = Valeur"
        std::cout << azColName[i] << " " << (val[i] ? val[i] : "NULL") << " | ";
    }

    // Un saut de ligne pour séparer chaque tâche (chaque ligne de la BDD)
    std::cout << std::endl;

    // On doit toujours retourner 0 pour dire à SQLite de passer à la ligne suivante
    return 0;
}


// fonction add
void addTask(sqlite3* db, char* &dbError) {

    std::string task;
    std::cout << "Enter task : " << std::endl;
    std::getline(std::cin, task);
    std::string request;

    request = "INSERT INTO Tasks (task) VALUES('" + task + "');";

    sqlite3_exec(db, request.c_str(), nullptr, nullptr, &dbError);

    if (dbError != nullptr) {
        std::cerr << "Error during the process failure " << dbError << std::endl;
    }

    else {
        std::cout << "Task added succes" << std::endl;
    }

}



// fonction supp
void delTask(sqlite3* db, char* &dbError) {

    std::string taskID;
    std::cout << "Enter the Task ID : " << std::endl;
    std::cin >>taskID;

    std::string request;
    const int x = std::stoi(taskID);

    request = "DELETE FROM Tasks WHERE idTask = " + std::to_string(x) + ";";
    sqlite3_exec(db, request.c_str(), nullptr, nullptr, &dbError);
    if (dbError != nullptr) {
        std::cerr << "Error during the process failure " << dbError << std::endl;
    }
    else {
        std::cout << "Task deleted succes" << std::endl;
    }

}


// fonction lire tout
void seeTasks( sqlite3* db, char* &dbError) {
    sqlite3_exec(db, "SELECT * FROM Tasks", callback, nullptr, &dbError);

    if (dbError != nullptr) {
        std::cerr << "Error during the process failure "<< dbError << std::endl;
    }
    else {
        std::cout << "End of the tasks" << std::endl;
    }
}



int main() {

    std::vector<std::string> nameList = {};     // liste des taches à réaliser
    std::string userInput;
    sqlite3* db = nullptr;                                // db for the tasks
    char* dbError = nullptr;


    // database creation
    int dbInitResult = sqlite3_open("tasks.db", &db);

    // check db opening
    if (dbInitResult != SQLITE_OK ) {
        std::cerr << "Can't open the database failure " << dbError << std::endl;    // cerr because error message
    }
    else {
        std::cout << "Database opened succes" << std::endl;
    }

    sqlite3_exec(db, "CREATE TABLE Tasks (idTask INTEGER PRIMARY KEY AUTOINCREMENT, task TEXT)", nullptr, nullptr, &dbError);



    // boucle principale
    while (true) {
        std::cout << "\n1 : see tasks \n2 : add task \n3 : del task \nq : quit" << std::endl;
        std::getline(std::cin, userInput);

        if (userInput == "1") {
            // see tasks function
            seeTasks(db, dbError);
        }

        else if (userInput == "2") {
            // add task function
            addTask(db, dbError);
        }

        else if (userInput == "3") {
            // del task function
            delTask(db, dbError);
        }

        else if (userInput == "q") {
            break;
        }
    }

    return 0;
}