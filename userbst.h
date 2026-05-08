#pragma once

#include "util.h"  

#include <string>
#include <vector>

//Binary search tree veri yapisini yoneten sinif.
class UserBST
{
    struct BSTNode {
        User     user;
        BSTNode* left;
        BSTNode* right;
        BSTNode(const User& u) : user(u), left(nullptr), right(nullptr) {}
    };

    BSTNode* root = nullptr;

    // BST Tree ekleme fonksiyonu
    BSTNode* insert(BSTNode* node, const User& u)
    {
        // kullanici adlarini alfabetik olarak kiyaslar
        // alfabede once gelenler agacin sol tarafina, sonra gelenler sag tarafina yerlesir.
        if (!node) return new BSTNode(u);
        if (u.username < node->user.username)
            node->left = insert(node->left, u);
        else if (u.username > node->user.username)
            node->right = insert(node->right, u);
        else
            node->user = u; // update
        return node;
    }

    // BST Tree arama fonksiyonu
    BSTNode* find(BSTNode* node, const std::string& username) const
    {
        if (!node) return nullptr;
        if (username == node->user.username) return node;
        if (username < node->user.username) return find(node->left, username);
        return find(node->right, username);
    }

    // Alfabetik olarak en kucuk dugum
    BSTNode* minNode(BSTNode* node) const
    {
        while (node->left) 
            node = node->left;
        return node;
    }

    // Binary search tree silme islemi
    BSTNode* remove(BSTNode* node, const std::string& username)
    {
        if (!node) return nullptr;
        if (username < node->user.username)
            node->left = remove(node->left, username);
        else if (username > node->user.username)
            node->right = remove(node->right, username);
        else {
            if (!node->left) 
            {
                BSTNode* r = node->right; 
                delete node; 
                return r; 
            }
            if (!node->right) 
            { 
                BSTNode* l = node->left;  
                delete node; 
                return l; 
            }
            BSTNode* succ = minNode(node->right);
            node->user = succ->user;
            node->right = remove(node->right, succ->user.username);
        }
        return node;
    }

    // inorder yontemi ile BST traversal, kucukten buyuge siralama yapar.
    void inorder(BSTNode* node, std::vector<User>& out) const
    {
        if (!node) return;
        inorder(node->left, out);
        out.push_back(node->user);
        inorder(node->right, out);
    }

    // agaci bellekten temizleyen fonksiyon
    void destroy(BSTNode* node)
    {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    ~UserBST() { destroy(root); }

    void insert(const User& u) { root = insert(root, u); } 
    bool contains(const std::string& u) const { return find(root, u) != nullptr; }
    User* get(const std::string& u) const { auto* n = find(root, u); return n ? &n->user : nullptr; }
    void  remove(const std::string& u) { root = remove(root, u); }
    // tum kullanicilari inorder ile alfabetik olarak siralayip vector'e atayan fonksiyon
    std::vector<User> allUsers() const { std::vector<User> v; inorder(root, v); return v; }
};