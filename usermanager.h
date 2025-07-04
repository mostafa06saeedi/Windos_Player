#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <vector>
#include <string>
#include <QString>
#include<QDebug>
#include "User.h"

using namespace std;

class UserManager
{
private:
    vector<User> users;
    string filePath;

    UserManager(string filePath = "D:/nima/playu/sers.txt");

    static UserManager* m_instance;

public:
    static UserManager* instance(const string& filePath = "D:/nima/play/users.txt");

    void loadFromFile();
    void saveToFile() const;
    User* findUser(const string& username);
    User* findUser(const QString& username);

    void updateAccount(User* user);

    void signUp();
    void login();
    void forgotPassword();

    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;

    void adduser(User _user);

};

bool isValidPassword(const string& password);
bool isValidInput(const std::string& username);
bool isValidEmail(const string& email);
bool isValidUsername(const string& username);


#endif // USERMANAGER_H
