#pragma once

#include "hash.h" 
#include "stack.h" 
#include "userbst.h"
#include "graph.h"

#include <iostream> 
#include <string>


class UserManagementSystem
{
    PasswordHashMap  passwords;
    UserBST          registry;
    FriendGraph      social;
    Stack<std::string>    navHistory;   // gezi gecmisi

    std::string  loggedInUser;
    bool isLoggedIn = false;

    void printBanner() const
    {
        std::cout << "\n";
        std::cout << "  ----------------------------------------\n";
        std::cout << "  -      KULLANICI YONETIM SISTEMI       -\n";
        std::cout << "  ----------------------------------------\n";
    }

    void printSeparator() const { std::cout << "  ----------------------------------\n"; }

    std::string promptLine(const std::string& label) const
    {
        std::cout << "  " << label << ": ";
        std::string val; 
        std::getline(std::cin, val);
        return val;
    }

    std::string promptPassword(const std::string& label) const
    {
        std::cout << "  " << label << ": ";
        std::string val; 
        std::getline(std::cin, val);
        return val;
    }

    
    void handleSignUp();
    void handleLogin();
    void handleLogout();
    void handleViewProfile();
    void handleChangePassword();
    void handleFriends();
    void handleNavHistory();
    void handleAdminPanel();    
    void runUserMenu();
   
public:
    void run()
    {
        printBanner();
        logEvent(1, "Sistem baslatildi.");

        while (true) {
            printSeparator();
            std::cout << "  1. Kayit ol\n"
                << "  2. Giris yap\n"
                << "  0. Cikis\n";
            printSeparator();
            std::cout << "  Seciniz: ";
            std::string ch; 
            std::getline(std::cin, ch);

            if (ch == "1") handleSignUp();
            else if (ch == "2") { handleLogin(); if (isLoggedIn) runUserMenu(); }
            else if (ch == "0") { std::cout << "  Gorusuruz!\n\n"; logEvent(1, "Sistem kapatiliyor."); break; }
            else std::cout << "  Gecersiz secim.\n";
        }
    }
};
