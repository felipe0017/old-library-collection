// DRAWING PRIMITIVES

#define PRIMI_UP_RIGHT    0
#define PRIMI_UP_LEFT     1
#define PRIMI_DOWN_LEFT   2
#define PRIMI_DOWN_RIGHT  3

namespace PRIMI{
	
	// ASSOCIATED FUNCTION FOR DRAWING PIXELS.
	void (*F)(VEC2 POS, COLOR CLR);
	COLOR C;
	
	// SET DRAW PIXEL FUNCTION
	// [!] YOU MUST CALL THIS FUNCTION BEFORE DRAWING ANYTHING
	void SET( void (*F1)(VEC2 POS, COLOR CLR) ){ F = F1; }
	
	void DRAWPIXEL(VEC2 POS, COLOR CLR){
		if( F ){
			F(POS, CLR);
		}
	}
	
	void SET_COLOR(COLOR CLR){
		C = CLR;
	}
	
	void HLINE(VEC2 I, U16 X_DISPLACEMENT);
	void VLINE(VEC2 I, U16 Y_DISPLACEMENT);
	
	void VRECT(RECT R);
	void FRECT(RECT R);
	
	void DIAGONAL(VEC2 I, U16 DISPLACEMENT, U8 DIR);
	void LINE(VEC2 I, VEC2 F);
	
	void VCIRCLE(CIRCLE C);
	void FCIRCLE(CIRCLE C);

	VOID VTRIANGLE(TRIANGLE T);
	VOID FTRIANGLE(TRIANGLE T);
}


void PRIMI::HLINE(VEC2 I, U16 X_DISPLACEMENT){
	
	for(U16 i=0; i<=X_DISPLACEMENT; i++)
		DRAWPIXEL(VEC2(I.X+i,I.Y), PRIMI::C);
}

void PRIMI::VLINE(VEC2 I, U16 Y_DISPLACEMENT){
	
	for(U16 i=0; i<=Y_DISPLACEMENT; i++)
		DRAWPIXEL(VEC2(I.X,I.Y+i), PRIMI::C);
}

void PRIMI::VRECT(RECT R){
	
	VEC2 I,F; // FOR EACH SIDE
	VEC2 SIZE; SIZE = GET_SIZE(R);
	
	I = GET_POS_I(R);
	F = GET_VERTEX(R, RECT_UP_RIGHT);
	HLINE(I, SIZE.X - 1);
	
	F = GET_VERTEX(R, RECT_DOWN_LEFT);
	VLINE(I, SIZE.Y - 1);
	
	F = GET_POS_F(R);
	I = GET_VERTEX(R, RECT_DOWN_LEFT);
	HLINE(I, SIZE.X - 1);
	
	I = GET_VERTEX(R, RECT_UP_RIGHT);
	VLINE(I, SIZE.Y - 1);
}

void PRIMI::FRECT(RECT R){
	VEC2 I,F,SIZE; GET_3POS(R,I,F,SIZE);
	
	for(U16 i=I.Y; i<=F.Y; i++)
		HLINE(VEC2(I.X, i), SIZE.X - 1);
}

void PRIMI::DIAGONAL(VEC2 I, U16 DISPLACEMENT, U8 DIR){
	S32 DX=1,DY=1;
	U16 DC=0; // DISPLACEMENT COUNTER
	switch(DIR){
		case PRIMI_UP_RIGHT:   DY=-1; break;
		case PRIMI_UP_LEFT:    DX=-1; DY=-1; break;
		case PRIMI_DOWN_LEFT:  DX=-1; break;
		case PRIMI_DOWN_RIGHT: break;
	}
	
	S32 X=0,Y=0;
	while( DC < DISPLACEMENT ){
		X += DX;
		Y += DY;
		DRAWPIXEL(VEC2(I.X + X, I.Y + Y), PRIMI::C);
		DC++;
	}
}

void PRIMI::LINE(VEC2 I, VEC2 F){
	if(F.X < I.X) SWAP2(I, F);
	int DX = F.X - I.X;
	int DY = F.Y - I.Y;
	
	if(DX == 0){
		if(DY < 0){ I.Y += DY; DY = -DY; }
		VLINE(I, DY); return;
	}
	else if(DY == 0) {HLINE(I, DX); return;}
	
	if(DX > ABS(DY)){
		REAL INC_Y;
		REAL Y = 0;
		INC_Y = REAL_DIV( MAKEREAL(DY), MAKEREAL(DX) );
		for(int X=0;X<=DX;X++){
			DRAWPIXEL(VEC2(I.X+X, TRUNC(REAL_ADD(MAKEREAL(I.Y),Y))), PRIMI::C);
			Y = REAL_ADD(Y, INC_Y);
		}
	}
	else{
		REAL INC_X;
		REAL X = 0;
		DY = ABS(DY);
		INC_X = REAL_DIV( MAKEREAL(DX), MAKEREAL(DY) );
		int Y_SENSE = SGN(F.Y - I.Y);
		for(int Y=0;Y<=DY;Y++){
			int YY = Y * Y_SENSE;
			DRAWPIXEL(VEC2(TRUNC(REAL_ADD(MAKEREAL(I.X),X)), I.Y+YY), PRIMI::C);
			X = REAL_ADD(X, INC_X);
		}
	}
}

void PRIMI::VCIRCLE(CIRCLE C){
	if(C.RADIUS < 2) { DRAWPIXEL(C.CENTER, PRIMI::C); return; }
	REAL IX,FX;
	IX = MAKEREAL( GET_MID(C, CIRCLE_LEFT ).X );
	FX = MAKEREAL( GET_MID(C, CIRCLE_RIGHT).X );
	
	REAL CX,CY,RADIUS,RADIUS2,INC_X,X;
	CX = MAKEREAL(C.CENTER.X);
	CY = MAKEREAL(C.CENTER.Y);
	RADIUS  = MAKEREAL(C.RADIUS);
	RADIUS2 = REAL_MUL(RADIUS, RADIUS);
	INC_X   = REAL_DIV( REAL_ONE, RADIUS );
	
	X = IX;
	bool FIRST = true;
	while(1){
		REAL ROOOT,Y1,Y2,X1,XX;
		ROOOT = REAL_SQRT(RADIUS2 - REAL_MUL(X - CX, X - CX));
		Y1 = REDON( -ROOOT + CY );
		Y2 = REDON( +ROOOT + CY );
		X1 = TRUNC( FX - (X-IX) );
		XX = TRUNC(X);
		if(FIRST) {X1--; FIRST = false;}
		
		// 1 QUADRANT
		DRAWPIXEL(VEC2(X1, Y1), PRIMI::C);
		// 2 QUADRANT
		DRAWPIXEL(VEC2(XX, Y1), PRIMI::C);
		// 3 QUADRANT
		DRAWPIXEL(VEC2(XX, Y2), PRIMI::C);
		// 4 QUADRANT
		DRAWPIXEL(VEC2(X1, Y2), PRIMI::C);
		
		X = REAL_ADD(X, INC_X);
		if(X > CX) break;
	}
}

void PRIMI::FCIRCLE(CIRCLE C){
	if(C.RADIUS < 2) { DRAWPIXEL(C.CENTER, PRIMI::C); return; }
	REAL IX,FX;
	IX = MAKEREAL( GET_MID(C, CIRCLE_LEFT ).X );
	FX = MAKEREAL( GET_MID(C, CIRCLE_RIGHT).X );
	REAL CX,CY,RADIUS,RADIUS2,INC_X,X;
	CX = MAKEREAL(C.CENTER.X);
	CY = MAKEREAL(C.CENTER.Y);
	RADIUS  = MAKEREAL(C.RADIUS);
	RADIUS2 = REAL_MUL(RADIUS, RADIUS);
	INC_X   = REAL_DIV( REAL_ONE, RADIUS );
	X = IX;
	bool FIRST = true;
	while(1){
		REAL ROOOT,Y1,Y2,X1,XX;
		ROOOT = REAL_SQRT(RADIUS2 - REAL_MUL(X - CX, X - CX));
		Y1 = REDON( -ROOOT + CY );
		Y2 = REDON( +ROOOT + CY );
		X1 = TRUNC( FX - (X-IX) );
		XX = TRUNC(X);
		if(FIRST) {X1--; FIRST = false;}
		
		// 1 & 2 QUADRANT
		PRIMI::HLINE(VEC2(XX,Y1), X1-XX);
		// 3 & 4 QUADRANT
		PRIMI::HLINE(VEC2(XX,Y2), X1-XX);
		
		X = REAL_ADD(X, INC_X);
		if(X > CX) break;
	}
}

VOID PRIMI::VTRIANGLE(TRIANGLE T){
	VEC2 V,V1,V2;
	GET_3POS(T, V, V1, V2);
	
	PRIMI::LINE(V , V1);
	PRIMI::LINE(V1, V2);
	PRIMI::LINE(V2, V );
}

VOID PRIMI::FTRIANGLE(TRIANGLE T){
	/*
	VEC2 V,V1,V2;
	GET_3POS(T, V, V1, V2);
	EXT<TRIANGLE> EX_T;
	IF(V.Y == V1.Y OR V1.Y == V2.Y OR V.Y == V2.Y){
		PUSH(EX_T, T);
	}
	ELSE{
		TRIANGLE T1,T2;
		VEC2 MID_Y;
		VEC2 OTHER_V, OTHER_V1;
		     IF((V1 > V  AND V2 < V ) OR (V2 > V  AND V1 < V )) { MID_Y = V ; OTHER_V = V1; OTHER_V2 = V2; }
		ELSE IF((V  > V1 AND V2 < V1) OR (V2 > V1 AND V  < V1)) { MID_Y = V1; OTHER_V = V ; OTHER_V2 = V2; }
		ELSE IF((V  > V2 AND V1 < V2) OR (V1 > V2 AND V  < V2)) { MID_Y = V2; OTHER_V = V ; OTHER_V2 = V1; }
		
		VEC2 UPPER, LOWER;
		IF(OTHER_V.Y > OTHER_V1.Y){ UPPER = OTHER_V1; LOWER = OTHER_V; }
		ELSE                      { UPPER = OTHER_V ; LOWER = OTHER_V1;}
		
		VEC2 D;
		D.X = UPPER.X - LOWER.X;
		D.Y = UPPER.Y - LOWER.Y;
		VEC2 NEW_V;
		BINEXP BE_MID_X, BE_MID_Y, BE_UPPER_X, BE_DX, BE_DY;
		BE_MID_X    = MID_Y.X;
		BE_MID_Y    = MID_Y.Y;
		BE_UPPER_X = UPPER.X;
		BE_DX      = D.X;
		BE_DY      = D.Y;
		BINEXP NEW_X;
		NEW_X = (BE_MID_Y / BE_UPPER_X * BE_DY / BE_DX) - BE_MID_X;
		NEW_V.X = INT(NEW_X);
		NEW_V.Y = MID.Y;
		
		T1 = MAKETRIANGLE(MID_Y, UPPER, NEW_V);
		T2 = MAKETRIANGLE(MID_Y, LOWER, NEW_V);
		
		PUSH(EX_T, T1);
		PUSH(EX_T, T2);
	}
	
	FOR(ATYPE I=0; I<GET_SIZE(EX_T); I++){
		TRIANGLE TT;
		TT = EX_T[I];
		GET_3POS(TT, V, V1, V2);
		
	}*/
}

