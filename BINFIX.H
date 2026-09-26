// FIXED POINT ARITHMETIC LIBRARY (32-bit)
// 16 BITS FOR INTEGER PART (HWORD), AND 16 BITS FOR FRACCIONARY PART (LWORD)
// 27-08-2021

// !despues de revisar hacer que TODAS las subrutinas acepten zeros, +-infinitos y no-definidos
// recordar mantener actualizado cuando un numero sea zero, por ejemplo el retorno de una suma entre +2.0 y -2.0
// retorna cero, ademos de retornar, antes de retornar modificar su SNI a 1.
// tambien para valores especiales, por ej: infinite -> -infinite
// tambien mantener actualizado sni para cero, positive number y negative number,
// todas las funciones que operen con binfix 32bits deben mantener actualizados los valores de arriba

#define BF_TYPE S32
#define BF_BIGTYPE S64

#define BF_HWORD S16
#define BF_LWORD U16

#define TWOPOW16 65536

#define BFSNI_POS    0
#define BFSNI_NEG    1
#define BFSNI_ZERO   2
#define BFSNI_INF    3
#define BFSNI_NEGINF 4
#define BFSNI_UNDEF  5


U8 BFMATRIX_ADD[6][6] = {
	{0, 0, 0, 3, 4, 5},
	{0, 1, 1, 3, 4, 5},
	{0, 1, 2, 3, 4, 5},
	{3, 3, 3, 3, 2, 5},
	{4, 4, 4, 2, 4, 5},
	{5, 5, 5, 5, 5, 5}
};

U8 BFMATRIX_MUL[6][6] = {
	{0, 1, 2, 3, 4, 5},
	{1, 0, 2, 4, 3, 5},
	{2, 2, 2, 2, 2, 5},
	{3, 4, 2, 3, 4, 5},
	{4, 3, 2, 4, 3, 5},
	{5, 5, 5, 5, 5, 5}
};

U8 BFMATRIX_DIV[6][6] = {
	{0, 1, 5, 2, 2, 5},
	{1, 0, 5, 2, 2, 5},
	{2, 2, 3, 2, 2, 5},
	{3, 4, 5, 3, 4, 5},
	{4, 3, 5, 4, 3, 5},
	{5, 5, 5, 5, 5, 5}
};

U8 BFMATRIX_ABS[6] = {0, 0, 0, 3, 3, 5};

U8 BFMATRIX_MIN[6][6] = {
	{0, 1, 0, 0, 4, 5},
	{1, 1, 1, 1, 4, 5},
	{2, 1, 2, 2, 4, 5},
	{0, 1, 2, 3, 4, 5},
	{4, 4, 4, 4, 4, 5},
	{5, 5, 5, 5, 5, 5}
};

U8 BFMATRIX_MAX[6][6] = {
	{0, 0, 0, 3, 0, 5},
	{0, 1, 2, 3, 1, 5},
	{0, 2, 2, 3, 2, 5},
	{3, 3, 3, 3, 3, 5},
	{0, 1, 2, 3, 4, 5},
	{5, 5, 5, 5, 5, 5}
};

U8 BFMATRIX_SQRT[6] = {0, 5, 2, 3, 5, 5};

U8 BFMATRIX_CBRT[6] = {0, 1, 2, 3, 4, 5};

U8 BFMATRIX_SIN[6] = {0, 0, 2, 5, 5, 5};

U8 BFMATRIX_ASIN[6] = {0, 1, 2, 5, 5, 5};

// en realidad, todas o casi todas las subrutinas necesitan una matriz para numeros especiales


struct BINFIX{
	public:
		U8 SNI; // SPECIAL NUMBER IDENTIFIER
		union{
			BF_TYPE DATA;
			// PARTS OF NUMBER
			struct{
				BF_LWORD LWORD;
				BF_HWORD HWORD;
			}P;
		};

	public:
		BINFIX(){
			this->SNI = 2;
			this->DATA = 0;
		}
		friend BINFIX MAKEBINFIX(BF_HWORD HWORD, BF_LWORD LWORD);
		friend BINFIX MAKEBINFIX(BF_HWORD HWORD);
		friend BINFIX MAKEBINFIXX(U8 SNI);
		
		friend void BREAK(BINFIX X, BF_HWORD &HWORD, BF_LWORD &LWORD);
		friend BINFIX SIGN(BINFIX X);
		friend S8 SGN(BINFIX X);
		friend BINFIX SETSIGN(BINFIX X, S8 SIGN);
		friend BINFIX SETSIGN(BINFIX X, BINFIX SIGN);
		friend BINFIX INVSIGN(BINFIX X);
		// IF X IS +-INFINITE OR UNDEFINED
		friend bool IS_SPECIAL(BINFIX X);
		friend bool IS_ZERO(BINFIX X);
		friend bool IS_NEG(BINFIX X);
		friend void UPDATE(BINFIX &X);
		
		// BASIC OPERATIONS
		friend BINFIX ADD(BINFIX X, BINFIX Y);
		friend BINFIX SUB(BINFIX X, BINFIX Y);
		// FOR MULTIPLICATIONS AND DIVISIONS BY POWER OF 2, USE LSHIFT & RSHIFT
		friend BINFIX MUL(BINFIX X, BINFIX Y);
		// IF 'Y' IS A CONSTANT, CONSIDER MULTIPLYING BY THE PRECALCULED RECIPROCAL OF Y
		friend BINFIX DIV(BINFIX X, BINFIX Y);
		friend BINFIX ABS(BINFIX X);
		friend BINFIX ABSDIF(BINFIX X, BINFIX Y); // POSITIVE DIFERENCE
		friend BINFIX MAD(BINFIX A, BINFIX B, BINFIX C); // A * B + C
		friend BINFIX MOD(BINFIX X, BINFIX Y);
		friend BINFIX MIN(BINFIX X, BINFIX Y);
		friend BINFIX MAX(BINFIX X, BINFIX Y);
		friend BINFIX TRUNC(BINFIX X);
		friend BINFIX REDON(BINFIX X);
		friend BINFIX NEXT_INT(BINFIX X);
		friend BINFIX PREV_INT(BINFIX X);
		friend BINFIX LSHIFT(BINFIX X, U8 Y); // = X * (2^Y)
		friend BINFIX RSHIFT(BINFIX X, U8 Y); // = X / (2^Y)
		friend BINFIX MAP(BINFIX VALUE, BINFIX IA, BINFIX FA, BINFIX IB, BINFIX FB); // SEE MAP IN MATH.H
		
		// CONVERSIONS
		friend BINFIX TORAD(BINFIX DEGREES);
  		friend BINFIX TODEG(BINFIX RADIANS);
  		friend BF_HWORD TO_INT(BINFIX X);
  		friend void FORMAT_BINDEG(BINFIX &X);
  		
  		// TRIGONOMETRY
  		friend BINFIX SINX(BINFIX X); // CALCULATE THE SINE OF RADIANS
  		friend BINFIX SIN(BINFIX X);  // THE REST, USE SEACH TABLES FOR FIRST QUADRANT OF SINE
		friend BINFIX COS(BINFIX X);
		friend BINFIX TAN(BINFIX X);
		friend BINFIX ASIN(BINFIX X);
		friend BINFIX ACOS(BINFIX X);
		friend BINFIX ATAN(BINFIX X);
		friend BINFIX ATAN2(BINFIX Y, BINFIX X);

		// EXPONENTIAL
		friend BINFIX EXP(BINFIX X);
		friend BINFIX LN(BINFIX X);
		friend BINFIX LOG(BINFIX X);
		friend BINFIX LOG2(BINFIX X);

		// POTENTIALS
		friend BINFIX INV(BINFIX X); // 1 / X
		friend BINFIX POW(BINFIX X, BF_TYPE Y);
		friend BINFIX POW(BINFIX X, BINFIX Y);
		friend BINFIX SQ(BINFIX X); // X^2
		friend BINFIX SQRT(BINFIX X);
		friend BINFIX INVSQRT(BINFIX X); // 1 / SQRT(X)
		friend BINFIX CB(BINFIX X); // X^3
		friend BINFIX CBRT(BINFIX X);
		friend BINFIX HYPOT(BINFIX X, BINFIX Y);
		friend BINFIX HYPOTX(BINFIX X, BINFIX Y); // RETURN C^2
		
		// COMPARISON
		friend bool ISEQUAL(BINFIX X, BINFIX Y);
		friend bool ISNOEQUAL(BINFIX X, BINFIX Y);
		friend bool ISMAYOR(BINFIX X, BINFIX Y);
		friend bool ISMINOR(BINFIX X, BINFIX Y);
		friend bool ISMAYOREQUAL(BINFIX X, BINFIX Y);
		friend bool ISMINOREQUAL(BINFIX X, BINFIX Y);

		friend void PRINT(BINFIX X);
};


BINFIX MAKEBINFIX(BF_HWORD HWORD, BF_LWORD LWORD){
	BINFIX X;
	X.P.HWORD = HWORD;
	X.P.LWORD = LWORD; //(((HWORD << 16) + LWORD * POW10(16)) << 16) / POW10(4);
	UPDATE(X);
	return X;
}

BINFIX MAKEBINFIX(BF_HWORD HWORD){
	BINFIX X;
	X.P.HWORD = HWORD;
	UPDATE(X);
	return X;
}

BINFIX MAKEBINFIXX(U8 SNI){
	BINFIX X;
	X.SNI = SNI;
	if(!IS_SPECIAL(X) and !IS_ZERO(X)) TEST(0);
	return X;
}


BINFIX BF_MAX     = MAKEBINFIX(S16_MAX, U16_MAX);  // = +32767.9999847412109375
BINFIX BF_MIN     = MAKEBINFIX(S16_MIN, U16_MAX);  // = -32768.9999847412109375
BINFIX BF_MIN_ABS = MAKEBINFIX(0, 1);              // = +0.0000152587890625

BINFIX BF_HALF   = MAKEBINFIX(0, 0x8000);   // = +0.5
BINFIX BF_180    = MAKEBINFIX(180);         // = +180.0
BINFIX BF_ZERO   = MAKEBINFIX(0);           // =  0.0
BINFIX BF_ONE    = MAKEBINFIX(1);           // = +1.0
BINFIX BF_NEGONE = MAKEBINFIX(-1);          // = -1.0
BINFIX BF_PI     = MAKEBINFIX(3, 0x243F);   // = +3.14158630370
BINFIX BF_2PI    = MAKEBINFIX(6, 0x487E);   // = +6.28317260742
BINFIX BF_3PI    = MAKEBINFIX(9, 0x6CBE);   // = +9.42477416992
BINFIX BF_PIHALF = MAKEBINFIX(1, 0x921F);   // = +1.57078552246
BINFIX BF_E      = MAKEBINFIX(2, 0xB7E1);   // = +2.7182769775
BINFIX BF_INV180 = MAKEBINFIX(0, 0x16C);    // = +0.00555419920    ( 1 / 180 )
BINFIX BF_INV360 = MAKEBINFIX(0, 0xB6);     // = +0.0027770996     ( 1 / 360 )
BINFIX BF_INVPI  = MAKEBINFIX(0, 0x517C);   // = +0.318298339840   ( 1 / PI  )
BINFIX BF_INV10  = MAKEBINFIX(0, 0x1999);   // = +0.09999084472    ( 1 / 10  )
BINFIX BF_INV100 = MAKEBINFIX(0, 0x28F);    // = +0.00999450683    ( 1 / 100 )
BINFIX BF_INV255 = MAKEBINFIX(0, 0x101);    // = +0.00392150878    ( 1 / 255 )
BINFIX BF_INF    = MAKEBINFIXX(BFSNI_INF);
BINFIX BF_NEGINF = MAKEBINFIXX(BFSNI_NEGINF);
BINFIX BF_UNDEF  = MAKEBINFIXX(BFSNI_UNDEF);

BINFIX BFMATRIX_INVSIGN[6] = {
	BF_NEGONE,
	BF_ONE,
	BF_ONE,
	BF_NEGONE,
	BF_ONE,
	BF_UNDEF
};

BINFIX BFMATRIX_SIGN[6] = {
	BF_ONE,
	BF_NEGONE,
	BF_ONE,
	BF_ONE,
	BF_NEGONE,
	BF_UNDEF
};

BINFIX BFMATRIX_SETSIGN[6][2] = {
	{BF_ONE   , BF_NEGONE},
	{BF_NEGONE, BF_ONE},
	{BF_ONE   , BF_ONE},
	{BF_ONE   , BF_NEGONE},
	{BF_NEGONE, BF_ONE},
	{BF_UNDEF , BF_UNDEF}
};

void BREAK(BINFIX X, BF_HWORD &HWORD, BF_LWORD &LWORD){
	HWORD = X.P.HWORD;
	LWORD = X.P.LWORD;
}

BINFIX SIGN(BINFIX X){
	if(IS_SPECIAL(X)) X = BFMATRIX_SIGN[X.SNI];
	else              X.DATA = SGN(X.DATA);
	UPDATE(X);
	return X;
}

S8 SGN(BINFIX X){
	if(IS_SPECIAL(X)) return 0;
	return SGN(X.DATA);
}

BINFIX SETSIGN(BINFIX X, S8 SIGN){
	if(IS_SPECIAL(X)) X = BFMATRIX_SETSIGN[X.SNI][(SIGN)? 0 : 1];
	else              X.DATA = SETSIGN(X.DATA, SIGN);
	UPDATE(X);
	return X;
}

BINFIX SETSIGN(BINFIX X, BINFIX S){
	return SETSIGN(X, SGN(S.DATA));
}

BINFIX INVSIGN(BINFIX X){
	if(IS_SPECIAL(X)) X = BFMATRIX_INVSIGN[X.SNI];
	else              X.DATA = INVSIGN(X.DATA);
	UPDATE(X);
	return X;
}

bool IS_SPECIAL(BINFIX X){
	return (X.SNI > 2);
}

bool IS_ZERO(BINFIX X){
	return (X.SNI == BFSNI_ZERO);
}

bool IS_NEG(BINFIX X){
	return ISMINOR(X, BF_ZERO);
}

void UPDATE(BINFIX &X){
	if( !IS_SPECIAL(X) ){
		BF_TYPE D;
		D = X.DATA;
		     if(D == 0) X.SNI = BFSNI_ZERO;
		else if(D > 0)  X.SNI = BFSNI_POS;
		else            X.SNI = BFSNI_NEG;
	}
}

BINFIX ADD(BINFIX X, BINFIX Y){
	BINFIX R;
	if( IS_SPECIAL(X) or IS_SPECIAL(Y))
		R.SNI = BFMATRIX_ADD[X.SNI][Y.SNI];
	else{
		R.DATA = X.DATA + Y.DATA;
		UPDATE(R);
	}
	return R;
}

BINFIX SUB(BINFIX X, BINFIX Y){
	return ADD(X, INVSIGN(Y));
}

BINFIX MUL(BINFIX X, BINFIX Y){
	BINFIX R;
	if( IS_SPECIAL(X) or IS_SPECIAL(Y))
		R.SNI = BFMATRIX_MUL[X.SNI][Y.SNI];
	else{
		BF_TYPE XX, YY;
		XX = X.DATA;
		YY = Y.DATA;
		BF_BIGTYPE BR;
		BR = BF_BIGTYPE(XX) * YY;
		if(ABS(BR) < TWOPOW16 and !(!XX or !YY)){
			goto end;
		}
		BR = BR >> 16;
		R.DATA = BR;
		end:
			UPDATE(R);
	}
	return R;
}

BINFIX DIV(BINFIX X, BINFIX Y){
	BINFIX R;
	if( IS_SPECIAL(X) or IS_SPECIAL(Y) or IS_ZERO(X) or IS_ZERO(Y))
		R.SNI = BFMATRIX_DIV[X.SNI][Y.SNI];
	
	else{
		BF_TYPE XX, YY;
		XX = X.DATA;
		YY = Y.DATA;
		BF_BIGTYPE BR;
		S64 S;
		BR  = BF_BIGTYPE(XX) << 16;
		S = SGN(BR);
		BR  = BR / YY;
		R.DATA = BR;
		end:
			UPDATE(R);
	}
	return R;
}

BINFIX ABS(BINFIX X){
	if( IS_SPECIAL(X) ) X.SNI  = BFMATRIX_ABS[X.SNI];
	else                X.DATA = ABS(X.DATA);
	UPDATE(X);
	return X;
}

BINFIX ABSDIF(BINFIX X, BINFIX Y){
	return ABS( SUB(X, Y) );
}

BINFIX MAD(BINFIX A, BINFIX B, BINFIX C){
	BINFIX R;
	R = MUL(A, B);
	R = ADD(R, C);
	return R;
}

BINFIX MOD(BINFIX X, BINFIX Y){
	BINFIX R;
	R = DIV(X, Y);
	R = TRUNC(R);
	R = SUB(X, MUL(Y, R));
	return R;
}

BINFIX MIN(BINFIX X, BINFIX Y){
	BINFIX R;
	if( IS_SPECIAL(X) or IS_SPECIAL(Y) ){
		R.SNI = BFMATRIX_MIN[X.SNI][Y.SNI];
		if(R.SNI == 0)
			R.DATA = (IS_SPECIAL(X))? Y.DATA : X.DATA;
	}
	else R.DATA = MIN(X.DATA, Y.DATA);
	UPDATE(X);
	return R;
}

BINFIX MAX(BINFIX X, BINFIX Y){
	BINFIX R;
	if( IS_SPECIAL(X) or IS_SPECIAL(Y) ){
		R.SNI = BFMATRIX_MAX[X.SNI][Y.SNI];
		if(R.SNI == 0)
			R.DATA = (IS_SPECIAL(X))? Y.DATA : X.DATA;
	}
	else R.DATA = MAX(X.DATA, Y.DATA);
	UPDATE(X);
	return R;
}

BINFIX TRUNC(BINFIX X){
	X.P.LWORD = 0x0;
	UPDATE(X);
	return X;
}

BINFIX REDON(BINFIX X){
	return ADD(X, BF_HALF);
}

BINFIX NEXT_INT(BINFIX X){
	if(IS_SPECIAL(X)) return X;
	if(IS_NEG(X)){
		X.P.LWORD = 0;
	}
	else{
		if(X.P.LWORD != 0){
			X.P.LWORD = 0;
			X = ADD(X, BF_ONE);
		}
	}
	UPDATE(X);
	return X;
}

BINFIX PREV_INT(BINFIX X){
	if(IS_SPECIAL(X)) return X;
	if(IS_NEG(X)){
		if(X.P.LWORD != 0){
			X.P.LWORD = 0;
			X = SUB(X, BF_ONE);
		}
	}
	else{
		X.P.LWORD = 0;
	}
	UPDATE(X);
	return X;
}

BINFIX LSHIFT(BINFIX X, U8 Y){
	if( IS_SPECIAL(X) ) return X;
	BINFIX R;
	R.DATA = X.DATA << Y;
	UPDATE(X);
	return R;
}

BINFIX RSHIFT(BINFIX X, U8 Y){
	if( IS_SPECIAL(X) ) return X;
	BINFIX R;
	R.DATA = X.DATA >> Y;
	UPDATE(X);
	return R;
}

BINFIX MAP(BINFIX VALUE, BINFIX IA, BINFIX FA, BINFIX IB, BINFIX FB){
	BINFIX R;
	R = MUL(SUB(VALUE, IA), SUB(FA, IA)); 
	R = MUL(R, SUB(FB, IB));
	R = ADD(R, IB);
	return R;
}

BINFIX TORAD(BINFIX DEGREES){
	BINFIX R;
	R = MUL(DEGREES, BF_PI);
	R = MUL(R, BF_INV180);
	UPDATE(R);
	return R;
}

BINFIX TODEG(BINFIX RADIANS){
	BINFIX R;
	R = MUL(RADIANS, BF_180);
	R = MUL(R, BF_INVPI);
	UPDATE(R);
	return R;
}

BF_HWORD TO_INT(BINFIX X){
	return X.P.HWORD;
}

void FORMAT_BINDEG(BINFIX &X){
	if(IS_SPECIAL(X) or IS_ZERO(X)) return;
	BF_TYPE INT;
	INT = TO_INT(X);
	INT = INT & 0xFF;
	X = MAKEBINFIX(INT);
	UPDATE(X);
}

BINFIX SIN(BINFIX X){
	if(IS_SPECIAL(X)){
		X.SNI = BFMATRIX_SIN[X.SNI];
		return X;
	}
	FORMAT_BINDEG(X);
	BINFIX R;
	U8 BINDEG;
	U8 QUADRANT=1;
	U8 iNDEX;
	bool CHANGESIGN=false;
	BINDEG = TO_INT(X);
	//.
	     if(BINDEG > 192) QUADRANT=4;
	else if(BINDEG > 128) QUADRANT=3;
	else if(BINDEG > 64 ) QUADRANT=2;
	
	switch(QUADRANT){
		case 1:
			iNDEX = BINDEG;
			break;
		case 2:
			iNDEX = 128 - BINDEG;
			break;
		case 3:
			iNDEX = BINDEG - 128;
			CHANGESIGN = true;
			break;
		case 4:
			iNDEX = 256 - BINDEG;
			CHANGESIGN = true;
			break;
	}
	R.DATA = SINTAB[iNDEX];
	UPDATE(R);
	if(CHANGESIGN) R = INVSIGN(R);
	UPDATE(R);
	return R;
}

BINFIX SINX(BINFIX X){
	BINFIX R;
	if( IS_SPECIAL(X)){
		R.SNI = BFMATRIX_SIN[X.SNI];
	}
	else{
		if(ISMAYOR(X, BF_2PI)) X = MOD(X, BF_2PI);
		R = MAKEBINFIX(0, 0x45A); // ~0.017
		if( ISMINOR(X, BF_PIHALF) ){
			R = MUL(R, ADD(X, BF_PI));
			R = MUL(R, X);
			R = MUL(R, SUB(X, BF_PI));
			R = MUL(R, SUB(X, BF_2PI));
		}
		else{
			R = INVSIGN(R);
			R = MUL(R, X);
			R = MUL(R, SUB(X, BF_PI));
			R = MUL(R, SUB(X, BF_2PI));
			R = MUL(R, SUB(X, BF_3PI));
		}
		UPDATE(R);
	}
	return R;
}

BINFIX COS(BINFIX X){
	return SIN( SUB(MAKEBINFIX(64), X) );
}

BINFIX TAN(BINFIX X){
	return DIV( SIN(X), COS(X) );
}

BINFIX ASIN(BINFIX X){
	if(IS_SPECIAL(X)){
		X.SNI = BFMATRIX_ASIN[X.SNI];
		return X;
	}
	if(ISMINOR(X, BF_NEGONE) or ISMAYOR(X, BF_ONE)){
		X.SNI = BFSNI_UNDEF;
		return X;
	}
	BINFIX R;
	S8 START;
	S8 END;
	if(ISMAYOREQUAL(X, BF_ZERO)){
		START = 0;
		END = 64;
	}
	else{
		START = -64;
		END = -1;
	}
	for(S8 i=START; i<=END;i++){
		BINFIX I;
		I = MAKEBINFIX(i);
		I = SIN(I);
		if(ISMINOR(I, X)) continue;
		R = MAKEBINFIX(i);
		return R;
	}
	R = MAKEBINFIX(64);
	return R;
}

BINFIX ACOS(BINFIX X){
	BINFIX R;
	R = ASIN(INVSIGN(X));
	R = ADD(R, MAKEBINFIX(64));
	return R;
}

BINFIX ATAN(BINFIX X){
	BINFIX R;
	switch(X.SNI){
		case BFSNI_INF:
			R = MAKEBINFIX(64);
			return R;
		case BFSNI_NEGINF:
			R = MAKEBINFIX(-64);
			return R;
		case BFSNI_UNDEF:
			return X;
	}
	S8 START;
	S8 END;
	if(ISMAYOREQUAL(X, BF_ZERO)){
		START = 0;
		END = 64;
	}
	else{
		START = -64;
		END = -1;
	}
	for(S8 i=START; i<=END;i++){
		BINFIX I;
		I = MAKEBINFIX(i);
		I = TAN(I);
		if(ISMINOR(I, X)) continue;
		R = MAKEBINFIX(i);
		return R;
	}
}

BINFIX ATAN2(BINFIX Y, BINFIX X){
	BINFIX R;
	bool S1, S2;
	S1 = ISMINOR(X, BF_ZERO);
	S2 = ISMINOR(Y, BF_ZERO);
	R = ATAN(DIV(Y, X));
	if(S1 xor S2){
		if(ISMINOR(X, BF_ZERO)) R = ADD(R, MAKEBINFIX(128));
		else                    R = SUB(R, MAKEBINFIX(128));
	}
	UPDATE(X);
	return R;
}

BINFIX EXP(BINFIX X){

}

BINFIX LN(BINFIX X){

}

BINFIX LOG(BINFIX X){

}

BINFIX LOG2(BINFIX X){

}

BINFIX INV(BINFIX X){
	return DIV(BF_ONE, X);
}

BINFIX POW(BINFIX X, BF_TYPE Y){
	BINFIX R;
	if(IS_ZERO(X)){
		if(Y>0){
			R = BF_ZERO;
			UPDATE(R);
			return R;
		}
		else if(Y==0){
			R = BF_INF;
			return R;
		}
		else{
			R = BF_INF;
			return R;
		}
	}
	if(Y > 0){
		R = X;
		for(BF_TYPE i=0; i<Y-1; i++)
			R = MUL(R, X);
		UPDATE(R);
		return R;
	}
	else if(Y == 0){
		if(IS_SPECIAL(X)) return X;
		R = BF_ONE;
		UPDATE(R);
		return R;
	}
	else{
		switch(X.SNI){
			case BFSNI_INF:    R=BF_ZERO;  UPDATE(R); return R;
			case BFSNI_NEGINF: R=BF_ZERO;  UPDATE(R); return R;
			case BFSNI_UNDEF:  R=BF_UNDEF; return R;
		}
		BINFIX RECIPROCAL;
		RECIPROCAL = DIV(BF_ONE, X);
		R = RECIPROCAL;
		for(BF_TYPE i=0; i<Y-1; i++)
			R = MUL(R, RECIPROCAL);
		UPDATE(R);
		return R;
	}
}

BINFIX POW(BINFIX X, BINFIX Y){
	BINFIX R;
	R = MUL(LN(X), Y);
	R = EXP(R);
	return R;
}

BINFIX SQ(BINFIX X){
	return MUL(X, X);
}

BINFIX SQRT(BINFIX X){
	X.SNI = BFMATRIX_SQRT[X.SNI];
	if( !IS_SPECIAL(X) )
		X.DATA = SQRT(S64(X.DATA) << 16);
	UPDATE(X);
	return X;
}

BINFIX INVSQRT(BINFIX X){
	X = DIV(SQRT(X), X);
	UPDATE(X);
	return X;
}

BINFIX CB(BINFIX X){
	return MUL(SQ(X), X);
}

BINFIX CBRT(BINFIX X){
	X.SNI = BFMATRIX_CBRT[X.SNI];
	if( !IS_SPECIAL(X) )
		//X.DATA = CBRT(S64(X.DATA) << 24);
		X.DATA = CBRT(S64(X.DATA) << 32);
	UPDATE(X);
	return X;
}

BINFIX HYPOT(BINFIX X, BINFIX Y){
	X = ADD(SQ(X), SQ(Y));
	X = SQRT(X);
	UPDATE(X);
	return X;
}

BINFIX HYPOTX(BINFIX X, BINFIX Y){
	X = ADD(SQ(X), SQ(Y));
	UPDATE(X);
	return X;
}

bool ISEQUAL(BINFIX X, BINFIX Y){
	if( IS_SPECIAL(X) or IS_SPECIAL(Y))
		return (X.SNI == Y.SNI);
	return (X.DATA == Y.DATA);
}

bool ISNOEQUAL(BINFIX X, BINFIX Y){
	return !ISEQUAL(X,Y);
}

bool ISMAYOR(BINFIX X, BINFIX Y){
	if( IS_SPECIAL(X) or IS_SPECIAL(Y)){
		BINFIX MAXX;
		MAXX = MAX(X, Y);
		return ISEQUAL(X, MAXX);
	}
	return (X.DATA > Y.DATA);
}

bool ISMINOR(BINFIX X, BINFIX Y){
	if( IS_SPECIAL(X) or IS_SPECIAL(Y)){
		BINFIX MINN;
		MINN = MIN(X, Y);
		return ISEQUAL(X, MINN);
	}
	return (X.DATA < Y.DATA);
}

bool ISMAYOREQUAL(BINFIX X, BINFIX Y){
	if( IS_SPECIAL(X) or IS_SPECIAL(Y))
		return (ISMAYOR(X, Y) or ISEQUAL(X, Y));
	return (X.DATA >= Y.DATA);
}

bool ISMINOREQUAL(BINFIX X, BINFIX Y){
	if( IS_SPECIAL(X) or IS_SPECIAL(Y))
		return (ISMINOR(X, Y) or ISEQUAL(X, Y));
	return (X.DATA <= Y.DATA);
}


void PRINT(BINFIX X){

	if( IS_ZERO(X) ){ std::cout<< "0.0"; return; }
	if( IS_SPECIAL(X) ){
		switch(X.SNI){
			case BFSNI_INF:    std::cout<< "+INF" ; return;
			case BFSNI_NEGINF: std::cout<< "-INF" ; return;
			case BFSNI_UNDEF:  std::cout<< "UNDEF"; return;
		}
	}
	
	BINFIX SGN;
	SGN = SIGN(X);
	BF_HWORD HWORD;
	BF_LWORD LWORD;
	BREAK(X, HWORD, LWORD);
	if(SGN.DATA == BF_NEGONE.DATA) std::cout<<"-";
	std::cout<< HWORD;
	if(LWORD != 0){
		std::cout<< " + "    ;
		std::cout<< LWORD    ;
		std::cout<< "/65536" ;
	}
}


/*void PRINT(BINFIX X){
	if(X.DATA < 0) {std::cout<<"-"; B = ABS(X);}
	QUICK_BREAK(X)
	std::cout<< HWORD ;
	std::cout<< ","   ;
	U8 SB = COUNT_SIGNIFICATIVE_BITS(LWORD);
	if(LWORD == 0){ std::cout<<"0"; return; }
	U64 R;
	R = ( LWORD * POW10(16)) >> 16;
	std::cout<< R;
	//std::cout<< "\n" << ABS(DECIMAL_LENGHT(R) - 12);
	//R = R / (16-DECIMAL_LENGHT(R));
	//std::cout<< R;
}*/
