//
// Created by Charly CATIN--RICO on 27/05/2026.
//

// structure
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
#include <QCheckBox>

sqlite3* db = nullptr;  // db for the tasks

// fonction supp
void delTask(sqlite3* db, std::string task) {


    std::string request = "DELETE FROM Tasks WHERE Tasks.task = '"+task+"';";
    char* dbError = nullptr;
    sqlite3_exec(db, request.c_str(), nullptr, nullptr, &dbError);


    if (dbError != nullptr) {
        std::cerr << "Error during the process failure " << dbError << std::endl;
    }

    else {
        std::cout << "Task delete succes" << std::endl;
    }
}


// la fonction de callback pour recupere les resultat de la db
int callback(void* data, int col, char** val, char** colName) {

    // layout
    auto* layout = static_cast<QVBoxLayout*>(data);

    // verif que la table est pas vide
    if (col > 1 && val[1] != nullptr) {

        // conversion
        auto taskText = QString::fromUtf8(val[1]);


        auto* taskCheck = new QCheckBox(taskText);

        layout->addWidget(taskCheck);

    }

    return 0;
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






// fonction lire tout
void seeTasks(sqlite3* db, QVBoxLayout* layout) {

    clearLayout(layout);

    char* dbError = nullptr;

    // layout en 4ème arg Il arrivera dans le void* data de callback
    sqlite3_exec(db, "SELECT * FROM Tasks", callback, layout, &dbError);

    if (dbError != nullptr) {
        std::cerr << "Error during the process: " << dbError << std::endl;
        sqlite3_free(dbError);
    }
}




int main(int argc, char* argv[]) {

    std::vector<std::string> nameList = {};     // liste des taches à réaliser
    std::string userInput;

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
    // auto seeButton = new QPushButton("See");

    // check box doivent etre sur le coté d'une task
    auto checkBox = new QCheckBox("Check");

    // style des boutons
    addButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #E6BF29;" // Couleur de fond (Hexadécimal)
        "   color: white;"              // Couleur du texte
        "   border-radius: 5px;"        // Bords légèrement arrondis
        "   padding: 4px 7px;"         // Marges internes (haut/bas, gauche/droite)
        "}"

        "QPushButton:hover {"              // <-- Quand la souris survole
        "background-color: #4CAF50;"    // Vert un peu plus foncé
        "}"
    );

    delButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #E6BF29;"
        "   color: white;"
        "   border-radius: 5px;"
        "   padding: 4px 7px;"
        "}"
        "QPushButton:hover {"              // <-- Quand la souris survole
        "background-color: #F44336;"    // Vert un peu plus foncé
        "}"
        );

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(delButton);
    // buttonLayout->addWidget(seeButton);

    hLayout->addLayout(buttonLayout);
    hLayout->addWidget(taskBox);
    hLayout->setAlignment(Qt::AlignTop);

    taskBox->setLayout(tLayout);


    // connection des boutons

    //[&](){  } fonction anonyme
    QObject::connect(addButton, &QPushButton::clicked, [&]() {
        addTask(db, &window);
        seeTasks(db, tLayout);
        }
    );


    QObject::connect(delButton, &QPushButton::clicked, [&]() {

    // list des taches qu'on va vouloir supp
    std::vector<std::string> tasksToDelete;

    // tout les elem du layout
    for (int i = 0; i < tLayout->count(); ++i) {

        // recuperer le widget avec l'indice
        QWidget* widget = tLayout->itemAt(i)->widget();

        // verif widget existant
        if (widget) {
            // cast sur l'objet qui est la checkbox
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(widget);

            // verif que la case existe et qu'elle est cochée
            if (checkBox && checkBox->isChecked()) {
                // ajout à la liste des taches à supp
                tasksToDelete.push_back(checkBox->text().toStdString());
            }
        }
    }

    // boucle sur les données à supp
    for (const std::string& taskName : tasksToDelete) {
        delTask(db, taskName);
    }

    seeTasks(db, tLayout);
});


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

    seeTasks(db, tLayout);
    return app.exec();
}