#ifndef UTILS_H
#define UTILS_H

#define _GET(type,letter,bit) (type ## letter >> bit) & 1
#define _SET(type,letter,bit) type ## letter |= _BV(bit)
#define _CLEAR(type,letter,bit) type ## letter &= ~_BV(bit)
#define _TOGGLE(type,letter,bit) type ## letter ^= _BV(bit)

#define SETINPUT(pin) _CLEAR(DDR,pin)
#define SETOUTPUT(pin) _SET(DDR,pin)
#define SETBIT(pin) _SET(PORT,pin)
#define CLEARBIT(pin) _CLEAR(PORT,pin)
#define TOGGLEBIT(pin) _TOGGLE(PORT,pin)
#define READBIT(pin) _GET(PIN,pin)

#endif
