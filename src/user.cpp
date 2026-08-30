#include "user.h"
#include <sstream>

using namespace std;

 QString User::currentname = "";


User::User(string fullName, string username, string passwordHash,
           string email, string secretEncrypted)
    : fullName(fullName), username(username), passwordHash(passwordHash),
    email(email), secretEncrypted(secretEncrypted) {}

string User::getUsername() const { return username; }
string User::getPasswordHash() const { return passwordHash; }
string User::getFullName() const { return fullName; }
string User::getEmail() const { return email; }
string User::getEncryptedSecret() const { return secretEncrypted; }

void User::setFullName(const string& name) { fullName = name; }
void User::setEmail(const string& mail) { email = mail; }
void User::setPasswordHash(const string& hash) { passwordHash = hash; }
void User::setEncryptedSecret(const string& secret) { secretEncrypted = secret; }

string User::toString() const {
    return fullName + ";" + username + ";" + passwordHash + ";" + email + ";" + secretEncrypted;
}

User User::fromString(const string& line) {
    stringstream ss(line);
    string fullName, username, passwordHash, email, secretEncrypted;

    getline(ss, fullName, ';');
    getline(ss, username, ';');
    getline(ss, passwordHash, ';');
    getline(ss, email, ';');
    getline(ss, secretEncrypted, ';');

    return User(fullName, username, passwordHash, email, secretEncrypted);
}
