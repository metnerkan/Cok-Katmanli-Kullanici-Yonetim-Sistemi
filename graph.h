#pragma once

#include <string>   
#include <vector>
#include <map>
#include <queue>

/*
* Kisa ozet:
* Grafta her bir kullanici adjacent list (komsu listesinde) tutulur, bu listede bulunan
* kullanicilarin her birinin kendine ait kenarlari (arkadaslari bulunur)
* bu kenarlari tuttugumuz veri yapisi ise bagli listedir. (EdgeNode)
* 
* Kayit olurken her bir kullanici kendine ait komsu listesine eklenir. Bu sayede her kullanicinin
* arkadas listesi takip edilebilir.
*/

class FriendGraph
{
    // her bir kullanici adi linked list'e atanir
    struct EdgeNode {
        std::string   friend_username;
        EdgeNode* next;
        EdgeNode(const std::string& f) : friend_username(f), next(nullptr) {}
    };

    // komsuluk listesi
    struct AdjList {
        std::string    username;
        EdgeNode* head = nullptr;
    };

    std::vector<AdjList> adj;

    int indexOf(const std::string& u) const
    {
        for (int i = 0; i < (int)adj.size(); ++i)
            if (adj[i].username == u) return i;
        return -1;
    }

public:
    ~FriendGraph()
    {
        for (auto& a : adj) 
        {
            EdgeNode* cur = a.head;
            while (cur) 
            {
                EdgeNode* tmp = cur; 
                cur = cur->next; 
                delete tmp; 
            }
        }
    }

    void addUser(const std::string& u) // komsuluk listesine kullanici ekleyen fonk
    {
        if (indexOf(u) == -1) adj.push_back({ u, nullptr });
    }

    void removeUser(const std::string& u)
    {
        // komple vektor gezilir, tek tek silinecek kullaniciya isaret eden pointerlar duzeltilir.
        for (auto& a : adj) {
            EdgeNode* cur = a.head;
            EdgeNode* prev = nullptr;
            while (cur) 
            {
                if (cur->friend_username == u) 
                {
                    if (prev) 
                        prev->next = cur->next;
                    else       
                        a.head = cur->next;
                    delete cur;
                    cur = prev ? prev->next : a.head;
                }
                else
                {
                    prev = cur;
                    cur = cur->next;
                }
            }
        }
        // kendisini de sileriz
        int idx = indexOf(u);
        if (idx != -1) {
            EdgeNode* cur = adj[idx].head;
            while (cur) 
            { 
                EdgeNode* tmp = cur; 
                cur = cur->next; 
                delete tmp; 
            }
            adj.erase(adj.begin() + idx);
        }
    }

    // arkadas ekler
    void addFriend(const std::string& a, const std::string& b)
    {
        auto addEdge = [&](const std::string& from, const std::string& to) {
            int i = indexOf(from);
            if (i == -1) return;
            for (EdgeNode* c = adj[i].head; c; c = c->next)
                if (c->friend_username == to) return; // zaten arkadaslar
            EdgeNode* node = new EdgeNode(to);
            node->next = adj[i].head;
            adj[i].head = node;
            };
        addEdge(a, b); // a nin arkadas listesine b
        addEdge(b, a); // b nin arkadas listesine a eklenir.
        // her kullanicinin kendisine ozgu bir arkadas listesi oldugunu unutmamaliyiz.
    }

    // kullanicinin arkadas listesini kontrol eden fonk
    bool areFriends(const std::string& a, const std::string& b) const
    {
        int i = indexOf(a);
        if (i == -1) return false;
        for (EdgeNode* c = adj[i].head; c; c = c->next)
            if (c->friend_username == b) return true;
        return false;
    }

    // kullanicin arkadaslarini string dizisini atayip return eden fonk
    std::vector<std::string> friendsOf(const std::string& u) const
    {
        std::vector<std::string> result;
        int i = indexOf(u);
        if (i == -1) return result;
        for (EdgeNode* c = adj[i].head; c; c = c->next)
            result.push_back(c->friend_username);
        return result;
    }

    // Breadth First Search algorithmasi iki kullanici arasindaki en kisa yolu bulmak icin kullanilir
    std::vector<std::string> shortestPath(const std::string& src, const std::string& dst) const
    {
        if (indexOf(src) == -1 || indexOf(dst) == -1) return {};
        std::map<std::string, std::string> parent;
        std::queue<std::string> q;
        q.push(src);
        parent[src] = "";
        while (!q.empty()) {
            std::string cur = q.front(); 
            q.pop();
            if (cur == dst) {
                std::vector<std::string> path;
                for (std::string at = dst; at != ""; at = parent[at])
                    path.push_back(at);
                reverse(path.begin(), path.end());
                return path;
            }
            int i = indexOf(cur);
            if (i == -1) continue;
            for (EdgeNode* c = adj[i].head; c; c = c->next)
                if (!parent.count(c->friend_username)) 
                {
                    parent[c->friend_username] = cur;
                    q.push(c->friend_username);
                }
        }
        return {}; // yol yok
    }

};