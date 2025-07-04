#include "usermanager.h"
#include "Encryption.h"
#include <iostream>
#include <fstream>
//#include <stdexcept>
using namespace std;

UserManager* UserManager::m_instance = nullptr;

UserManager* UserManager::instance(const string& filePath) {
    if (!m_instance) {
        m_instance = new UserManager(filePath);
    }
    return m_instance;
}

UserManager::UserManager(string filePath) : filePath(filePath) {
    loadFromFile();
}

void UserManager::loadFromFile() {
    users.clear();
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filePath << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        users.push_back(User::fromString(line));
    }
}

void UserManager::saveToFile() const {
    ofstream file(filePath);
    for (const auto& user : users) {
        file << user.toString() << "\n";
    }
}


User* UserManager::findUser(const string& username) {
    qDebug() << "Searching for user:" << QString::fromStdString(username);
    qDebug() << "Number of users:" << users.size();

    for (auto& user : users) {
        if (user.getUsername() == username) {
            qDebug() << "User found:" << QString::fromStdString(username);
            return &user;
        }
    }
    qDebug() << "User not found:" << QString::fromStdString(username);
    return nullptr;
}


User* UserManager::findUser(const QString& username) {
    for (auto& user : users)
        if (user.getUsername() == username)
            return &user;
    return nullptr;
}

bool isValidEmail(const string& email) {
    return email.find('@') != string::npos;
}

bool isValidPassword(const string& password) {
    if (password.length() < 6) return false;

    return  isValidInput(password);
}

bool isValidUsername(const string& username) {

    UserManager* manager = UserManager::instance();

    for (char c : username)
        if (!isalnum(c) && c != '_')
            return false;

    if(manager->findUser(username) != nullptr)
        return false;

    return !username.empty();
}

bool isValidSecurityAnswer(const string& answer) {
    for (char c : answer)
        if (!isalnum(c) && c != ' ' && c != '_')
            return false;
    return !answer.empty();
}

bool isValidFullName(const string& name) {
    for (char c : name)
        if (!isalpha(c) && c != ' ')
            return false;
    return !name.empty();
}

void UserManager::signUp() {
    string fullName, username, password, confirmPassword, email, secret;

    do {
        cout << "Full Name: ";
        getline(cin, fullName);
        if (!isValidFullName(fullName)) {
            cout << "Invalid full name. Only letters and spaces are allowed.\n";
        } else {
            break;
        }
    } while (true);

    do {
        cout << "Username (letters, digits, underscore only): ";
        getline(cin, username);
        if (!isValidUsername(username)) {
            cout << "Invalid username format.\n";
        } else if (findUser(username)) {
            cout << "This username already exists.\n";
        } else {
            break;
        }
    } while (true);

    do {
        cout << "Password (min 8 chars, include upper/lower/digit): ";
        getline(cin, password);
        cout << "Confirm Password: ";
        getline(cin, confirmPassword);

        if (password != confirmPassword) {
            cout << "Passwords do not match.\n";
        } else if (!isValidPassword(password)) {
            cout << "Password is too weak.\n";
        } else {
            break;
        }
    } while (true);

    do {
        cout << "Email: ";
        getline(cin, email);
        if (!isValidEmail(email)) {
            cout << "Invalid email address. Must contain '@'.\n";
        } else {
            break;
        }
    } while (true);

    do {
        cout << "Security Question - What is your best friend's name? ";
        getline(cin, secret);
        if (!isValidSecurityAnswer(secret)) {
            cout << "Invalid characters in answer. Use letters, numbers, spaces, and underscores only.\n";
        } else {
            break;
        }
    } while (true);

    string hashedPass = Encryption::hashPassword(password);
    string encryptedSecret = Encryption::encrypt(secret);

    users.emplace_back(fullName, username, hashedPass, email, encryptedSecret);
    saveToFile();
    cout << "Registration successful!\n";
}

void UserManager::login() {
    string username, password;
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);

    User* user = findUser(username);
    if (!user || user->getPasswordHash() != Encryption::hashPassword(password)) {
        cout << "Invalid username or password.\n";
        return;
    }

    cout << "Login successful! Welcome, " << user->getFullName() << "!\n";
    updateAccount(user);
}

void UserManager::forgotPassword() {
    string username;
    cout << "Enter your username: ";
    getline(cin, username);

    User* user = findUser(username);
    if (!user) {
        cout << "User not found.\n";
        return;
    }

    string answer;
    cout << "What is your best friend's name? ";
    getline(cin, answer);

    if (Encryption::decrypt(user->getEncryptedSecret()) == answer) {
        string newPass, confirmPass;
        do {
            cout << "New Password: ";
            getline(cin, newPass);
            cout << "Confirm New Password: ";
            getline(cin, confirmPass);

            if (newPass != confirmPass) {
                cout << "Passwords do not match.\n";
            } else if (!isValidPassword(newPass)) {
                cout << "Password is too weak.\n";
            } else {
                break;
            }
        } while (true);

        *user = User(user->getFullName(), user->getUsername(), Encryption::hashPassword(newPass),
                     user->getEmail(), user->getEncryptedSecret());
        saveToFile();
        cout << "Password successfully updated!\n";
    } else {
        cout << "Incorrect answer to the security question.\n";
    }
}

void UserManager::adduser(User _user){


    this->users.push_back(_user);
    this->saveToFile();
}

void UserManager::updateAccount(User* user) {

    string choice;
    while (true) {
        cout << "Select an option: ";
        getline(cin, choice);
        if (choice == "1") {
            string newName;
            cout << "New Full Name: ";
            getline(cin, newName);
            if (isValidFullName(newName)) {
                user->setFullName(newName);
                cout << "Full name updated.\n";
            } else {
                cout << "Invalid name format.\n";
            }
        } else if (choice == "2") {
            string newEmail;
            cout << "New Email: ";
            getline(cin, newEmail);
            if (isValidEmail(newEmail)) {
                user->setEmail(newEmail);
                cout << "Email updated.\n";
            } else {
                cout << "Invalid email.\n";
            }
        } else if (choice == "3") {
            string newPass, confirm;
            cout << "New Password: ";
            getline(cin, newPass);
            cout << "Confirm New Password: ";
            getline(cin, confirm);
            if (newPass != confirm) {
                cout << "Passwords do not match.\n";
            } else if (!isValidPassword(newPass)) {
                cout << "Password is too weak.\n";
            } else {
                user->setPasswordHash(Encryption::hashPassword(newPass));
                cout << "Password updated.\n";
            }
        } else if (choice == "4") {
            string newAnswer;
            cout << "New Best Friend Name: ";
            getline(cin, newAnswer);
            if (isValidSecurityAnswer(newAnswer)) {
                user->setEncryptedSecret(Encryption::encrypt(newAnswer));
                cout << "Security answer updated.\n";
            } else {
                cout << "Invalid format.\n";
            }
        } else if (choice == "0") {
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
    saveToFile();
    cout << "All changes saved.\n";
}

bool isValidInput(const std::string& username) {
    if (username.empty()) return false;
    for (char c : username) {
        if (!std::isalnum(c) && c != '_' && c != '@') {
            return false;  // فقط حروف، ارقام، '_' و '@' مجازند
        }
    }
    return true;
}

