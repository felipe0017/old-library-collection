// TYPEDEF FIXED WIDTH DATA & LIMITS

TYPEDEF unsigned char           U8;
TYPEDEF char                    S8;
TYPEDEF unsigned short int      U16;
TYPEDEF short int               S16;
TYPEDEF unsigned int            U32;
TYPEDEF int                	    S32;
TYPEDEF unsigned long long int  U64;
TYPEDEF long long int           S64;
TYPEDEF bool                    BOOL;

// MAX, MIN
#define U8_MIN 0
#define U8_MAX 255
#define S8_MIN -128
#define S8_MAX 127
#define U16_MIN 0
#define U16_MAX 65535
#define S16_MIN -32768
#define S16_MAX 32767
#define U32_MIN 0LL
#define U32_MAX 4294967295LL
#define S32_MIN -2147483648LL
#define S32_MAX 2147483647LL
#define U64_MIN 0ULL
#define U64_MAX 18446744073709551615ULL
#define S64_MIN -9223372036854775807LL - 1LL
#define S64_MAX 9223372036854775807LL
