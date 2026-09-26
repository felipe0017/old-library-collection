// MATH TOOLS

S64 MAX(S64 A, S64 B){
	RETURN (A>B)? A: B;
}

ATYPE MIN(ATYPE A, ATYPE B){
	RETURN (A<B)? A: B;
}

// ADJUST A VALUE FROM RANGE 'A' TO RANGE 'B'
// IA, FA -> INITIAL & FINAL VALUES OF RANGE 'A'
// IB, FB -> INITIAL & FINAL VALUES OF RANGE 'B'
ATYPE MAP(ATYPE VALUE, ATYPE IA, ATYPE FA, ATYPE IB, ATYPE FB){
	ATYPE R;
	R = ((VALUE-IA) * 256) / (FA-IA);
	R = (((FB-IB) * R) / 256) + IB;
	RETURN R;
}

// MIN VALUES ARE ZERO
ATYPE MAP(ATYPE VALUE, ATYPE FA, ATYPE FB){
	RETURN (VALUE * FB) / FA;
}

ATYPE SGN(ATYPE A){
	RETURN (A < 0)? -1 : +1;
}

U8 SGNX(ATYPE A){
	RETURN (A < 0);
}

ATYPE INVSIGN(ATYPE A){
	RETURN ~A + 1;
}

ATYPE SETSIGN(ATYPE A, ATYPE S){
	IF(SGN(A) != S)
		RETURN INVSIGN(A);
	RETURN A;
}

ATYPE SETSIGNX(ATYPE A, U8 S){
	IF(SGNX(A) != S)
		RETURN INVSIGN(A);
	RETURN A;
}

ATYPE FACTORIAL(U8 A){
	ATYPE R=1;
	FOR(U8 i = 0; i < A; i++)
		R = R * i;
	RETURN R;
}

// X^2
ATYPE SQ(ATYPE A){
	RETURN ATYPE(A) * A;
}

// X^3
ATYPE CB(ATYPE A){
	RETURN ATYPE(A) * SQ(A);
}

// 2^X, X AS POSITIVE INTEGER
ATYPE EXP2(U8 A){
	ATYPE R=1;
	R = R << A;
	RETURN R;
}

ATYPE BIT_SHIFTR(ATYPE N, ATYPE SD){
	RETURN (N >> SD);
}

ATYPE BIT_SHIFTL(ATYPE N, ATYPE SD){
	RETURN (N << SD);
}

U8 X8BIT_ROTATER(U8 N, U8 SD){
	U8 R;
	R = (N >> SD) | ((N << (8 - SD)));
	RETURN R;
}

U16 X16BIT_ROTATER(U16 N, U16 SD){
	U16 R;
	R = (N >> SD) | ((N << (16 - SD)));
	RETURN R;
}

U32 X32BIT_ROTATER(U32 N, U32 SD){
	U32 R;
	R = (N >> SD) | ((N << (32 - SD)));
	RETURN R;
}

U64 X64BIT_ROTATER(U64 N, U64 SD){
	U64 R;
	R = (N >> SD) | ((N << (64 - SD)));
	RETURN R;
}

U8 X8BIT_ROTATEL(U8 N, U8 SD){
	U8 R;
	R = (N << SD) | (N >> (8 - SD));
	RETURN R;
}

U16 X16BIT_ROTATEL(U16 N, U16 SD){
	U16 R;
	R = (N << SD) | (N >> (16 - SD));
	RETURN R;
}

U32 X32BIT_ROTATEL(U32 N, U32 SD){
	U32 R;
	R = (N << SD) | (N >> (32 - SD));
	RETURN R;
}

U64 X64BIT_ROTATEL(U64 N, U64 SD){
	U64 R;
	R = (N << SD) | (N >> (64 - SD));
	RETURN R;
}

VOID SWAP(ATYPE &A, ATYPE &B){
	ATYPE AUX;
	AUX = A;
	A = B;
	B = AUX;
}

TEMPLATE <TYPENAME T>
VOID SWAP2(T &A, T &B){
	T AUX;
	AUX = A;
	A = B;
	B = AUX;
}

BOOL ISPAIR(ATYPE A){
	RETURN !(A & 0X1);
}

BOOL ISODD(ATYPE A){
	RETURN (A & 0X1);
}

VOID DMOD(ATYPE A, ATYPE B, ATYPE &MODULE, ATYPE &REST){
	MODULE = A / B;
	REST   = A - B*MODULE;
}

ATYPE ABS(ATYPE A){
	RETURN (A < 0)? INVSIGN(A) : A;
}

// INCLUDE EXTREMES
BOOL IN_RANGE(ATYPE NUMBER, ATYPE MIN_EXTREME, ATYPE MAX_EXTREME){
	RETURN (NUMBER >= MIN_EXTREME AND NUMBER <= MAX_EXTREME);
}

#include <stdlib.h>     /* srand, rand */
ATYPE RANDOM(ATYPE I, ATYPE F){
	RETURN ( rand()%F + I );
}

ATYPE PURE_MUL(ATYPE A, ATYPE B){
	ATYPE R=0;
	S8 S, SA, SB;
	SA = SGNX(A);
	SB = SGNX(B);
	S = SA XOR SB;
	A = ABS(A);
	B = ABS(B);
	FOR(ATYPE I=0; I<B; I++){
		R = R + A;
	}
	SETSIGNX(R, S);
	RETURN R;
}

ATYPE MUL(ATYPE A, ATYPE B){
	ATYPE R=0;
	FOR(U8 I=0; I<64; I++){
		BOOL OPERAND = (B >> I) & 0X1;
		ATYPE RT = A & ((OPERAND)? 0XFFFFFFFF : 0X0);
		R += (RT << I);
	}
	RETURN R;
}

// RETURN THE MODULE
ATYPE PURE_DIV(ATYPE A, ATYPE B, ATYPE &R){
	ATYPE I, VALUE;
	I = 0;
	VALUE = 0;
	WHILE(1){
		VALUE = VALUE + B;
		IF(VALUE == A) { R = I + 1; RETURN 0;         }
		IF(VALUE >  A) { R = I;     RETURN A - B * I; }
		I++;
	}
}

U8 USELESS_ZEROS(ATYPE N){
	IF(N < 0) RETURN 0;
	
	U8 COUNTER = 0;
	ATYPE NN;
	FOR(U8 i=0; i<8; i++){
		// BYTE CHECK
		NN = N >> ((7-i) * 8);
		
		IF((NN & 0xFF) != 0){
			FOR(U8 j=0; j<8; j++){
				BOOL BIT;
				BIT = (NN >> (7-j)) & 1;
				IF(!BIT) COUNTER++;
				ELSE RETURN COUNTER;
			}
		}
		ELSE COUNTER = COUNTER + 8;
	}
	RETURN COUNTER;
}

// RETURN THE POSITION OF THE BIGGER HIGH BIT
U8 BIT_LENGTH(ATYPE N){
	RETURN ( 64 - USELESS_ZEROS(N) );
}

// HOW MANY BYTES ARE NEEDED TO STORE A NUMBER OF BITS
ATYPE GET_BYTE_LENGTH(ATYPE BITS_LENGTH){
	RETURN (BITS_LENGTH % 8? BITS_LENGTH / 8 + 1 : BITS_LENGTH / 8);
}

// RETURN THE MODULE
ATYPE DIV(ATYPE A, ATYPE B, ATYPE &R){
	// QUICK VALUES
	IF(A < B) {R=0; RETURN A;}
	IF(A==B)  {R=1; RETURN 0;}
	
	ATYPE S, SA, SB;
	SA = SGNX(A);
	SB = SGNX(B);
	S = SA XOR SB;
	A = ABS(A);
	B = ABS(B);
	ATYPE _R=0;
	ATYPE MT=0; // SUMA DE MODULOS
	FOR(U8 i=0; i<BIT_LENGTH(A); i++){
		ATYPE OPERAND = ((A >> i) & 0X1) << i;
		ATYPE RT,M;
		M = PURE_DIV(OPERAND, B, RT);
		
		_R  = _R  + RT;
		MT = MT + M;
	}
	ATYPE I; // INTEGER TO ADD
	MT = PURE_DIV(MT, B, I);
	_R += I;
	R = SETSIGNX(_R, S);
	RETURN MT;
}

ATYPE MOD(ATYPE A, ATYPE B){
	ATYPE R,X;
	R = DIV(A, B, X);
	RETURN R;
}

ATYPE POW(ATYPE B, ATYPE E){
	ATYPE R = 1;
	FOR(ATYPE i=0; i<E; i++){
		R = R * B;
	}
	RETURN R;
}

// 10*X
ATYPE BY10(ATYPE A){
	RETURN (A << 3) + (A << 1);
}

// 10^X
ATYPE POW10(U8 A){
	RETURN POW10TAB[A];
}

//GET DECIMAL LENGTH
U8 DECIMAL_LENGTH(ATYPE N){
	N = ABS(N);
	U8 LENGTH = 1;
	ATYPE EXP = 1;
	WHILE(N >= EXP){
		LENGTH++;
		EXP = EXP * 10;
	}
	RETURN LENGTH;
}

// RETURN THE NUMBER OF HIGH BITS
U8 BIT_COUNT(ATYPE N){
	U8 COUNTER = 0;
	ATYPE NN;
	for(U8 i=0; i<64; i++){
		NN = N >> i;
		BOOL BIT;
		BIT = NN & 1;
		IF(BIT) COUNTER++;
	}
	RETURN COUNTER;
}

ATYPE ROOT(ATYPE I, ATYPE R){
	IF(R < 0 && ISPAIR(I)){
		RETURN 0;
	}
	// QUICK VALUES
	IF(R == 0) RETURN  0;
	IF(R == 1) RETURN  1;
	
	ATYPE i=1;
	ATYPE T; // TEST VALUE
	WHILE(1){
		T = POW(i,I);
		IF(T == R) RETURN i;
		IF(T >  R) RETURN i-1;
		i++;
	}
}

ATYPE PURE_SQRT(ATYPE A){
	
	// QUICK VALUES
	IF(A == 0) RETURN  0;
	IF(A == 1) RETURN  1;
	
	ATYPE i;
	// QUICK JUMPS
	U8 LENGTH = BIT_LENGTH(A);
	i = SQRTTAB[MAX(LENGTH-1, 0)];
	
	ATYPE T; // TEST VALUE
	WHILE(1){
		T = i * i;
		IF(T == A) RETURN i;
		IF(T >  A) RETURN i-1;
		i++;
	}
}

ATYPE PURE_iSQRT(ATYPE A, ATYPE START_ITERATOR){
	
	// QUICK VALUES
	IF(A == 0) RETURN  0;
	IF(A == 1) RETURN  1;
	
	ATYPE i=START_ITERATOR;
	
	ATYPE T; // TEST VALUE
	WHILE(1){
		T = i * i;
		IF(T == A) RETURN i;
		IF(T >  A) RETURN i-1;
		i++;
	}
}

ATYPE SQRT(ATYPE A){
	IF(A < 0){
		RETURN 0;
	}
	
	U8 LENGTH;
	LENGTH = BIT_LENGTH(A);
	ATYPE B,C,F,D, CL;
	B = A >> (LENGTH/2);
	C = EXP2(LENGTH - BIT_LENGTH(B));
	
	B = PURE_SQRT(B);
	CL = BIT_LENGTH(C);
	C = EXP2((CL-1)/2);
	
	D = B * C;
	IF(SQ(D) == A) RETURN D;
	A = PURE_iSQRT(A, D);
	
	RETURN A;
}

ATYPE PURE_CBRT(ATYPE A){
	ATYPE i;
	// QUICK JUMPS
	U8 LENGTH = BIT_LENGTH(A);
	i = CBRTTAB[MAX(LENGTH-1, 0)];
	
	ATYPE T; // TEST VALUE
	WHILE(1){
		T = i * i * i;
		IF(T == A) RETURN i;
		IF(T >  A) RETURN i - 1;
		i++;
	}
}

ATYPE PURE_iCBRT(ATYPE A, ATYPE START_ITERATOR){
	ATYPE i=START_ITERATOR;
	
	ATYPE T; // TEST VALUE
	WHILE(1){
		T = i * i * i;
		IF(T == A) RETURN i;
		IF(T >  A) RETURN i - 1;
		i++;
	}
}

ATYPE CBRT(ATYPE A){
	
	IF(A == 0) RETURN  0;
	IF(A == 1) RETURN  ( (A<0)? -1 : +1);
	
	U8 S;
	S = SGNX(A);
	A = ABS(A);
	
	U8 LENGTH;
	LENGTH = BIT_LENGTH(A);
	ATYPE B,C,F,D, CL;
	B = A >> (LENGTH/2);
	C = EXP2(LENGTH - BIT_LENGTH(B));
	
	B = PURE_CBRT(B);
	C = PURE_CBRT(C);
	
	D = B * C;
	IF(CB(D) == A) RETURN SETSIGN(D, S);
	A = PURE_iCBRT(A, D);
	
	RETURN SETSIGNX(A, S);
}

U8 LOG10(ATYPE ARG){
	
	IF(ARG < 1){
		RETURN 0;
	}
	
	// QUICK VALUES
	IF(ARG < 10) RETURN 0;
	
	ATYPE R=1;// TEST VALUE
	U8 I=0; // ITERATOR
	WHILE(1){
		R = MUL(R, 10);
		IF(!(R <= ARG)) break;
		I++;
	}
	RETURN I;
}

U8 LOG2(ATYPE ARG){
	IF(ARG < 2){
		RETURN 0;
	}
	
	ATYPE TV=1; // TEST VALUE
	FOR(U8 I=0; I<ABITS; I++){
		TV = TV << 1;
		IF(!(TV < ARG)){
			RETURN (TV > ARG)? I : I+1;
		}
	}
}
