#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "mbed.h"
#include "FlashIAP.h"

#define MAX_USERS 9

class UserManager {
public:
    UserManager();
    void init();
    bool verifyAdminPassword(const char* password);
    bool verifyUserPassword(const char* password);
    void storeUserPassword(int userIndex, const char* password);
    void deleteUser(int userIndex);

private:
    char passwords[MAX_USERS][9];  // in-memory copy
    char adminPassword[9];
    FlashIAP flash;

    uint32_t calculateChecksum(const struct FlashPage& page);
    bool readLatestPage(struct FlashPage& out);
    void writeNewPage(uint32_t prevIndex);
};

#endif
