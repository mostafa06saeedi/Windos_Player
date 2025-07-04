#include "encryption.h"
#include <functional>

using namespace std;

string Encryption::hashPassword(const string& password) {
    hash<string> hasher;
    return to_string(hasher(password));
}

string Encryption::encrypt(const string& data, int key) {
    string result = data;
    for (char& c : result)
        c += key;
    return result;
}

string Encryption::decrypt(const string& encrypted, int key) {
    string result = encrypted;
    for (char& c : result)
        c -= key;
    return result;
}
