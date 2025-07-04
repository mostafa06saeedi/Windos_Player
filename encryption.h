#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <QString>
#include <QCryptographicHash>
using namespace std;

namespace Encryption {
string hashPassword(const string& password);
string encrypt(const string& data, int key = 5);
string decrypt(const string& encrypted, int key = 5);
}

#endif // ENCRYPTION_H
