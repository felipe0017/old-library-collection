// BITMAP MANIPULATION

BITMAP* HFLIP(BITMAP* BMP);
BITMAP* VFLIP(BITMAP* BMP);
BITMAP* ROTATE(BITMAP* BMP, F32 A);
BITMAP* TINT(BITMAP* BMP, COLOR C);
BITMAP* INVERSE(BITMAP* BMP);
BITMAP* DIFFUSE(BITMAP* BMP, U16 D);
BITMAP* SCALE(BITMAP* BMP, VEC2F32 F);
BITMAP* OTINT(BITMAP* BACKGROUND, BITMAP* BMP, VEC2 BMP_POS, U8 O);

BITMAP* HFLIP(BITMAP* BMP){
	BITMAP* B;
	B = create_bitmap(BMP->w, BMP->h);
	clear(B);
	
	FOR(ATYPE X=0; X<BMP->w; X++){
		FOR(ATYPE Y=0; Y<BMP->h; Y++){
			COLOR C;
			C = getpixel(BMP, X, Y);
			putpixel(B, (BMP->w - 1) - X, Y, C);
		}
	}
	
	RETURN B;
}

BITMAP* VFLIP(BITMAP* BMP){
	BITMAP* B;
	B = create_bitmap(BMP->w, BMP->h);
	clear(B);
	
	FOR(ATYPE X=0; X<BMP->w; X++){
		FOR(ATYPE Y=0; Y<BMP->h; Y++){
			COLOR C;
			C = getpixel(BMP, X, Y);
			putpixel(B, X, (BMP->h - 1) - Y, C);
		}
	}
	
	RETURN B;
}

BITMAP* ROTATE(BITMAP* BMP, F32 A){
	TEST(0);
}

BITMAP* TINT(BITMAP* BMP, COLOR C){
	BITMAP* B;
	B = create_bitmap(BMP->w, BMP->h);
	clear(B);
	
	FOR(ATYPE X=0; X<BMP->w; X++){
		FOR(ATYPE Y=0; Y<BMP->h; Y++){
			COLOR BMPC; // BITMAP COLOR
			COLOR RC;   // RESULTANT COLOR
			BMPC = getpixel(BMP, X, Y);
			RC = COLOR_TINT(BMPC, C);
			putpixel(B, X, Y, RC);
		}
	}
	
	RETURN B;
}

BITMAP* INVERSE(BITMAP* BMP){
	BITMAP* B;
	B = create_bitmap(BMP->w, BMP->h);
	clear(B);
	
	FOR(ATYPE X=0; X<BMP->w; X++){
		FOR(ATYPE Y=0; Y<BMP->h; Y++){
			COLOR C;
			C = getpixel(BMP, X, Y);
			C = COLOR_INVERSE(C);
			putpixel(B, X, Y, C);
		}
	}
	
	RETURN B;
}

// D=0 CLEAR BITMAP, D=U16_MAX COMPLETELY DIFFUSE BITMAP
// BIG VALUES OF D PRODUCE BITMAPS IN REAL-TIME
BITMAP* DIFFUSE(BITMAP* BMP, U16 D){
	BITMAP* B;
	B = create_bitmap(BMP->w, BMP->h);
	clear(B);
	
	F32 DX, DY;
	DX = MAP(D, F32(U16_MAX), F32(BMP->w - 1));
	DY = MAP(D, F32(U16_MAX), F32(BMP->h - 1));
	
	IF(D == 0 OR DX < F32(1) OR DY < F32(1)){
		draw_sprite(B, BMP, 0, 0);
		RETURN B;
	}
	
	FOR(F32 X = F32(0LL); X < F32(BMP->w - 1); X+=DX){
		FOR(F32 Y = F32(0LL); Y < F32(BMP->h - 1); Y+=DY){
			ATYPE XX, YY;
			XX = INT(X);
			YY = INT(Y);
			COLOR C;
			C = getpixel(BMP, XX, YY);
			
			rectfill(B, XX, YY, XX + INT(DX), YY + INT(DY), C);
		}
	}
	
	RETURN B;
}

BITMAP* SCALE(BITMAP* BMP, VEC2F32 F){
	IF(F.X < F32(0LL) OR F.Y < F32(0LL)) TEST(0);
	BITMAP* B;
	VEC2 NEW_SIZE;
	NEW_SIZE.X = INT(F32(BMP->w) * F.X);
	NEW_SIZE.Y = INT(F32(BMP->h) * F.Y);
	B = create_bitmap(NEW_SIZE.X, NEW_SIZE.Y);
	clear(B);
	
	FOR(F32 X=F32(0LL); X<F32(BMP->w - 1); X++){
		FOR(F32 Y=F32(0LL); Y<F32(BMP->h - 1); Y++){
			COLOR C;
			C = getpixel(BMP, INT(X), INT(Y));
			F32 XX, YY;
			XX = X * F.X;
			YY = Y * F.Y;
			rectfill(B, INT(XX), INT(YY), INT(XX + F.X), INT(YY + F.Y), C);
		}
	}
	
	RETURN B;
}

// BMP_POS IS THE POSITION WHERE BMP WILL BE DRAW IN BACKGROUND BITMAP
BITMAP* OTINT(BITMAP* BACKGROUND, BITMAP* BMP, VEC2 BMP_POS, U8 O){
	BITMAP* B;
	B = create_bitmap(BMP->w, BMP->h);
	clear(B);
	
	FOR(ATYPE X=0; X<BMP->w; X++){
		FOR(ATYPE Y=0; Y<BMP->h; Y++){
			COLOR BGC;  // BACKGROUND COLOR
			COLOR BMPC; // BITMAP COLOR
			COLOR RC;   // RESULTANT COLOR
			BMPC = getpixel(BMP, X, Y);
			BGC  = getpixel(BACKGROUND, X + BMP_POS.X, Y + BMP_POS.Y);
			
			U8 BMPR, BMPG, BMPB;
			EXTRACT_COLOR(BMPC, BMPR, BMPG, BMPB);
			BMPC = MAKECOLOR(BMPR, BMPG, BMPB, O);
			
			RC = COLOR_OTINT(BMPC, BGC);
			putpixel(B, X, Y, RC);
		}
	}
	
	RETURN B;
}
