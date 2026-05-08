#pragma once

#include <sstream> //string stream
#include <iostream> 
#include <cstdint>
#include <string>
#include <vector>
#include <iomanip>

// SHA-256 benzeri hashing fonksiyonu
// Deterministik 64-byte hex string döner

namespace PasswordHash
{
    static const uint32_t K[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
        0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
        0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
        0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
        0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
        0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
        0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
        0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
        0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };

    static uint32_t rotr(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }

    inline std::string hash(const std::string& msg)
    {
        uint32_t h0 = 0x6a09e667, h1 = 0xbb67ae85, h2 = 0x3c6ef372, h3 = 0xa54ff53a,
            h4 = 0x510e527f, h5 = 0x9b05688c, h6 = 0x1f83d9ab, h7 = 0x5be0cd19;

        std::vector<uint8_t> data(msg.begin(), msg.end());
        uint64_t bitLen = data.size() * 8;
        data.push_back(0x80);
        while (data.size() % 64 != 56) data.push_back(0x00);
        for (int i = 7; i >= 0; --i)
            data.push_back(static_cast<uint8_t>((bitLen >> (i * 8)) & 0xFF));

        for (size_t i = 0; i < data.size(); i += 64) {
            uint32_t w[64];
            for (int j = 0; j < 16; ++j)
                w[j] = (data[i + j * 4] << 24) | (data[i + j * 4 + 1] << 16) |
                (data[i + j * 4 + 2] << 8) | data[i + j * 4 + 3];
            for (int j = 16; j < 64; ++j) {
                uint32_t s0 = rotr(w[j - 15], 7) ^ rotr(w[j - 15], 18) ^ (w[j - 15] >> 3);
                uint32_t s1 = rotr(w[j - 2], 17) ^ rotr(w[j - 2], 19) ^ (w[j - 2] >> 10);
                w[j] = w[j - 16] + s0 + w[j - 7] + s1;
            }
            uint32_t a = h0, b = h1, c = h2, d = h3, e = h4, f = h5, g = h6, hh = h7;
            for (int j = 0; j < 64; ++j) {
                uint32_t S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
                uint32_t ch = (e & f) ^ (~e & g);
                uint32_t tmp1 = hh + S1 + ch + K[j] + w[j];
                uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
                uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
                uint32_t tmp2 = S0 + maj;
                hh = g; g = f; f = e; e = d + tmp1;
                d = c;  c = b; b = a; a = tmp1 + tmp2;
            }
            h0 += a; h1 += b; h2 += c; h3 += d;
            h4 += e; h5 += f; h6 += g; h7 += hh;
        }
        std::ostringstream oss;
        for (uint32_t v : {h0, h1, h2, h3, h4, h5, h6, h7})
            oss << std::hex << std::setw(8) << std::setfill('0') << v;
        return oss.str();
    }
}

// Hash-map ve ilgili fonksiyon implementasyonlari icinde barindiran sinif
class PasswordHashMap
{
    static const int BUCKETS = 64;

    struct Node {
        std::string key;   // username
        std::string value; // hashed password
        Node* next;
        Node(const std::string& k, const std::string& v) : key(k), value(v), next(nullptr) {}
    };

    Node* table[BUCKETS];

    int bucketIndex(const std::string& key) const
    {
        size_t h = 0;
        for (char c : key) h = h * 31 + static_cast<unsigned char>(c);
        return static_cast<int>(h % BUCKETS);
    }

public:
    PasswordHashMap() { std::fill(table, table + BUCKETS, nullptr); } // memset'in modern cpp versiyonu

    ~PasswordHashMap()
    {
        for (int i = 0; i < BUCKETS; ++i) {
            Node* cur = table[i];
            while (cur) { Node* tmp = cur; cur = cur->next; delete tmp; }
        }
    }

    void put(const std::string& username, const std::string& rawPassword);
    bool verify(const std::string& username, const std::string& rawPassword) const;
    bool contains(const std::string& username) const;
    void remove(const std::string& username);
    void printStats() const;
};