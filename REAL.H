// REAL NUMBERS ARITHMETIC LIBRARY (32-bit)
// 02-06-2021

typedef S32 REAL;

#define REAL_INT S32
#define REAL_EXA U16


#define REAL_MAX_VALUE S32_MAX
#define REAL_MIN_VALUE S32_MIN
#define REAL_MAX_ABS   S32_MAX
#define REAL_MIN_ABS   1

#define MAKEREALX(N) N*10000

// SOME CONSTANTS
const REAL REAL_ZERO  =     0;
const REAL REAL_ONE   = 10000;
const REAL REAL_EULER = 27183;
const REAL REAL_PI    = 31416;


REAL MAKEREAL(S32 N){
	S64 R = S64(N)* 10000;
	if( R > S32_MAX ){
		return S32_MAX;
	}
	if( R < S32_MIN ){
		return S32_MIN;
	}
	return R;
}

// GET INTEGER PART
REAL_INT TRUNC(REAL N){
	return N / 10000;
}

// GET EXACT PART
REAL_EXA GET_EXA(REAL N){
	REAL_INT INT;
	REAL_EXA EXA;
	INT = TRUNC(N);
	INT = ABS(INT) * 10000;
	EXA = ABS(N) - INT;
	return EXA;
}

// INTEGER PART APROXIMATION
REAL REDON(REAL N){
	S64 R;
	R = S64(N) + ((N<0)? -5000 : 5000);
	if( R > S32_MAX ) return TRUNC(S32_MAX);
	if( R < S32_MIN ) return TRUNC(S32_MIN);
	return TRUNC(R);
}

void PRINT(REAL N){
	if(N < 0 and TRUNC(N) == 0) std::cout<<"-";
	std::cout<<TRUNC(N)<<",";
	REAL_EXA EXA;
	EXA = GET_EXA(N);
	     if(EXA < 10  ) std::cout<<"000";
	else if(EXA < 100 ) std::cout<<"00";
	else if(EXA < 1000) std::cout<<"0";
	std::cout<<EXA;
}

REAL REAL_ADD(REAL A, REAL B){
	REAL R;
	R = A + B;
	return R;
}

REAL REAL_SUB(REAL A, REAL B){
	S64 R;
	R = A - B;
	return R;
}

// POSITIVE DIFERENCE
REAL REAL_ABSDIF(REAL A, REAL B){
	return ABS( REAL_SUB(A, B) );
}

REAL REAL_MUL(REAL A, REAL B){
	S64 R;
	R = S64(A) * B;
	R = R / 10000;
	return R;
}

REAL REAL_DIV(REAL A, REAL B){
	if(A == 0 and B == 0){
		RETURN 0;
	}
	if(B == 0){
		RETURN 0;
	}
	S64 R;
	S64 S;
	R = S64(A) * 10000;
	S = SGN(R);
	R = R/B;
	if(R==0 and A != 0){
		RETURN 0;
	}
	return R;
}

REAL REAL_MOD(REAL A, REAL B){
	return A % B;
}

REAL TORADIANS(REAL DEGREES){
	REAL R;
	R = REAL_MUL(DEGREES, REAL_PI);
	R = REAL_DIV(R, MAKEREALX(180));
	return R;
}

REAL TODEGREES(REAL RADIANS){
	REAL R;
	R = REAL_MUL(RADIANS, MAKEREALX(180));
	R = REAL_DIV(R, REAL_PI);
	return R;
}

// 1 / X
REAL REAL_INV(REAL R){
	return REAL_DIV(REAL_ONE, R);
}

REAL REAL_SQRT(REAL A){
	return SQRT(S64(A)*10000);
}

// X^2
REAL REAL_SQ(REAL A){
	return REAL_MUL(A, A);
}

// 1 / SQRT(X)
REAL REAL_INVSQRT(REAL A){
	return REAL_DIV(REAL_ONE, REAL_SQRT(A));
}

REAL REAL_CBRT(REAL A){
	return CBRT(S64(A)*10000000);
}

// X^3
REAL REAL_CB(REAL A){
	return REAL_MUL( REAL_SQ(A), A);
}

// HYPOTENUSE
// R1, R2 ARE SIDES OF A RECTANGLE TRIANGLE
REAL REAL_HYPOT(REAL R1, REAL R2){
	return REAL_SQRT( REAL_ADD(REAL_SQ(R1), REAL_SQ(R2)) );
}
