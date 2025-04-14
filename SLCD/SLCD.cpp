#include "SLCD.h"
#include "LCDconfig.h"


const uint8_t WF_ORDERING_TABLE[ ] = {
    CHAR1a,   // LCD81 --- Pin:5  LCDnAddress=51
    CHAR1b,   // LCD82 --- Pin:6  LCDnAddress=52
    CHAR2a,   // LCD83 --- Pin:7  LCDnAddress=53
    CHAR2b,   // LCD84 --- Pin:8  LCDnAddress=54
    CHAR3a,   // LCD85 --- Pin:9  LCDnAddress=55
    CHAR3b,   // LCD86 --- Pin:10 LCDnAddress=56
    CHAR4a,   // LCD87 --- Pin:11 LCDnAddress=57
    CHAR4b,   // LCD88 --- Pin:12 LCDnAddress=58
    CHARCOM0, // LCD77 --- Pin:1  LCDnAddress=4D
    CHARCOM1, // LCD78 --- Pin:2  LCDnAddress=4E
    CHARCOM2, // LCD79 --- Pin:3  LCDnAddress=4F
    CHARCOM3, // LCD80 --- Pin:4  LCDnAddress=50
};

const char ASCII_TO_WF_CODIFICATION_TABLE [ ] = {

    /*
                   segA
                 ________
                |        |
           segF |        | segB
                |        |
                 -segG--
                |        |
           segE |        | segC
                |________|
                   segD
    */

    ( SEGD+ SEGE+ SEGF+!SEGG) , ( SEGC+ SEGB+ SEGA) ,//Char = 0,   offset=0
    (!SEGD+!SEGE+!SEGF+!SEGG) , ( SEGC+ SEGB+!SEGA) ,//Char = 1,   offset=4
    ( SEGD+ SEGE+!SEGF+ SEGG) , (!SEGC+ SEGB+ SEGA) ,//Char = 2,   offset=8
    ( SEGD+!SEGE+!SEGF+ SEGG) , ( SEGC+ SEGB+ SEGA) ,//Char = 3,   offset=12
    (!SEGD+!SEGE+ SEGF+ SEGG) , ( SEGC+ SEGB+!SEGA) ,//Char = 4,   offset=16
    ( SEGD+!SEGE+ SEGF+ SEGG) , ( SEGC+!SEGB+ SEGA) ,//Char = 5,   offset=20
    ( SEGD+ SEGE+ SEGF+ SEGG) , ( SEGC+!SEGB+ SEGA) ,//Char = 6,   offset=24
    (!SEGD+!SEGE+!SEGF+!SEGG) , ( SEGC+ SEGB+ SEGA) ,//Char = 7,   offset=28
    ( SEGD+ SEGE+ SEGF+ SEGG) , ( SEGC+ SEGB+ SEGA) ,//Char = 8,   offset=32
    ( SEGD+!SEGE+ SEGF+ SEGG) , ( SEGC+ SEGB+ SEGA) ,//Char = 9,   offset=36
    (!SEGD+!SEGE+!SEGF+!SEGG) , (!SEGC+!SEGB+!SEGA) ,//Char = :,   offset=40
    (!SEGD+!SEGE+!SEGF+!SEGG) , (!SEGC+!SEGB+!SEGA) ,//Char = ;,   offset=44
    (!SEGD+!SEGE+!SEGF+!SEGG) , (!SEGC+!SEGB+!SEGA) ,//Char = <,   offset=48
    ( SEGD+!SEGE+!SEGF+ SEGG) , (!SEGC+!SEGB+!SEGA) ,//Char = =,   offset=52
    (!SEGD+!SEGE+!SEGF+!SEGG) , (!SEGC+!SEGB+!SEGA) ,//Char = >,   offset=56
    (!SEGD+!SEGE+!SEGF+!SEGG) , (!SEGC+!SEGB+ SEGA) ,//Char = ?,   offset=60
    ( SEGD+ SEGE+ SEGF+ SEGG) , ( SEGC+ SEGB+ SEGA) ,//Char = @,   offset=64
    (!SEGD+ SEGE+ SEGF+ SEGG) , ( SEGC+ SEGB+ SEGA) ,//Char = A,   offset=68
    ( SEGD+ SEGE+ SEGF+ SEGG) , ( SEGC+!SEGB+!SEGA) ,//Char = B,   offset=72
    ( SEGD+ SEGE+ SEGF+!SEGG) , (!SEGC+!SEGB+ SEGA) ,//Char = C,   offset=76
    ( SEGD+ SEGE+!SEGF+ SEGG) , ( SEGC+ SEGB+!SEGA) ,//Char = D,   offset=80
    ( SEGD+ SEGE+ SEGF+ SEGG) , (!SEGC+!SEGB+ SEGA) ,//Char = E,   offset=84
    (!SEGD+ SEGE+ SEGF+ SEGG) , (!SEGC+!SEGB+ SEGA) ,//Char = F,   offset=88
    ( SEGD+ SEGE+ SEGF+ SEGG) , ( SEGC+!SEGB+ SEGA) ,//Char = G,   offset=92
    (!SEGD+ SEGE+ SEGF+ SEGG) , ( SEGC+ SEGB+!SEGA) ,//Char = H,   offset=96
    (!SEGD+!SEGE+!SEGF+!SEGG) , ( SEGC+!SEGB+!SEGA) ,//Char = I,   offset=100
    ( SEGD+ SEGE+!SEGF+!SEGG) , ( SEGC+ SEGB+!SEGA) ,//Char = J,   offset=104
    (!SEGD+ SEGE+ SEGF+ SEGG) , ( SEGC+!SEGB+ SEGA) ,//Char = K,   offset=108
    ( SEGD+ SEGE+ SEGF+!SEGG) , (!SEGC+!SEGB+!SEGA) ,//Char = L,   offset=112
    (!SEGD+ SEGE+ SEGF+!SEGG) , ( SEGC+ SEGB+ SEGA) ,//Char = M,   offset=116
    (!SEGD+ SEGE+!SEGF+ SEGG) , ( SEGC+!SEGB+!SEGA) ,//Char = N,   offset=120
    ( SEGD+ SEGE+!SEGF+ SEGG) , ( SEGC+!SEGB+!SEGA) ,//Char = O,   offset=124
    (!SEGD+ SEGE+ SEGF+ SEGG) , (!SEGC+ SEGB+ SEGA) ,//Char = P,   offset=128
    ( SEGD+!SEGE+ SEGF+ SEGG) , ( SEGC+ SEGB+ SEGA) ,//Char = Q,   offset=132
    (!SEGD+ SEGE+!SEGF+ SEGG) , (!SEGC+!SEGB+!SEGA) ,//Char = R,   offset=136
    ( SEGD+!SEGE+ SEGF+ SEGG) , ( SEGC+!SEGB+ SEGA) ,//Char = S,   offset=140
    ( SEGD+ SEGE+ SEGF+ SEGG) , (!SEGC+!SEGB+!SEGA) ,//Char = T,   offset=144
    ( SEGD+ SEGE+ SEGF+!SEGG) , ( SEGC+ SEGB+!SEGA) ,//Char = U,   offset=148
    ( SEGD+ SEGE+!SEGF+!SEGG) , ( SEGC+!SEGB+!SEGA) ,//Char = V,   offset=152
    ( SEGD+ SEGE+ SEGF+!SEGG) , ( SEGC+ SEGB+!SEGA) ,//Char = W,   offset=156
    (!SEGD+ SEGE+ SEGF+ SEGG) , ( SEGC+ SEGB+!SEGA) ,//Char = X,   offset=160
    ( SEGD+!SEGE+ SEGF+ SEGG) , ( SEGC+ SEGB+!SEGA) ,//Char = Y,   offset=164
    ( SEGD+!SEGE+!SEGF+ SEGG) , (!SEGC+!SEGB+!SEGA) ,//Char = Z,   offset=168
    ( SEGD+!SEGE+!SEGF+!SEGG) , (!SEGC+!SEGB+ SEGA) ,//Char = [,   offset=172
    ( SEGD+!SEGE+!SEGF+!SEGG) , (!SEGC+!SEGB+ SEGA) ,//Char = \,   offset=176
    ( SEGD+!SEGE+!SEGF+!SEGG) , (!SEGC+!SEGB+ SEGA) ,//Char = ],   offset=180
    ( SEGD+!SEGE+!SEGF+!SEGG) , (!SEGC+!SEGB+ SEGA) ,//Char = ^,   offset=184
    ( SEGD+!SEGE+!SEGF+!SEGG) , (!SEGC+!SEGB+ SEGA) ,//Char = _,   offset=188
    ( SEGD+!SEGE+!SEGF+!SEGG) , (!SEGC+!SEGB+ SEGA) ,//Char = `,   offset=192
    ( !SEGD+!SEGE+!SEGF+SEGG) , (!SEGC+!SEGB+ !SEGA),//Char = -,   offset=194
};

SLCD::SLCD()
{
    init();
    CharPosition = 0;
}

void SLCD::init()
{
    SIM->SCGC5 |= SIM_SCGC5_SLCD_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

    // configure pins for LCD operation
    PORTC->PCR[20] = 0x00000000;     //VLL2
    PORTC->PCR[21] = 0x00000000;     //VLL1
    PORTC->PCR[22] = 0x00000000;     //VCAP2
    PORTC->PCR[23] = 0x00000000;     //VCAP1
    // Enable IRCLK
    MCG->C1  |= MCG_C1_IRCLKEN_MASK | MCG_C1_IREFSTEN_MASK;
    MCG->C2  |= MCG_C2_IRCS_MASK ;  //0 32KHZ internal reference clock; 1= 4MHz irc

    //Check if the Fast reference has its divide by 2 enabled (default):
    if ((MCG->SC & MCG_SC_FCRDIV_MASK) != 1<<1)
        error("Invalid clock configuration for SLCD\n");
    LCD->GCR = 0x0;
    LCD->AR  = 0x0;

    // LCD configurartion
    LCD->GCR =   ( LCD_GCR_RVEN_MASK*_LCDRVEN
                   | LCD_GCR_RVTRIM(_LCDRVTRIM)         //0-15
                   | LCD_GCR_CPSEL_MASK*_LCDCPSEL
                   | LCD_GCR_LADJ(_LCDLOADADJUST)       //0-3
                   | LCD_GCR_VSUPPLY_MASK*_LCDSUPPLY    //0-1
                   |!LCD_GCR_FDCIEN_MASK
                   | LCD_GCR_ALTDIV(1)         //divide by something
                   |!LCD_GCR_LCDDOZE_MASK
                   |!LCD_GCR_LCDSTP_MASK
                   |!LCD_GCR_LCDEN_MASK                 //WILL BE ENABLE ON SUBSEQUENT STEP
                   | LCD_GCR_SOURCE_MASK*1
                   | LCD_GCR_ALTSOURCE_MASK*0
                   | LCD_GCR_LCLK(0)    //0-7
                   | LCD_GCR_DUTY(_LCDDUTY)   //0-7
                 );
    uint8_t i;
    uint32_t *p_pen;
    uint8_t pen_offset;   // 0 or 1
    uint8_t pen_bit;      // 0 to 31
    LCD->PEN[0] = 0x0;
    LCD->PEN[1] = 0x0;
    LCD->BPEN[0] = 0x0;
    LCD->BPEN[1] = 0x0;
    p_pen = (uint32_t *)&LCD->PEN[0];
    for (i=0; i<_LCDUSEDPINS; i++) {
        pen_offset = WF_ORDERING_TABLE[i]/32;
        pen_bit    = WF_ORDERING_TABLE[i]%32;
        p_pen[pen_offset] |= 1 << pen_bit;
        if (i>= _LCDFRONTPLANES) {  // Pin is a backplane
            p_pen[pen_offset+2] |= 1 << pen_bit;  // Enable  BPEN
            LCD->WF8B[(uint8_t)WF_ORDERING_TABLE[i]] = 1 << (i - _LCDFRONTPLANES);   // fill with 0x01, 0x02, etc
        }
    }
    LCD->GCR |= LCD_GCR_LCDEN_MASK;
}

int SLCD::_putc(int c)
{
    Write_Char(c);
    return 0;
}

void SLCD::Write_Char (char lbValue)
{
    if (CharPosition >= _CHARNUM)
        CharPosition = 0;

    if (lbValue == '.') {
        // Use built-in dot
        DP(CharPosition-1, true);
        return;
    }

    uint8_t char_val;
    uint8_t temp;
    uint8_t *lbpLCDWF;
    uint8_t lbCounter;
    uint16_t arrayOffset;
    uint8_t position;

    lbpLCDWF = (uint8_t *)&LCD->WF8B[0];
    /* only ascii character if value not writeable write as @ */
    if (lbValue>='a' && lbValue<='z') {
        lbValue -= 32; // UpperCase
    }
    if (lbValue<ASCCI_TABLE_START || lbValue >ASCCI_TABLE_END) {
        lbValue = BLANK_CHARACTER;  // default value as space
    }
    lbValue -=ASCCI_TABLE_START;        // Remove the offset to search in the ascci table
    arrayOffset = (lbValue * _CHAR_SIZE); // Compensate matrix offset
    // ensure bLCD position is in valid limit
    lbCounter = 0;  //number of writings to complete one char
    while (lbCounter<_CHAR_SIZE) {
        position = (CharPosition) *_LCDTYPE + lbCounter;
        temp=0;
        if (lbCounter==1) {
            temp = lbpLCDWF[WF_ORDERING_TABLE[position]] & 0x01;//bit 0 has the special symbol information
        }
        char_val = ASCII_TO_WF_CODIFICATION_TABLE[arrayOffset + lbCounter];
        lbpLCDWF[WF_ORDERING_TABLE[position]] = char_val | temp;
        //  if (char_val==0) lbCounter = _CHAR_SIZE; //end of this character
        lbCounter++;
    }
    CharPosition++;
}

void SLCD::Home()
{
    CharPosition =  0;
}

void SLCD::Contrast (uint8_t lbContrast)
{
    lbContrast &= 0x0F;              //Forced to the only values accepted
    LCD->GCR |= LCD_GCR_RVTRIM(lbContrast);
}

void SLCD::clear()
{
    All_Segments(0);
}

void SLCD::All_Segments (int mode)
{
    uint8_t lbTotalBytes = _CHARNUM * _LCDTYPE;
    uint8_t lbCounter=0;
    uint8_t *lbpLCDWF;

    lbpLCDWF = (uint8_t *)&LCD->WF8B[0];
    while (lbCounter < lbTotalBytes) {
        if (mode==1) {
            lbpLCDWF[(uint8_t)WF_ORDERING_TABLE[lbCounter++]]=_ALLON;
        } else {
            lbpLCDWF[WF_ORDERING_TABLE[lbCounter++]]=0;
        }
    }
}

void SLCD::DP(int pos, bool on)
{
    uint8_t *lbpLCDWF;
    int tableLoc;
    switch (pos) {
        case 0:
            tableLoc = 1;
            break;
        case 1:
            tableLoc = 3;
            break;
        case 2:
            tableLoc = 5;
            break;
        default:
            return; // Bad position
    }
    lbpLCDWF = (uint8_t *)&LCD->WF8B[0];
    if (on) {
        lbpLCDWF[(uint8_t)WF_ORDERING_TABLE[tableLoc]]|=1;
    } else {
        lbpLCDWF[(uint8_t)WF_ORDERING_TABLE[tableLoc]]&=~1;
    }
}

void SLCD::DP1 (int mode)
{
    DP(0, mode==1);
}

void SLCD::DP2 (int mode)
{
    DP(1, mode==1);
}

void SLCD::DP3 (int mode)
{
    DP(2, mode==1);
}

void SLCD::Colon (bool on)
{
    uint8_t *lbpLCDWF;
    lbpLCDWF = (uint8_t *)&LCD->WF8B[0];
    if (on) {
        lbpLCDWF[(uint8_t)WF_ORDERING_TABLE[7]]|=1;
    } else {
        lbpLCDWF[(uint8_t)WF_ORDERING_TABLE[7]]&=~1;
    }
}

void SLCD::blink(int blink)
{
    if (( blink > 7) || (blink < 0))
        LCD->AR &= ~LCD_AR_BLINK_MASK;
    else
        LCD->AR |= LCD_AR_BLINK_MASK | blink;
}

void SLCD::deepsleepEnable(bool enable)
{
    MCG->C1 &= ~MCG_C1_IREFSTEN_MASK;
    MCG->C1 |= enable << MCG_C1_IREFSTEN_SHIFT;
    LCD->GCR &= ~LCD_GCR_LCDSTP_MASK;
    LCD->GCR |= (!enable) << LCD_GCR_LCDSTP_SHIFT;
}