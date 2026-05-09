#include "ums.h"

void UserManagementSystem::handleSignUp()
{
        navHistory.push("kayit-olma");
        printSeparator();
        std::cout << "  [ KAYDOL ]\n";
        printSeparator();

        std::string username = promptLine("Kullanici adi");
        if (username.empty()) { std::cout << "  Kullanici adi bos olamaz!\n"; return; }

        if (registry.contains(username)) {
            std::cout << "  Kullanici adi kullaniliyor!\n";
            logEvent(2, "Basarisiz kayit: kullanici adi '" + username + "' zaten var");
            return;
        }

        std::string pw1 = promptPassword("Sifre");
        std::string pw2 = promptPassword("Sifreyi tekrar giriniz");
        if (pw1 != pw2) { std::cout << "  Sifreler eslesmiyor\n"; return; }
        if (pw1.size() < 5) { std::cout << "  Sifreniz en az 5 karakter barindirmali\n"; return; }

        // ilk kaydolan kullanici default olarak admin atanir
        std::string role = registry.allUsers().empty() ? "admin" : "kullanici";

        User newUser(username, role);
        registry.insert(newUser);
        passwords.put(username, pw1);
        social.addUser(username);

        std::cout << "  Hesap olusturuldu! Rol: [" << role << "]\n";
        logEvent(3, "Yeni kullanici kaydi olusturuldu " + username + " (" + role + ")");  
}

void UserManagementSystem::handleLogin()
{
    navHistory.push("giris-yapti");
    printSeparator();
    std::cout << "  [ GIRIS YAP ]\n";
    printSeparator();

    std::string username = promptLine("Kullanici adi");
    std::string password = promptPassword("Sifre");

    if (!registry.contains(username)) {
        std::cout << "  Kullanici bulunamadi.\n";
        logEvent(4, "Giris basarisiz: bilinmeyen kullanici adi '" + username + "'");
        return;
    }

    User* u = registry.get(username);
    if (!u->isActive) {
        std::cout << "  Hesap deaktive edilmis.\n";
        logEvent(5, "Inaktif hesaba giris denemesi " + username);
        return;
    }

    if (!passwords.verify(username, password)) {
        std::cout << "  Yanlis sifre.\n";
        logEvent(5, "Giris basarisiz. '" + username + "' icin yanlis sifre");
        return;
    }

    loggedInUser = username;
    isLoggedIn = true;
    std::cout << "  Hos geldiniz, " << username << "! [" << u->role << "]\n";
    logEvent(1, "Kullanici giris yapti: " + username);
}

void UserManagementSystem::handleLogout()
{
    logEvent(1, "Kullanici cikis yapti: " + loggedInUser);
    std::cout << "  Cikis yaptiniz, gorusuruz " << loggedInUser << "!\n";
    loggedInUser.clear();
    isLoggedIn = false;
}

void UserManagementSystem::handleViewProfile()
{
    navHistory.push("profil");
    User* u = registry.get(loggedInUser);
    if (!u) return;
    printSeparator();
    std::cout << "  [ PROFIL ]\n";
    printSeparator();
    std::cout << "  Kullanici adi  : " << u->username << "\n";
    std::cout << "  Rol      : " << u->role << "\n";
    std::cout << "  Olusturuldu   : " << u->createdAt << "\n";
    std::cout << "  Durum   : " << (u->isActive ? "Aktif" : "Inaktif") << "\n";

    auto friends = social.friendsOf(loggedInUser);
    std::cout << "  Arkadaslar   : ";
    if (friends.empty())
        std::cout << "(yok)";
    else
    {
        for (const auto& f : friends)
            std::cout << f << " ";
    }
    std::cout << "\n";
}

void UserManagementSystem::handleChangePassword()
{
    navHistory.push("sifre-degistirme");
    printSeparator();
    std::cout << "  [ SIFRE DEGISTIR ]\n";
    printSeparator();
    std::string old_ = promptPassword("Simdiki sifre");
    if (!passwords.verify(loggedInUser, old_)) {
        std::cout << "  Simdiki sifre yanlis.\n";
        logEvent(4, loggedInUser + " icin basarisiz sifre degisimi");
        return;
    }
    std::string np1 = promptPassword("Yeni sifre giriniz");
    std::string np2 = promptPassword("Yeni sifreyi tekrar girin");
    if (np1 != np2) { std::cout << "  Sifreler ayni degil!\n"; return; }
    if (np1.size() < 5) { std::cout << "  Sifre en az 5 karakter icermelidir.\n"; return; }
    passwords.put(loggedInUser, np1);
    std::cout << "  Sifre basarili bir sekilde guncellendi.\n";
    logEvent(3, loggedInUser + " icin sifre guncellendi.");
}

void UserManagementSystem::handleFriends()
{
    navHistory.push("arkadaslar");
    printSeparator();
    std::cout << "  [ ARKADAS YONETIMI ]\n";
    printSeparator();
    std::cout << "  1. Arkadas ekle\n"
        << "  2. Arkadaslari goruntule\n"
        << "  3. Baglanti agini bul\n"
        << "  0. Geri\n";
    printSeparator();
    std::cout << "  Seciniz: ";
    std::string ch;
    std::getline(std::cin, ch);

    if (ch == "1") {
        std::string target = promptLine("Arkadas eklemek istediginiz kisinin kullanici adi:");
        if (target == loggedInUser) { std::cout << "  Kendinizi arkadas ekleyemezsiniz.\n"; return; }
        if (!registry.contains(target)) { std::cout << "  Kullanici bulunamadi.\n"; return; }
        if (social.areFriends(loggedInUser, target)) { std::cout << "  Zaetn arkadassiniz.\n"; return; }
        social.addFriend(loggedInUser, target);
        std::cout << "  " + target << " ile artik arkadas oldunuz\n";
        logEvent(1, loggedInUser + " arkadas ekledi: " + target);
    }
    else if (ch == "2") {
        auto friends = social.friendsOf(loggedInUser);
        std::cout << "  Arkadaslar (" << friends.size() << "):\n";
        for (const auto& f : friends) std::cout << "   * " << f << "\n";
    }
    else if (ch == "3") {
        std::string a = promptLine("Kimden");
        std::string b = promptLine("Kime");
        auto path = social.shortestPath(a, b);
        if (path.empty()) std::cout << "  Baglanti bulunamadi!\n";
        else {
            std::cout << "  Yol: ";
            for (size_t i = 0; i < path.size(); ++i) {
                std::cout << path[i];
                if (i + 1 < path.size()) std::cout << " -> ";
            }
            std::cout << "\n";
        }
    }
}

void UserManagementSystem::handleNavHistory()
{
    navHistory.push("gezinme-gecmisi");
    printSeparator();
    std::cout << "  [ GEZINME GECMISI ]\n";
    printSeparator();
    std::vector<std::string> pages;
    while (!navHistory.empty())
    {
        pages.push_back(navHistory.top());
        navHistory.pop();
    }
    for (int i = (int)pages.size() - 1; i >= 0; --i)
        navHistory.push(pages[i]);
    for (const auto& p : pages)
        std::cout << "   * " << p << "\n";
}

void UserManagementSystem::handleAdminPanel()
{
    User* me = registry.get(loggedInUser);
    if (!me || me->role != "admin") {
        std::cout << "  Erisim reddedildi. Sadece adminler bu paneli kullanabilir.\n";
        logEvent(5, loggedInUser + " tarafindan gecersiz admin erisimi.");
        return;
    }
    navHistory.push("admin-paneli");

    while (true) {
        printSeparator();
        std::cout << "  [ ADMIN PANELI ]\n";
        printSeparator();
        std::cout << "  1. Tum kullanicilari listele (BST alfabetik sirayla)\n"
            << "  2. Devre Disi Birak / Aktiflestir\n"
            << "  3. Kullaniciyi Sil\n"
            << "  4. Kayit Defteri\n"
            << "  5. Hash Map Istatistikleri\n"
            << "  0. Geri\n";
        printSeparator();
        std::cout << "  Seciniz: ";
        std::string ch;
        std::getline(std::cin, ch);

        if (ch == "0") break;
        else if (ch == "1") {
            auto users = registry.allUsers();
            std::cout << "\n  " << std::left << std::setw(16)
                << "Kullanici Adi"
                << std::setw(10)
                << "Rol"
                << "Durum\n";
            printSeparator();
            for (const auto& u : users)
                std::cout << "  " << std::left
                << std::setw(16)
                << u.username
                << std::setw(10)
                << u.role
                << std::setw(6)
                << (u.isActive ? "Aktif" : "Inaktif") << "\n";
        }
        else if (ch == "2") {
            std::string target = promptLine("Kullanici adi giriniz: ");
            User* t = registry.get(target);
            if (!t) { std::cout << "  Kullanici bulunamadi.\n"; continue; }
            if (target == loggedInUser) { std::cout << "  Kendinizi devre disi birakamazsiniz\n"; continue; }
            t->isActive = !t->isActive;
            std::cout << "  '" + target << "' artik "
                << (t->isActive ? "aktif" : "inaktif") << ".\n";
            logEvent(4, "Admin " + loggedInUser + " durumunu degistirdi: " + target);
        }
        else if (ch == "3") {
            std::string target = promptLine("Silinecek kullanici adi");
            if (target == loggedInUser) { std::cout << "  Kendinizi silemezsiniz.\n"; continue; }
            if (!registry.contains(target)) { std::cout << "  Kullanici bulunamadi.\n"; continue; }
            registry.remove(target);
            passwords.remove(target);
            social.removeUser(target);
            std::cout << "  Kullanici '" << target << "' silindi.\n";
            logEvent(5, "Admin " + loggedInUser + " su kullaniciyi sildi: " + target);
        }
        else if (ch == "4") {
            std::cout << "\n  [ KAYIT DEFTERI ]\n";
            printSeparator();
            std::cout << "  "
                << std::left
                << std::setw(12)
                << "Oncelik"
                << std::setw(18)
                << "Zaman"
                << "Olay\n";
            printSeparator();
            std::vector<AuditEvent> saved;
            while (!auditHeap.empty()) {
                const AuditEvent& e = auditHeap.top();
                std::cout << "  "
                    << std::setw(6)
                    << e.priority
                    << std::setw(22) << e.timestamp
                    << e.description << "\n";
                saved.push_back(e);
                auditHeap.pop();
            }
            for (auto& e : saved) auditHeap.push(e);
        }
        else if (ch == "5") {
            passwords.printStats();
        }
    }
}

void UserManagementSystem::runUserMenu()
{
    while (isLoggedIn) {
        printSeparator();
        std::cout << "  " << loggedInUser << " olarak giris yapildi.\n";
        printSeparator();
        std::cout 
            << "  1. Profili gor\n"
            << "  2. Sifre degistir\n"
            << "  3. Arkadas menusu\n"
            << "  4. Gezinme gecmisi\n"
            << "  5. Admin paneli\n"
            << "  0. Cikis yap\n";
        printSeparator();
        std::cout << "  Seciniz: ";
        std::string ch;
        std::getline(std::cin, ch);

        if (ch == "1") handleViewProfile();
        else if (ch == "2") handleChangePassword();
        else if (ch == "3") handleFriends();
        else if (ch == "4") handleNavHistory();
        else if (ch == "5") handleAdminPanel();
        else if (ch == "0") handleLogout();
        else std::cout << "  Gecersiz secim.\n";
    }
}


