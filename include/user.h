#ifndef USER_H
#define USER_H

#include <string>
#include<QString>
#include<QStringList>
#include<QMap>
#include<QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

using namespace std;

class User
{

private:
    string fullName;
    string username;
    string passwordHash;
    string email;
    string secretEncrypted;
public:

    User(string fullName, string username, string passwordHash,
         string email, string secretEncrypted);

    string getUsername() const;
    string getPasswordHash() const;
    string getFullName() const;
    string getEmail() const;
    string getEncryptedSecret() const;


    static QString currentname;


    void setFullName(const string& name);
    void setEmail(const string& email);
    void setPasswordHash(const string& hash);
    void setEncryptedSecret(const string& secret);

    string toString() const;
    static User fromString(const string& line);
};

#endif // USER_H
