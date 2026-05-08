#define _CRT_SECURE_NO_WARNINGS

#include "hash.h" 
#include "util.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>




int main()
{
    PasswordHashMap phm;
    phm.put("test", "sifresifre");
    phm.printStats();
}