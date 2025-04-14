#include "UserManager.h"
#include <cstring>

#define NUM_PAGES 8
#define PAGE_SIZE 256
#define SECTOR_SIZE 1024

#define FLASH_END      0x00040000  // Last address of flash (adjust if needed)
#define SECTOR0_ADDR   (FLASH_END - 2 * SECTOR_SIZE)
#define SECTOR1_ADDR   (FLASH_END - SECTOR_SIZE)

struct FlashPage {
    uint32_t index;
    char passwords[MAX_USERS][9];  // 9 bytes per password
    uint32_t checksum;
};

UserManager::UserManager() {
    memset(adminPassword, 0, sizeof(adminPassword));
    strncpy(adminPassword, "12345678", 8);
    adminPassword[8] = '\0';
}

uint32_t UserManager::calculateChecksum(const FlashPage& page) {
    uint32_t sum = page.index;
    for (int i = 0; i < MAX_USERS; i++) {
        for (int j = 0; j < 9; j++) {
            sum += page.passwords[i][j];
        }
    }
    return sum;
}

bool UserManager::readLatestPage(FlashPage& out) {
    flash.init();
    FlashPage candidate;
    uint32_t maxIndex = 0;
    bool found = false;

    for (int s = 0; s < 2; s++) {
        uint32_t base = s == 0 ? SECTOR0_ADDR : SECTOR1_ADDR;
        for (int p = 0; p < 4; p++) {
            uint32_t addr = base + p * PAGE_SIZE;
            flash.read(&candidate, addr, sizeof(FlashPage));

            if (calculateChecksum(candidate) == candidate.checksum) {
                if (!found || candidate.index > maxIndex) {
                    maxIndex = candidate.index;
                    memcpy(&out, &candidate, sizeof(FlashPage));
                    found = true;
                }
            }
        }
    }

    flash.deinit();
    return found;
}

void UserManager::init() {
    FlashPage latest;
    if (!readLatestPage(latest)) {
        // No valid page, use default
        memset(passwords, 0, sizeof(passwords));
        strncpy(passwords[0], adminPassword, 8);
        writeNewPage(0);
    } else {
        memcpy(passwords, latest.passwords, sizeof(passwords));
    }
}

void UserManager::writeNewPage(uint32_t prevIndex) {
    FlashIAP flash;
    flash.init();

    FlashPage page{};
    page.index = prevIndex + 1;
    memcpy(page.passwords, passwords, sizeof(passwords));
    page.checksum = calculateChecksum(page);

    uint32_t pageSlot = page.index % NUM_PAGES;
    uint32_t sector = pageSlot / 4;
    uint32_t pageInSector = pageSlot % 4;

    uint32_t base = (sector == 0) ? SECTOR0_ADDR : SECTOR1_ADDR;
    uint32_t addr = base + pageInSector * PAGE_SIZE;

    // Only erase if writing to first page in sector
    if (pageInSector == 0) {
        flash.erase(base, SECTOR_SIZE);
    }

    flash.program(&page, addr, sizeof(FlashPage));
    flash.deinit();
}

bool UserManager::verifyAdminPassword(const char* password) {
    return strcmp(password, passwords[0]) == 0;
}

bool UserManager::verifyUserPassword(const char* password) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (strlen(passwords[i]) >= 4 && strcmp(password, passwords[i]) == 0) {
            return true;
        }
    }
    return false;
}

void UserManager::storeUserPassword(int userIndex, const char* password) {
    FlashPage latest;
    if (!readLatestPage(latest)) {
        memset(passwords, 0, sizeof(passwords));
        strncpy(passwords[0], adminPassword, 8);
    } else {
        memcpy(passwords, latest.passwords, sizeof(passwords));
    }

    strncpy(passwords[userIndex], password, 8);
    passwords[userIndex][8] = '\0';

    writeNewPage(latest.index);
}

void UserManager::deleteUser(int userIndex) {
    storeUserPassword(userIndex, "");  // blank means deleted
}
