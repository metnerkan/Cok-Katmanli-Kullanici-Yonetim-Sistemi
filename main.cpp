#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <limits>
#include <iomanip>
#include <ctime>


static std::string currentTimestamp() // simdiki zamani string veri tipinde donduren fonksiyon
{
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

static void clearInput()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


// Kayýt sistemi
struct AuditEvent
{
    int priority;   // öncelik uygulamasý, daha yüksek önceliði olan daha yukarýda gözükür
    std:: string timestamp;
    std::string description;

    bool operator<(const AuditEvent& o) const { return priority < o.priority; } /*
    operator overloading, öncelik kýyasý için iki adet AuditEvent nesnesi kýyasý için kullanýlýr
    */
};

std::priority_queue<AuditEvent> auditHeap;

static void logEvent(int priority, const std::string& desc)
{
    auditHeap.push({ priority, currentTimestamp(), desc });  // priority queue push eden fonk.
    // STL (standart template library) de mevcuttur.
}

// Kullanýcý verilerini tutan struct
struct User
{
    std::string username;
    std::string role;        // "admin" | "user"
    std::string createdAt;
    bool   isActive;

    User() : role("user"), isActive(true) {}
    User(const std::string& u, const std::string& r = "user")
        : username(u), role(r),
        createdAt(currentTimestamp()), isActive(true) {
    }
};


int main()
{
    std::cout << "merhaba" << std::endl;
}