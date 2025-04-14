#pragma once
#include "mbed.h"
#include "Stream.h"
/*  ------ sample usage------

    #include "mbed.h"
    #include "SLCD.h"

    SLCD slcd;

    main()
    {
        slcd.printf("1.2.3.4"); // standard printf function, only charaters in ASCII_TO_WF_CODIFICATION_TABLE will display
                                // Dots printed using decimal points
        slcd.putc('A');         // prints a single character
        slcd.clear();           // All segments off
        slcd.All_Segments(y);   // y=1 for ALL segments on, 0 for ALL segments off
        slcd.DP(x, true/false); // Set/Clear decimal point. x=0, 1 or 2.
                                // Does nothing if invalid decimal point
        slcd.DPx(y);            // x=DP1 to DP3, y=1 for on 0 for off (legacy)
        slcd.Colon(y);          // y=1 for on, 0 for off
        slcd.CharPosition=x;    // x=0 to 3, 0 is start position
        slcd.Home();            // sets next charater to posistion 0 (start)
        slcd.Contrast (x);      // set contrast x=0 - 15, 0 lightest, 15 darkest
        slcd.blink(x);          // set display to blink, 0-7 is blink rate (default = 3), -1 disables blink
        slcd.deepsleepEnable(x);// true (default) keeps the lcd enabled in deepsleep, false disables its 4MHz internal oscillator clock. Total power consumption ~= 40uA
    }
*/

/**
* SLCD peripheral of the FRDM-KL46Z\n
Inherits mbed::Stream and supports the majority of Stream functions.

@code
#include "mbed.h"
#include "SLCD.h"

SLCD slcd;

main()
{
    slcd.Home();            // Moves cursor to position 0 (start)
    slcd.printf("1.2.3.4"); // Standard printf function, only characters in ASCII_TO_WF_CODIFICATION_TABLE will display
    // Dots printed using decimal points
    slcd.putc('A');         // Prints a single character
    
    while(1);   // Wait forever
}
@endcode
*/
class SLCD : public Stream
{
public:
    //! Construct an SLCD object
    SLCD();

    //! Move the SLCD cursor to the first character
    void Home();
    /**
    * Set contrast
    * @param lbContrast 0 - 15, 0 lightest, 15 darkest
    */
    void Contrast (uint8_t lbContrast);
    void All_Segments (int);
    //! Turn off all segments
    void clear();
    /**
    * Turn a decimal point on or off
    * @param pos decimal point position: 0-2
    * @param on True or False
    */
    void DP(int pos, bool on);
    void DP1 (int);
    void DP2 (int);
    void DP3 (int);
    //! Turn the colon symbol on or off
    void Colon (bool on);
    //! Current cursor position
    uint8_t CharPosition;
    void blink(int blink = 3);
    void deepsleepEnable(bool enable);

private:
    void Write_Char(char lbValue);
    void init();
    virtual int _putc(int c);
    virtual int _getc() {
        return 0;
    }
};