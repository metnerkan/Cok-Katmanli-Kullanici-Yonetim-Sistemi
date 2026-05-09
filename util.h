#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>  
#include <queue>

static std::string currentTimestamp() // simdiki zamani string veri tipinde donduren fonksiyon
{
    time_t rawTime;
    tm timeInfo;
    time(&rawTime);
    char buf[20];
    if (!localtime_s(&timeInfo, &rawTime))
    {
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeInfo);
        return std::string(buf);
    }
    else
        std::cerr << "Zaman donusturulemedi!" << std::endl; return {};
}

static void clearInput()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


// Kayýt sistemi
struct AuditEvent
{
    int priority;   // öncelik uygulamasý, daha yüksek önceliði olan daha yukarýda gözükür
    std::string timestamp;
    std::string description;

    bool operator<(const AuditEvent& o) const { return priority < o.priority; } /*
    operator overloading, öncelik kýyasý için iki adet AuditEvent nesnesi kýyasý için kullanýlýr
    */
};

inline std::priority_queue<AuditEvent> auditHeap;

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
    User(const std::string& u, const std::string& r = "user") // modern constructor syntax
        : username(u), role(r), createdAt(currentTimestamp()), isActive(true) {
    }
};
