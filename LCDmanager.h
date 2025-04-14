#ifndef LCDMANAGER_H
#define LCDMANAGER_H

#include "mbed.h"
#include "SLCD.h"  // Segment LCD (4-digit)

class LCDManager {
public:
    explicit LCDManager(SLCD& lcdRef);

    void print(const char* text);
    void scrollText(const char* text, int scrollDelay = 200);
    void scrollMessageBlocking(const char* text, int scrollDelay = 200);

private:
    SLCD& lcd;
    char currentScrollText[32];
    int scrollIndex;
    uint64_t lastScrollTime;
};

#endif // LCDMANAGER_H
