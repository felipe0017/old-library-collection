// FLOATING POINT LIBRARY (32-bit)
// 10-03-2022

/*
bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb
EEEEEEEE SINNNNNN NNNNNNNN NNNNNNNN
E = SIGNED EXPONENT
S = SIGN BIT
I = FLAG SNI
N = UNSIGNED COEFICIENT
*/


#define F32_TYPE  U32
#define F32_BTYPE U64

#define F32_S U8
#define F32_N U32
#define F32_E S8
#define F32_I U8

// F32 BIG N & E
#define F32B_N U64
#define F32B_E S16

/*
ZERO   0
NCV    1
POS    2
NEG    3
*/

F32_I F32MATRIX_ADD[4][4] = {
	{0, 1, 2, 3},
	{1, 1, 1, 1},
	{2, 1, 2, 2},
	{3, 1, 2, 2}
};

F32_I F32MATRIX_MUL[4][4] = {
	{0, 1, 0, 0},
	{1, 1, 1, 1},
	{0, 1, 2, 2},
	{0, 1, 2, 2}
};

BOOL F32MATRIX_MAYOR[4][4] = {
	{0, 0, 0, 1},
	{0, 0, 0, 0},
	{1, 0, 1, 1},
	{0, 0, 0, 1}
};

STRUCT F32{
	PRIVATE:
		F32_TYPE DATA;
		
		FRIEND VOID   SIGN(F32 &F, F32_S S);
		FRIEND F32_S  SGNX(F32 F);
		FRIEND F32_I  SNI (F32 F);
		FRIEND VOID   SETN(F32 &F, F32_N N);
		FRIEND VOID   SETE(F32 &F, F32_E E);
		FRIEND VOID   SETD(F32 &F, F32_TYPE DATA);
		FRIEND F32_N  GETN(F32 F);
		FRIEND F32_E  GETE(F32 F);
	
	PUBLIC:
		
		F32(){
			THIS->DATA = 0XFFFFFFFE;
		}
		
		F32(ATYPE DATA); // IF DATA==0, USE 0LL
		F32(CONST CHAR* TEXT);
		
		FRIEND F32 MAKEF32(F32_S S, F32_N N, F32_E E);
		
		FRIEND VOID   BREAK(F32 F, F32_S &S, F32_N &N, F32_E &E);
		FRIEND F32    SGN(F32 F);
		FRIEND VOID   SIGN(F32 &F, F32 SGN);
		FRIEND VOID   INVSIGN(F32 &F);
		FRIEND BOOL   SPECIAL(F32 F);
		
		F32&   OPERATOR= (ATYPE DATA);
		F32&   OPERATOR= (CONST CHAR* TEXT); // not implemented yet
		F32    OPERATOR+ ();
		F32    OPERATOR+ (F32 F);
		F32&   OPERATOR+=(F32 F);
	    F32&   OPERATOR++();    // PREFIX
	    F32    OPERATOR++(int); // SUFIX
	    F32    OPERATOR- ();
		F32    OPERATOR- (F32 F);
		F32&   OPERATOR-=(F32 F);
	    F32&   OPERATOR--();
	    F32    OPERATOR--(int);
	    F32    OPERATOR>>(F32_E D);
		F32    OPERATOR<<(F32_E D);
		F32&   OPERATOR>>=(F32_E D);
		F32&   OPERATOR<<=(F32_E D);
	    F32    OPERATOR* (F32 F);
	    F32&   OPERATOR*=(F32 F);
		F32    OPERATOR/ (F32 F);
		F32&   OPERATOR/=(F32 F);
		F32    OPERATOR% (F32 F);
		F32    OPERATOR%=(F32 F);
		/*F32  OPERATOR^ (F32 F);*/
		FRIEND F32 MYDIV(F32 X, F32 Y);
		
		BOOL   OPERATOR> (F32 F);
		BOOL   OPERATOR< (F32 F);
		BOOL   OPERATOR==(F32 F);
		BOOL   OPERATOR!=(F32 F);
		BOOL   OPERATOR>=(F32 F);
		BOOL   OPERATOR<=(F32 F);
		
		// BASIC OPERATIONS
		FRIEND F32 ABS(F32 X);
		FRIEND F32 ABSDIF(F32 X, F32 Y); // POSITIVE DIFERENCE
		FRIEND F32 MAD(F32 A, F32 B, F32 C);  //  A * B + C
		FRIEND F32 MAD2(F32 A, F32 B, F32 C); //  A * B - C
		FRIEND F32 MAD3(F32 A, F32 B, F32 C); // -A * B + C
		FRIEND F32 MAD4(F32 A, F32 B, F32 C); // -A * B - C
		FRIEND F32 MIN(F32 X, F32 Y);
		FRIEND F32 MAX(F32 X, F32 Y);
		FRIEND F32 TRUNC(F32 X);
		FRIEND F32 REDON(F32 X);
		FRIEND F32 NEXT_INT(F32 X);
		FRIEND F32 PREV_INT(F32 X);
		FRIEND F32 MAP(F32 VALUE, F32 IA, F32 FA, F32 IB, F32 FB); // SEE MAP() IN MATH.H
		FRIEND F32 MAP(F32 VALUE, F32 FA, F32 FB); // WHEN IA & IB == 0, THIS VERSION OF MAP IS MORE FAST
		
		// TYPE CONVERSION
		FRIEND ATYPE INT(F32 F);
		//FRIEND BINEXP_BIG MAKEBINEXP_BIG(F32 F);
		
		FRIEND STD::OSTREAM& OPERATOR<<(STD::OSTREAM& OS, F32 F);
};

F32 MAKEF32(F32_S S, F32_N N, F32_E E){
	S = S & 0X1;
	N = N & 0X7FFFFF;
	
	F32 F;
	F.DATA = (N | (S << 23)) | (E << 24);
	RETURN F;
}

VOID BREAK(F32 F, F32_S &S, F32_N &N, F32_E &E){
	S = (F.DATA >> 23      ) & 0X1;
	N = (F.DATA &  0X7FFFFF) | 0X400000;
	E = (F.DATA >> 24      );
}

F32 F32_MAX     = MAKEF32(0,~0,  127);  //  = +3.4028235e+38
F32 F32_MIN     = MAKEF32(1,~0,  127);  //  = -3.4028235e+38
F32 F32_MIN_ABS = MAKEF32(0, 0, -128);  //  = +2.9387359e-39

F32 F32_HALF    = MAKEF32(0, 0,   -1);  //  = +0.5
F32 F32_ONE     = MAKEF32(0, 0,    0);  //  = +1.0
F32 F32_NEGONE  = MAKEF32(1, 0,    0);  //  = -1.0

F32 F32_INV255  = MAKEF32(0, 0X8080, -8); // = +0.00392156839 ( 1 / 255 = +0.00392156862)

F32 F32_ZERO    = MAKEF32(1, 0X7FFFFF - 1, 0XFF);
F32 F32_NCV     = MAKEF32(1, 0X7FFFFF    , 0XFF); // NCV = NOT COMPUTABLE VALUE

F32 F32MATRIX_SGN[2] = {
	F32_ONE,
	F32_NCV
};

F32_I F32MATRIX_SGNX[2] = {
	0,
	0
};

F32 F32MATRIX_SETSIGN[2][2] = {
	{F32_ONE, F32_ONE},
	{F32_NCV, F32_NCV}
};

ATYPE F32MATRIX_INT[4] = {
	0,
	-1
};

F32 SGN(F32 F){
	IF( SPECIAL(F) ){
		RETURN F32MATRIX_SGN[SNI(F)];
	}
	RETURN SGNX(F)? F32_NEGONE : F32_ONE;
}

F32_S SGNX(F32 F){
	IF( SPECIAL(F) ){
		RETURN F32MATRIX_SGNX[SNI(F)];
	}
	RETURN ((F.DATA >> 23) & 0X1);
}

VOID SIGN(F32 &F, F32_S S){
	IF(SPECIAL(F)){
		F = F32MATRIX_SETSIGN[SNI(F)][S];
	}
	ELSE{
		F.DATA = (F.DATA & 0XFF7FFFFF) | ((S & 0X1) << 23);
	}
}

F32_I SNI(F32 F){
	IF( SPECIAL(F) ){
		RETURN (F.DATA & 0X1);
	}
	F32_S S;
	S = SGNX(F);
	RETURN MAX_F32SNI + S + 1;
}

VOID SIGN(F32 &F, F32 SGN){
	F32_S S;
	S = SGNX(SGN);
	SIGN(F, S);
}

VOID SETN(F32 &F, F32_N N){
	F.DATA = (F.DATA & 0XFFC00000) | (N & 0X7FFFFF);
}

VOID SETE(F32 &F, F32_E E){
	F.DATA = (F.DATA & 0XFFFFFF) | (E << 24);
}

VOID SETD(F32 &F, F32_TYPE DATA){
	F.DATA = DATA;
}

F32_N GETN(F32 F){
	RETURN ((F.DATA & 0X7FFFFF) | 0X400000);
}

F32_E GETE(F32 F){
	RETURN (F.DATA >> 24);
}

VOID INVSIGN(F32 &F){
	SIGN(F, SGNX(F)? 0 : 1);
}

BOOL SPECIAL(F32 F){
	RETURN (F.DATA >= 0X7FFFFF);
}
/*
MATRIX FOR ANALIZE SIGN IN MAYOR COMPARATION
0 -> CONTINUE IN SUBROUTINE
1 -> RETURN TRUE
2 -> RETURN FALSE
*/
U8 F32MATRIX_SGNM[2][2] = {
	{0, 1},
	{2, 0}
};

BOOL F32::OPERATOR>(F32 F){
	F32 X,Y;
	X = *THIS;
	Y = F;
	
	IF( SPECIAL(X) OR SPECIAL(Y) ){
		RETURN F32MATRIX_MAYOR[SNI(X)][SNI(Y)];
	}
	
	F32_S XS,YS; F32_N XN,YN; F32_E XE,YE;
	BREAK(X, XS, XN, XE);
	BREAK(Y, YS, YN, YE);
	
	SWITCH(F32MATRIX_SGNM[XS][YS]){
		CASE 1:
			RETURN BTRUE;
			break;
		CASE 2:
			RETURN BFALSE;
			break;
	}
	
	IF(XS AND YS){
		IF(XE < YE){
			RETURN BTRUE;
		}
		ELSE IF(XE == YE){
			IF(XN < YN) RETURN BTRUE;
		}
		RETURN BFALSE;
	}
	
	IF(XE > YE){
		RETURN BTRUE;
	}
	ELSE IF(XE == YE){
		IF(XN > YN) RETURN BTRUE;
	}
	
	RETURN BFALSE;
}

// IF X IS NOT GREATER OR EQUAL TO Y, IT IS LESS
BOOL F32::OPERATOR<(F32 F){
	F32 X,Y;
	X = *THIS;
	Y = F;
	
	RETURN ( !(X > Y) AND (X != Y) );
}

BOOL F32::OPERATOR==(F32 F){
	F32 X,Y;
	X = *THIS;
	Y = F;
	
	F32_S XS,YS; F32_N XN,YN; F32_E XE,YE;
	BREAK(X, XS, XN, XE);
	BREAK(Y, YS, YN, YE);
	
	IF( SPECIAL(X) OR SPECIAL(Y) ){
		RETURN (SNI(X) == SNI(Y));
	}

	RETURN (XS == YS AND XN == YN AND XE == YE);
}

BOOL F32::OPERATOR!=(F32 F){
	F32 X,Y;
	X = *THIS;
	Y = F;
	
	RETURN !(X == Y);
}

BOOL F32::OPERATOR>=(F32 F){
	F32 X,Y;
	X = *THIS;
	Y = F;
	
	RETURN (X > Y OR X == Y);
}

BOOL F32::OPERATOR<=(F32 F){
	F32 X,Y;
	X = *THIS;
	Y = F;
	
	RETURN (X < Y OR X == Y);
}

F32& F32::OPERATOR=(ATYPE DATA){
	F32 R;
	IF(DATA == 0) RETURN F32_ZERO;
	
	F32_S S; F32_N N; F32_E E;
	
	S = (DATA < 0);
	IF(S) DATA = INVSIGN(DATA);
	
	E = LOG2(DATA);
	N = ((DATA << 23) >> E) - TWOPOW22;
	
	R = MAKEF32(S, F32_N(N), F32_E(E));
	*THIS = R;
	RETURN *THIS;
}

F32::F32(ATYPE DATA){
	F32 F;
	F = DATA;
	*THIS = F;
}

F32& F32::OPERATOR=(CONST CHAR* TEXT){
	*THIS = F32_NCV;
	RETURN *THIS;
}

F32::F32(CONST CHAR* TEXT){
	F32 F;
	F = TEXT;
	*THIS = F;
}

F32 F32::OPERATOR+(){
	RETURN *THIS;
}

F32 F32::OPERATOR+(F32 F){
	F32 R,X,Y;
	X = *THIS;
	Y = F;
	
	IF( SPECIAL(X) OR SPECIAL(Y) ){
		F32_I I = F32MATRIX_ADD[SNI(X)][SNI(Y)];
		IF(I > MAX_F32SNI){
			R = (X == F32_ZERO)? Y : X;
		}
		ELSE{
			SETI(R, I);
		}
		RETURN R;
	}
	
	F32B_N NN,BXN,BYN; F32B_E EE;
	F32_S S,XS,YS; F32_N XN,YN; F32_E XE,YE;
	
	BREAK(X, XS, XN, XE);
	BREAK(Y, YS, YN, YE);

	U8 D;
	D = MAX(XE,YE) - MIN(XE,YE);
	IF(D > 64-23){
		R = (XE > YE)? X : Y;
		RETURN R;
	}
	IF(XE > YE){
		BXN = F32B_N(XN) << D;
		BYN = YN;
		EE  = F32B_E(XE) -  D;
	}
	ELSE{
		BXN = XN;
		BYN = F32B_N(YN) << D;
		EE  = F32B_E(YE) -  D;
	}
	
	BOOL OP;
	OP = XS XOR YS;
	
	IF(OP){
		IF(BXN > BYN){
			S = XS;
			NN = BXN - BYN;
		}
		ELSE{
			S = YS;
			NN = BYN - BXN;
		}
	}
	ELSE{
		S = XS;
		NN = BXN + BYN;
	}
	
	// NORMALIZE N
	U8 L;
	U8 ND;
	L = BIT_LENGTH(NN);
	
	IF(L < 23){
		ND = 23 - L;
		NN = NN << ND;
		EE = EE -  ND;
	}
	ELSE{
		ND = L - 23;
		NN = NN >> ND;
		EE = EE +  ND;
	}

	IF(NN == 0){
		R = F32_ZERO;
		RETURN R;
	}
	
	R = MAKEF32(S, F32_N(NN), F32_E(EE));
	RETURN R;
}

F32& F32::OPERATOR+=(F32 F){
	*THIS = *THIS + F;
	RETURN *THIS;
}

F32& F32::OPERATOR++(){
	*THIS = *THIS + F32_ONE;
	RETURN *THIS;
}

F32 F32::OPERATOR++(int){
	F32 OLD = *THIS;
	*THIS = *THIS + F32_ONE;
	RETURN OLD;
}

F32 F32::OPERATOR-(){
	F32 R;
	R = *THIS;
	INVSIGN(R);
	RETURN R;
}

F32 F32::OPERATOR-(F32 F){
	F32 R,X,Y;
	X = *THIS;
	Y = F;
	
	R = X + (-Y);
	RETURN R;
}

F32& F32::OPERATOR-=(F32 F){
	*THIS = *THIS - F;
	RETURN *THIS;
}

F32& F32::OPERATOR--(){
	*THIS = *THIS - F32_ONE;
	RETURN *THIS;
}

F32 F32::OPERATOR--(int){
	F32 OLD = *THIS;
	*THIS = *THIS - F32_ONE;
	RETURN OLD;
}

F32 F32::OPERATOR>>(F32_E D){
	F32 R;
	R = *THIS;
	
	IF( SPECIAL(R) ) RETURN R;
	
	F32B_E EE;
	F32_S S; F32_N N; F32_E E;
	
	BREAK(R, S, N, E);

	EE = F32B_E(E) - D;
	IF( !IN_RANGE(EE, S8_MIN, S8_MAX) ){
		R = F32_NCV;
		RETURN R;
	}
	
	R = MAKEF32(S, N, F32_E(EE));
	RETURN R;
}

F32 F32::OPERATOR<<(F32_E D){
	F32 R;
	R = *THIS;
	
	R = R >> (-D);
	
	RETURN R;
}

F32& F32::OPERATOR>>=(F32_E D){
	*THIS = *THIS >> D;
	RETURN *THIS;
}

F32& F32::OPERATOR<<=(F32_E D){
	*THIS = *THIS << D;
	RETURN *THIS;
}

F32 F32::OPERATOR*(F32 F){
	F32 R,X,Y;
	X = *THIS;
	Y = F;
	
	IF( SPECIAL(X) OR SPECIAL(Y) ){
		F32_I I = F32MATRIX_MUL[SNI(X)][SNI(Y)];
		SETI(R, I);
		RETURN R;
	}
	
	F32B_N NN; F32B_E EE;
	F32_S S,XS,YS; F32_N XN,YN; F32_E XE,YE;
	
	BREAK(X, XS, XN, XE);
	BREAK(Y, YS, YN, YE);
	
	S  = XS XOR YS;
	NN = (F32B_N(XN) * YN);
	BOOL BC; // BIG COEFFICIENT
	BC = ((NN >> 22) >= TWOPOW23);
	IF(BC){
		NN = (NN / 2) >> 22;
		EE = XE + YE + 1;
	}
	ELSE{
		NN = NN >> 22;
		EE = XE + YE;
	}
	
	IF( !IN_RANGE(EE, S8_MIN, S8_MAX) ){
		R = F32_NCV;
		RETURN R;
	}
	
	R = MAKEF32(S, F32_N(NN), F32_E(EE));
	RETURN R;
}

F32& F32::OPERATOR*=(F32 F){
	*THIS = *THIS * F;
	RETURN *THIS;
}

F32 F32::OPERATOR/(F32 F){
	F32 R,X,Y;
	X = *THIS;
	Y = F;
	
	IF( SPECIAL(X) OR SPECIAL(Y) ){
		RETURN F32_NCV;
	}
	
	F32B_N NN; F32B_E EE;
	F32_S S,XS,YS; F32_N XN,YN; F32_E XE,YE;
	
	BREAK(X, XS, XN, XE);
	BREAK(Y, YS, YN, YE);
	
	S  = XS XOR YS;
	BOOL SN; // SMALL NUMERATOR
	SN = (XN < YN);
	IF(SN){
		NN = ((F32B_N(XN) * 2) << 22) / YN;
		EE = XE - (YE + 1);
	}
	ELSE{
		NN = (F32B_N(XN) << 22) / YN;
		EE = XE - YE;
	}
	
	IF( !IN_RANGE(EE, S8_MIN, S8_MAX) ){
		R = F32_NCV;
		RETURN R;
	}
	
	R = MAKEF32(S, F32_N(NN), F32_E(EE));
	RETURN R;
}

F32& F32::OPERATOR/=(F32 F){
	*THIS = *THIS / F;
	RETURN *THIS;
}

F32 F32::OPERATOR%(F32 F){
	F32 R,X,Y;
	X = *THIS;
	Y = F;
	R = X - PREV_INT(X/Y) * Y;
	RETURN R;
}

F32 F32::OPERATOR%=(F32 F){
	*THIS = *THIS % F;
	RETURN *THIS;
}

F32 MYDIV(F32 X, F32 Y){
	F32 R;
	
	IF( SPECIAL(X) OR SPECIAL(Y) ){
		RETURN F32_NCV;
	}
	
	F32B_E EE;
	F32_S XS,YS; F32_N XN,YN; F32_E XE,YE;
	
	BREAK(X, XS, XN, XE);
	BREAK(Y, YS, YN, YE);
	
	F32 XX,YY;
	XX = MAKEF32(XS, XN, 0);
	YY = MAKEF32(YS, YN, 0);
	
	CONST U8 IT = 16; // SUBROUTINE ITERATIONS
	CONST U8 FN = 16; // FACTORS NUMBER
	F32 FA[FN]; // FACTORS
	F32 RE[FN]; // RECIPROCAL
	
	FOR(U8 I = 0; I < FN; I++){
		FA[I] = F32_ONE + (F32_ONE >> (I + 1));
		RE[I] = F32_ONE / FA[I];
	}
	
	F32 E;  // DENOMINATOR ERROR
	FOR(U8 I=0; I < IT; I++){
		F32 *PF; // POINTER FACTOR VAR
		BOOL F_TYPE; // 0 -> FA , 1 -> RE
		E = YY - F32_ONE;
		IF(E == F32_ZERO) break;
		IF(E > F32_ZERO){
			PF = &RE[0];
			F_TYPE = 1;
		}
		ELSE{
			PF = &FA[0];
			F_TYPE = 0;
		}
		F32 TE; // TEST ERROR
		FOR(U8 J = 0; J < FN; J++){
			F32 FACTOR;
			F32 TD; // TEST DENOMINATOR
			FACTOR = *(PF + J);
			TD = YY * FACTOR;
			TE = ABSDIF(TD, F32_ONE);
			IF( TE < ABS(E) ){
				XX = XX * FACTOR;
				YY = TD;
				break;
			}
			ELSE{
				IF(J == FN - 1) GOTO DONE;
			}
		}
	}
	DONE:
	EE = F32B_E( GETE(XX) ) + (XE - YE);
	IF( !IN_RANGE(EE, S8_MIN, S8_MAX) ){
		R = F32_NCV;
		RETURN R;
	}
	
	R = XX;
	SETE(R, F32_E(EE));
	
	RETURN R;
}

F32 ABS(F32 X){
	F32 R;
	R = X;
	SIGN(R, 0);
	RETURN R;
}

F32 ABSDIF(F32 X, F32 Y){
	RETURN ABS(X - Y);
}

F32 MAD(F32 A, F32 B, F32 C){
	RETURN A * B + C;
}

F32 MAD2(F32 A, F32 B, F32 C){
	RETURN A * B - C;
}

F32 MAD3(F32 A, F32 B, F32 C){
	RETURN -A * B + C;
}

F32 MAD4(F32 A, F32 B, F32 C){
	RETURN -A * B - C;
}

F32 MIN(F32 X, F32 Y){
	RETURN (X < Y)? X : Y;
}

F32 MAX(F32 X, F32 Y){
	RETURN (X > Y)? X : Y;
}

F32 TRUNC(F32 X){
	IF( SPECIAL(X) ) RETURN X;
	
	F32 R;
	
	F32_S S; F32_N N; F32_E E;
	BREAK(X, S, N, E);
	IF(E < 0){
		R = F32_ZERO;
		RETURN R;
	}
	IF(N == TWOPOW22){
		RETURN X;
	}
	
	ATYPE  A;
	F32 AA;
	F32 D;
	
	A  = TWOPOW22 >> E;
	AA = F32_ONE  >> E;
	
	N = N - TWOPOW22;
	D = N / A;
	
	R = F32_ONE + AA * D;
	SIGN(R, S);
	SETE(R, E);
	RETURN R;
}

F32 REDON(F32 X){
	RETURN TRUNC(X + F32_HALF);
}

F32 NEXT_INT(F32 X){
	IF(X > F32_ZERO){
		IF(X - TRUNC(X) > F32_ZERO){
			RETURN TRUNC(X) + F32_ONE;
		}
		RETURN X;
	}
	ELSE{
		IF(X - TRUNC(X) < F32_ZERO){
			RETURN TRUNC(X);
		}
		RETURN X;
	}
}

F32 PREV_INT(F32 X){
	IF(X > F32_ZERO){
		IF(X - TRUNC(X) > F32_ZERO){
			RETURN TRUNC(X);
		}
		RETURN X;
	}
	ELSE{
		IF(X - TRUNC(X) < F32_ZERO){
			RETURN TRUNC(X) - F32_ONE;
		}
		RETURN X;
	}
}

F32 MAP(F32 VALUE, F32 IA, F32 FA, F32 IB, F32 FB){
	RETURN ((VALUE - IA) / (FA - IA)) * (FB - IB) + IB;
}

F32 MAP(F32 VALUE, F32 FA, F32 FB){
	RETURN (VALUE * FB) / FA;
}

ATYPE INT(F32 F){
	IF( SPECIAL(F) ){
		RETURN F32MATRIX_INT[SNI(F)];
	}
	
	ATYPE R;
	F32_S S; F32_N N; F32_E E;
	BREAK(F, S, N, E);
	
	IF(E < 0) RETURN 0;
	
	R = 1 << E;
	IF(S) R = INVSIGN(R);
	R = R + ((ATYPE(N-TWOPOW22) * R) >> 22);
	RETURN R;
}

STD::OSTREAM& OPERATOR<<(STD::OSTREAM& OS, F32 F){
	IF( SPECIAL(F) ){
		SWITCH( SNI(F) ){
			CASE F32SNI_ZERO:
				OS << "0";
				break;
			CASE F32SNI_NCV:
				OS << "NCV";
				break;
		}
		RETURN OS;
	}
	
	F32_S S; F32_N N; F32_E E;
	BREAK(F, S, N, E);
	OS << ((S)? "-" : "+");
	OS << "( 1 + " << N - TWOPOW22  << " / " << TWOPOW22 << ")";
	OS << " x 2^" << int(E);
	OS << " = " << pow(-1, S) * (double(N) / double(TWOPOW22)) * pow(2, E); // ONLY FOR DEBUG
	RETURN OS;
}
