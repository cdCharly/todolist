//
// Created by Charly CATIN--RICO on 27/05/2026.
//
#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>

// pour les fenetres
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QInputDialog>
#include <QLabel>


// la fonction de callback pour recupere les resultat de la db
int callback(void* data, int col, char** val, char** colName) {

    // layout
    auto* layout = static_cast<QVBoxLayout*>(data);

    // verif que la table est pas vide
    if (col > 1 && val[1] != nullptr) {

        // conversion
        auto taskText = QString::fromUtf8(val[1]);


        auto* taskLabel = new QLabel(taskText);

        // 5. On l'ajoute directement au layout de notre QGroupBox
        layout->addWidget(taskLabel);
        // delete taskLabel;
    }

    return 0; // Obligatoire pour SQLite
}

// Nettoie tous les éléments visuels d'un layout
void clearLayout(QLayout* layout) {
    QLayoutItem* item;
    // Tant qu'il y a des éléments dans le layout
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget(); // On détruit le widget visuel (libération mémoire)
        }
        delete item; // On détruit le conteneur du layout
    }
}




// fonction add
void addTask(sqlite3* db, QWidget* window) {


    bool ok;
    QString userInput = QInputDialog::getText(window, "Add Task", "Enter task : ", QLineEdit::Normal, "", &ok);


    if (ok && !userInput.isEmpty()) {
        std::string task = userInput.toStdString();
        std::string request = "INSERT INTO Tasks (task) VALUES('" + task + "');";
        char* dbError = nullptr;
        sqlite3_exec(db, request.c_str(), nullptr, nullptr, &dbError);


        if (dbError != nullptr) {
            std::cerr << "Error during the process failure " << dbError << std::endl;
        }

        else {
            std::cout << "Task added succes" << std::endl;
        }

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
void seeTasks(sqlite3* db, QVBoxLayout* layout) {

    // 1. On vide l'interface des anciennes tâches
    clearLayout(layout);

    char* dbError = nullptr;

    // 2. On passe 'layout' en 4ème argument ! Il arrivera dans le 'void* data' du callback
    sqlite3_exec(db, "SELECT * FROM Tasks", callback, layout, &dbError);

    if (dbError != nullptr) {
        std::cerr << "Error during the process: " << dbError << std::endl;
        sqlite3_free(dbError);
    }
}




int main(int argc, char* argv[]) {

    std::vector<std::string> nameList = {};     // liste des taches à réaliser
    std::string userInput;
    sqlite3* db = nullptr;                                // db for the tasks
    char* dbError = nullptr;

    // init de l'app
    QApplication app(argc, argv);

    // init de la fenetre
    QWidget window;

    // layouts
    auto *buttonLayout = new QHBoxLayout();
    auto *hLayout = new QVBoxLayout(&window);
    auto *tLayout = new QVBoxLayout();              // avec taskBox uniquement

    //groups
    auto taskBox = new QGroupBox("Tasks");  // avec le tlayout uniquement



    // config fenetre
    window.setWindowTitle("TodoList");
    window.resize(400,500);


    // boutons
    auto addButton = new QPushButton("Add");
    auto delButton = new QPushButton("Del");
    auto seeButton = new QPushButton("See");

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(delButton);
    buttonLayout->addWidget(seeButton);

    hLayout->addLayout(buttonLayout);
    hLayout->addWidget(taskBox);
    hLayout->setAlignment(Qt::AlignTop);

    taskBox->setLayout(tLayout);




    // connection des boutons

    //[&](){  } fonction anonyme
    QObject::connect(addButton, &QPushButton::clicked, [&]() {
        addTask(db, &window);
        }
    );

    QObject::connect(seeButton, &QPushButton::clicked, [&]() {
        seeTasks(db, tLayout);
        }
    );


    window.show();

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


    return app.exec();
}