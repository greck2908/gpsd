/*
 * bits.h - extract binary data from message buffer
 * * These macros extract bytes, words, longwords, floats or doubles from
 * a message that contains these items in MSB-first byte order.
 * By defining the GET_ORIGIN and PUT_ORIGIN macros, it's possible to
 * change the origin of the indexing.
 *
 * assumptions:
 *  char is 8 bits, short is 16 bits, int is 32 bits, long long is 64 bits,
 *  float is 32 bits IEEE754, double is 64 bits IEEE754.
 *
 * it would be possible to use types like int16_t from header files to enforce
 * these assumptions, but splint does not understand those and will scream.
 * also, using such explicitly sized types usually causes warnings at many
 * other places in a program (like when calling library routines).  we will
 * need to consider this again when we want to port to an architecture which
 * implements differently sized types.  Noth 32- and 64-bit systems with
 * gcc are OK.
 */

union int_float {
    int i;
    float f;
};

union long_double {
    long long l;
    double d;
};

#ifndef GET_ORIGIN
#define GET_ORIGIN	0
#endif

#define getsb(buf, off)	((char)buf[GET_ORIGIN+(off)])
#define getub(buf, off)	(buf[GET_ORIGIN+(off)])
#define getsw(buf, off)	((short)(((unsigned)getub(buf, off) << 8) | (unsigned)getub(buf, off+1)))
#define getuw(buf, off)	((unsigned short)(((unsigned)getub(buf, off) << 8) | (unsigned)getub(buf, off+1)))
#define getsl(buf, off)	((int)(((unsigned)getuw(buf, off) << 16) | getuw(buf, off+2)))
#define getul(buf, off)	((unsigned int)(((unsigned)getuw(buf, off) << 16) | getuw(buf, off+2)))
#define getsL(buf, off)	((long long)(((unsigned long long)getul(buf, off) << 32) | getul(buf, off+4)))
#define getuL(buf, off)	((unsigned long long)(((unsigned long long)getul(buf, off) << 32) | getul(buf, off+4)))
#define getf(buf, off)	(i_f.i = getsl(buf, off), i_f.f)
#define getd(buf, off)	(l_d.l = getsL(buf, off), l_d.d)

#ifndef PUT_ORIGIN
#define PUT_ORIGIN	0
#endif

#define putbyte(buf,off,b) {buf[PUT_ORIGIN+(off)] = (unsigned char)(b);}
#define putword(buf,off,w) {putbyte(buf,off,(w) >> 8); putbyte(buf,off+1,w);}
#define putlong(buf,off,l) {putword(buf,off,(l) >> 16); putword(buf,off+2,l);}
