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

// kullanici hash map icinde var mi kontrolu yapan fonk
bool PasswordHashMap::contains(const std::string& username) const
{
    int idx = bucketIndex(username);
    Node* cur = table[idx];
    while (cur) 
    { 
        if (cur->key == username) 
            return true; 
        cur = cur->next; 
    }
    return false;
}

// kullanici silen fonk
void PasswordHashMap::remove(const std::string& username)
{
    int idx = bucketIndex(username);
    Node* cur = table[idx];
    Node* prev = nullptr;
    while (cur) 
    {
        if (cur->key == username) 
        {
            if (prev) // onceki dugum varsa onceki ile bir sonraki baglanir
                prev->next = cur->next;
            else       // yoksa head direkt olarak sonraki dugum olur.
                table[idx] = cur->next;
            delete cur;
            return;
        }
        prev = cur; cur = cur->next;
    }
}

// hash map istatistiklerini yazdiran fonk
void PasswordHashMap::printStats() const
{
    int used = 0, maxChain = 0, total = 0;
    for (int i = 0; i < BUCKETS; ++i) {
        int len = 0;
        for (Node* c = table[i]; c; c = c->next) 
            ++len;
        if (len) 
            ++used;
        maxChain = std::max(maxChain, len);
        total += len;
    }
    std::cout << "  Hash map: " << total << " kayit, "
        << used << "/" << BUCKETS << " buckets, "
        << "en cok zincirleme = " << maxChain << "\n";
}


