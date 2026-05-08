#include "hash.h"


// kullanici adi ve hashed sifreyi hash map'e koyan fonksiyon
void PasswordHashMap::put(const std::string& username, const std::string& rawPassword)
{
    int idx = bucketIndex(username); // kullanici adi ile kova indeksi elde edilir
    std::string hashed = PasswordHash::hash(rawPassword); // ham sifre hash fonksiyonundan gecirilir
    Node* cur = table[idx];
    while (cur) {
        if (cur->key == username) { cur->value = hashed; return; }
        cur = cur->next;
    }
    Node* node = new Node(username, hashed); // kullanici adi ve hashed sifre hash map'te tutulur.
    node->next = table[idx]; // yeni eklenen node ilk elemana isaret eder
    table[idx] = node; // sonra head, yeni eklenen node olarak guncellenir.
}

// hash map icinde gezen ve eslesme var mi diye kontrol eden fonksiyon
bool PasswordHashMap::verify(const std::string& username, const std::string& rawPassword) const
{
    int idx = bucketIndex(username);
    std::string hashed = PasswordHash::hash(rawPassword);
    Node* cur = table[idx];
    while (cur) {
        if (cur->key == username) return cur->value == hashed;
        cur = cur->next;
    }
    return false;
}
