// GRAPHICAL USER INTERFACE LIBRARY

CONST U8 BASGUI_RIGHT  = 0;
CONST U8 BASGUI_LEFT   = 1;
CONST U8 BASGUI_CENTER = 2;

/*
21-05-2022 16:36 PM
BASGUI A QUEDADO EN PAUSA HASTA CUANDO SEA NECESARIA
SE HA TESTEADO HASTA ANTES DE LA CLASE F32SLIDER
NECESITA RE-ESCRIBIRSE TODAS LAS SUBRUTINAS LOGIC(), YA QUE APENAS HOY ISE
EVENTOS DE MOUSE. EN TEORIA QUEDARIAN MUCHO MAS SIMPLIFICADAS.
COSAS POR HACER:
alguna subrutina en field para ver si una entidad esta dentro del rectangulo de la camara

ver si es posible automatizar las entidades que dlist ocultaria cuando se despliege

candeploy
deployed
deployed_r

hacer que cuando field tenga slider vertical tambien se habilite la ruedita del raton

field tambien tiene un miembro de tipo funcion llamado f, llamar a esa funcion cuando el mouse
este dentro de field::r !mouse_l y el mouse no dentro de ninguna entidad de field
*/

NAMESPACE BASGUI{
	
	STRUCT ENTITY{
		PRIVATE:
			
		PROTECTED:
			// ASSOCIATED FUNCTION TO EXECUTE WHEN THE MOUSE IS INSIDE R
			VOID (*F)();
			VOID DO_F(){
				IF(F) F();
			}
			
			VIRTUAL VOID INTERNAL_LOGIC(){}
			VIRTUAL VOID INTERNAL_DRAW (){}
			
			BOOL DEPLOYED;
			RECT DEPLOYED_R;
		
		PUBLIC:
			RECT R; // READ ONLY
			BITMAP* DESTINY;
			BOOL INTERACT; // ALLOW LOGIC()
			BOOL VISIBLE;  // ALLOW LOGIC() & DRAW()
			BOOL CANDEPLOY; // FOR EX: DLIST
			
			ENTITY(){
				THIS->INTERACT  = BTRUE;
				THIS->VISIBLE   = BTRUE;
				THIS->DESTINY   = NULL;
				THIS->CANDEPLOY = BFALSE;
				THIS->DEPLOYED  = BFALSE;
			}
			
			VOID LOGIC(){
				IF(THIS->VISIBLE AND THIS->INTERACT)
					THIS->INTERNAL_LOGIC();
			}
			
			VOID DRAW (){
				IF(THIS->VISIBLE){
					THIS->INTERNAL_DRAW();
				}
			}
	};
	
	STRUCT LABEL : PUBLIC ENTITY{
		PRIVATE:
			FRIEND VOID UPDATE(LABEL &L){
				VEC2 SIZE;
				SIZE.X = 8 * GET_SIZE(L.TEXT);
				SIZE.Y = 8;
				SET_SIZE(L.R, SIZE);
			}
			
			VOID INTERNAL_LOGIC(){
				IF(!MOUSE_L){
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
			}
			
			VOID INTERNAL_DRAW(){
				IF( EMPTY(THIS->TEXT) ) RETURN;
				
				VEC2 I,F,SIZE; GET_3POS(THIS->R, I, F, SIZE);
				SWITCH(THIS->ALIGN){
					CASE BASGUI_RIGHT:
						break;
					CASE BASGUI_LEFT:
						I.X = I.X - SIZE.X;
						break;
					CASE BASGUI_CENTER:
						I.X = I.X - (SIZE.X/2);
						break;
				}
				SET_POS(THIS->R, I);
				
				// SHADOW
				IF(THIS->SHADOW)
					textprintf(THIS->DESTINY, FONT, I.X+1, I.Y+1, COLOR_BLACK, "%s", CONST_CHAR_P(THIS->TEXT));
				
				// TEXT COLOR
				textprintf(THIS->DESTINY, FONT, I.X, I.Y, THIS->C, "%s", CONST_CHAR_P(THIS->TEXT));
			}
		
		PUBLIC:
			U8 ALIGN;
			BOOL SHADOW;
			COLOR C;
			EXT<S8> TEXT; // READ ONLY
			
			LABEL(){
				THIS->ALIGN  = BASGUI_RIGHT;
				SET(*THIS, "{TEXT}");
				THIS->SHADOW = BFALSE;
				THIS->C      = COLOR_GRAY;
			}
			
			FRIEND VOID SETPOS(LABEL &L, VEC2 POS){
				SET_POS(L.R, POS);
			}
			
			FRIEND VOID SET(LABEL &L, EXT<S8> TEXT){
				L.TEXT = TEXT;
				UPDATE(L);
			}
			
			FRIEND VOID SET(LABEL &L, CONST CHAR* TEXT){
				L.TEXT = TEXT;
				UPDATE(L);
			}
			
			FRIEND VOID SET(LABEL &L, U8 ALIGN){
				L.ALIGN = ALIGN;
			}
			
			FRIEND VOID SET(LABEL &L, BOOL SHADOW){
				L.SHADOW = SHADOW;
			}
			
			FRIEND VOID SETC(LABEL &L, COLOR C){
				L.C = C;
			}
			
			FRIEND VOID SETF(LABEL &L, VOID (*F)()){
				L.F = F;
			}
	};
	
	STRUCT BOX : PUBLIC ENTITY{
		PRIVATE:
			
			FRIEND VOID UPDATE(BOX &B){
				
			}
			
			VOID INTERNAL_LOGIC(){
				IF(!MOUSE_L){
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
			}
			
			VOID INTERNAL_DRAW(){
				VEC2 I,F; GET_2POS(THIS->R, I, F);
				
				// BACKGROUND COLOR
				U8 O;
				EXTRACT_OPACITY(THIS->BC, O);
				IF(O == OPACITY_OPAQUE)
					rectfill(THIS->DESTINY, I.X, I.Y, F.X, F.Y, THIS->BC);
				
				// BORDER COLOR
				IF(THIS->BORDER)
					rect(THIS->DESTINY, I.X, I.Y, F.X, F.Y, THIS->MC);
			}
		
		PUBLIC:
			BOOL BORDER;
			COLOR MC; // MARGEN/BORDER COLOR
			COLOR BC; // BACKGROUND COLOR
			
			BOX(){
				THIS->BC     = COLOR_DARKGRAY;
				THIS->MC     = COLOR_WHITE;
				THIS->BORDER = BFALSE;
			}
			
			FRIEND VOID SETPOS(BOX &B, VEC2 POS){
				SET_POS(B.R, POS);
			}
			
			FRIEND VOID SETSIZE(BOX &B, VEC2 SIZE){
				SET_SIZE(B.R, SIZE);
			}
			
			FRIEND VOID SET(BOX &B, BOOL BORDER){
				B.BORDER = BORDER;
			}
			
			FRIEND VOID SETMC(BOX &B, COLOR MC){
				B.MC = MC;
			}
			
			FRIEND VOID SETBC(BOX &B, COLOR BC){
				B.BC = BC;
			}
			
			FRIEND VOID SETF(BOX &B, VOID (*F)()){
				B.F = F;
			}
	};
	
	STRUCT BUTTON : PUBLIC ENTITY{
		PRIVATE:
			BOOL PRESSED; // 1 = PRESSED
			BOOL BLOCKED;
			BOOL PRE_PRESSED;
			// ASOCIATED FUNCTION TO EXECUTE WHEN PRESSED
			VOID (*F1)();
			VOID DO_F1(){
				IF(F1) F1();
			}
			
			BITMAP* BMP;
			COLOR PC; // COLOR WHEN PRESSED
			COLOR BC; // BACKGROUND COLOR
			LABEL L;
			BOOL AUTOSIZE;
			
			FRIEND VOID UPDATE(BUTTON &B){
				IF(B.BMP){
					VEC2 SIZE;
					SIZE.X = B.BMP->w;
					SIZE.Y = B.BMP->h;
					SET_SIZE(B.R, SIZE);
				}
				ELSE IF(B.AUTOSIZE){
					VEC2 SIZE;
					SIZE.X = 8 * GET_SIZE(B.L.TEXT);
					SIZE.Y = 8 * !EMPTY(B.L.TEXT);
					SET_SIZE(B.R, SIZE);
				}
			}
			
			VOID INTERNAL_LOGIC(){
				IF( !MOUSE_L ){
					THIS->BLOCKED = BFALSE;
					THIS->PRESSED = BFALSE;
				}
				IF( INSIDE2(THIS->R, V2_MOUSE) ){
					IF(!THIS->BLOCKED){
						IF(MOUSE_L){
							THIS->PRE_PRESSED = BFALSE;
							THIS->PRESSED = BTRUE;
							THIS->DO_F1();
							THIS->BLOCKED = BTRUE;
						}
						ELSE{
							THIS->PRE_PRESSED = BTRUE;
							THIS->PRESSED = BFALSE;
							THIS->DO_F();
						}
					}
				}
				ELSE{
					THIS->PRE_PRESSED = BFALSE;
					THIS->PRESSED = BFALSE;
					IF(MOUSE_L) THIS->BLOCKED = BTRUE;
				}
			}
			
			VOID INTERNAL_DRAW(){
				BOX B0X;
				B0X.DESTINY = THIS->DESTINY;
				SETPOS(B0X, GET_POS_I(THIS->R));
				SETSIZE(B0X, GET_SIZE(THIS->R));
				
				IF(THIS->BMP){
					VEC2 I;
					I = GET_POS_I(THIS->R);
					IF(THIS->PRE_PRESSED){
						SETBC(B0X, COLOR_TRANS);
						SET(B0X, BTRUE);
						SETMC(B0X, COLOR_WHITE);
						draw_sprite(THIS->DESTINY, THIS->BMP, I.X, I.Y);
					}
					ELSE IF(THIS->PRESSED){
						SETBC(B0X, THIS->PC);
					}
					ELSE{
						SETBC(B0X, COLOR_TRANS);
						draw_sprite(THIS->DESTINY, THIS->BMP, I.X, I.Y);
					}
				}
				ELSE{
					IF(THIS->PRE_PRESSED){
						SETBC(B0X, THIS->BC);
						SET(B0X, BTRUE);
						SETMC(B0X, COLOR_WHITE);
					}
					ELSE IF(THIS->PRESSED){
						SETBC(B0X, THIS->PC);
					}
					ELSE{
						SETBC(B0X, THIS->BC);
					}
				}
				
				IF(THIS->PRESSED) THIS->PRESSED = BFALSE;
				
				B0X.DRAW();
				
				VEC2 POS;
				POS = GET_MID(THIS->R);
				POS.Y = POS.Y - 8/2;
				SETPOS(THIS->L, POS);
				
				THIS->L.DESTINY = THIS->DESTINY;
				THIS->L.ALIGN = BASGUI_CENTER;
				THIS->L.DRAW();
			}
		
		PUBLIC:
			
			BUTTON(){
				THIS->BMP      = NULL;
				THIS->PRESSED  = BFALSE;
				THIS->BLOCKED  = BFALSE;
				THIS->PRE_PRESSED = BFALSE;
				
				THIS->BC = COLOR_DARKGRAY;
				THIS->PC = COLOR_GREEN;
				THIS->AUTOSIZE = BTRUE;
				UPDATE(*THIS);
			}
			
			FRIEND VOID SETPOS(BUTTON &B, VEC2 POS){
				SET_POS(B.R, POS);
				UPDATE(B);
			}
			
			FRIEND VOID SETSIZE(BUTTON &B, VEC2 SIZE){
				SET_SIZE(B.R, SIZE);
				B.AUTOSIZE = BFALSE;
				UPDATE(B);
			}
			
			FRIEND VOID SET(BUTTON &B, BITMAP* BMP){
				B.BMP = BMP;
				UPDATE(B);
			}
			
			FRIEND VOID SETPC(BUTTON &B, COLOR PC){
				B.PC = PC;
			}
			
			FRIEND VOID SETBC(BUTTON &B, COLOR BC){
				B.BC = BC;
			}
			
			FRIEND VOID SET(BUTTON &B, LABEL L){
				B.L = L;
			}
			
			FRIEND VOID SETF(BUTTON &B, VOID (*F)()){
				B.F = F;
			}
			
			FRIEND VOID SETF1(BUTTON &B, VOID (*F1)()){
				B.F1 = F1;
			}
			
			FRIEND VOID SETAS(BUTTON &B, BOOL AUTOSIZE){
				B.AUTOSIZE = AUTOSIZE;
				UPDATE(B);
			}
	};
	
	STRUCT PBAR : PUBLIC ENTITY{
		PRIVATE:
			U16 P; // PROGRESS: ]0, 256[
			
			FRIEND VOID UPDATE(PBAR &B){
				
			}
			
			VOID INTERNAL_LOGIC(){
				IF(!MOUSE_L){
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
			}
			
			VOID INTERNAL_DRAW(){
				ATYPE WIDTH;
				WIDTH = ( GET_SIZE(THIS->R).X * THIS->P ) / 256;
				VEC2 I,F; GET_2POS(THIS->R, I, F);
				
				// BACKGROUND COLOR
				IF(WIDTH){
					IF(THIS->P < 256){
						rectfill(THIS->DESTINY, I.X+WIDTH, I.Y+1, F.X-1, F.Y-1, THIS->BC);
					}
				}
				ELSE{
					rectfill(THIS->DESTINY, I.X+1, I.Y+1, F.X-1, F.Y-1, THIS->BC);
				}
				
				// PROGRESS COLOR
				IF(THIS->P == 256){
					rectfill(THIS->DESTINY, I.X+1, I.Y+1, F.X-1, F.Y-1, THIS->PC);
				}
				ELSE IF(WIDTH){
					rectfill(THIS->DESTINY, I.X+1, I.Y+1, I.X+WIDTH-1, F.Y-1, THIS->PC);
				}
				
				// BORDER COLOR
				rect(THIS->DESTINY, I.X, I.Y, F.X, F.Y, THIS->MC);
			}

		PUBLIC:
			COLOR BC; // BACKGROUND COLOR
			COLOR MC; // BORDER/MARGEN COLOR
			COLOR PC; // PROGRESS COLOR

			PBAR(){
				THIS->P = 0;
				THIS->PC = COLOR_GREEN;
				THIS->MC = COLOR_WHITE;
				THIS->BC = COLOR_DARKGRAY;
			}
			
			FRIEND VOID SETPOS(PBAR &B, VEC2 POS){
				SET_POS(B.R, POS);
			}
			
			FRIEND VOID SETSIZE(PBAR &B, VEC2 SIZE){
				SET_SIZE(B.R, SIZE);
			}
			
			FRIEND VOID SETMC(PBAR &B, COLOR MC){
				B.MC = MC;
			}
			
			FRIEND VOID SETBC(PBAR &B, COLOR BC){
				B.BC = BC;
			}
			
			FRIEND VOID SETPC(PBAR &B, COLOR PC){
				B.PC = PC;
			}
			
			FRIEND VOID SETF(PBAR &B, VOID (*F)()){
				B.F = F;
			}
			
			FRIEND VOID SET(PBAR &B, U16 PROGRESS){
				B.P = MIN(PROGRESS, 256);
			}
			
			FRIEND VOID ADD(PBAR &B, U16 ADD){
				B.P = MIN(B.P + ADD, 256);
			}
	};
	
	STRUCT CHECKBOX : PUBLIC ENTITY{
		PRIVATE:
			BOOL BLOCKED;
			BOOL *VAR; // ATTACHED VAR, SEE TYPE!
			BOOL AUTOSIZE;
			
			FRIEND VOID UPDATE(CHECKBOX &CB){
				IF(CB.AUTOSIZE) SET_SIZE(CB.R, VEC2(8));
			}
			
			VOID INTERNAL_LOGIC(){
				IF( !MOUSE_L ){
					THIS->BLOCKED = BFALSE;
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
				IF( INSIDE2(THIS->R, V2_MOUSE) ){
					IF(!THIS->BLOCKED){
						IF(MOUSE_L){
							*(THIS->VAR) = !( *(THIS->VAR) );
							THIS->BLOCKED = BTRUE;
						}
					}
				}
				ELSE IF(MOUSE_L) THIS->BLOCKED = BTRUE;
			}
			
			VOID INTERNAL_DRAW(){
				BOX B0X;
				B0X.DESTINY = THIS->DESTINY;
				
				SETPOS(B0X, GET_POS_I(THIS->R));
				SETSIZE(B0X, GET_SIZE(THIS->R));
				SET(B0X, BTRUE);
				SETMC(B0X, COLOR_WHITE);
				
				SETBC(B0X, (*(THIS->VAR))? COLOR_GREEN : COLOR_RED );
				B0X.DRAW();
			}

		PUBLIC:
			
			CHECKBOX(){
				THIS->VAR = NULL;
				THIS->BLOCKED = BFALSE;
				THIS->AUTOSIZE = BTRUE;
				UPDATE(*THIS);
			}
			
			FRIEND VOID SETPOS(CHECKBOX &CB, VEC2 POS){
				SET_POS(CB.R, POS);
			}
			
			FRIEND VOID SETSIZE(CHECKBOX &CB, VEC2 SIZE){
				SET_SIZE(CB.R, SIZE);
				CB.AUTOSIZE = BFALSE;
			}
			
			FRIEND VOID SET(CHECKBOX &CB, BOOL &VAR){
				CB.VAR = &VAR;
				*(CB.VAR) = BTRUE;
			}
			
			FRIEND VOID SETF(CHECKBOX &CB, VOID (*F)()){
				CB.F = F;
			}
			
			FRIEND VOID SETAS(CHECKBOX &CB, BOOL AUTOSIZE){
				CB.AUTOSIZE = AUTOSIZE;
				UPDATE(CB);
			}
	};
	
	STRUCT TEXTBOX : PUBLIC ENTITY{
		PRIVATE:
			U8 MAX_LENGTH; // MAX LENGTH OF TEXT
			BOOL BLOCKED;
			BOOL FOCUS;
			EXT<S8> *VAR; // ATTACHED VAR, SEE TYPE!
			
			FRIEND VOID UPDATE(TEXTBOX &TB){
				VEC2 SIZE;
				SIZE.X = 8 * TB.MAX_LENGTH;
				SIZE.Y = 8;
				SET_SIZE(TB.R, SIZE);
			}
			
			VOID INTERNAL_LOGIC(){
				IF( !MOUSE_L ){
					THIS->BLOCKED = BFALSE;
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
				IF( INSIDE2(THIS->R, V2_MOUSE) ){
					IF(!THIS->BLOCKED){
						IF(MOUSE_L){
							THIS->FOCUS   = BTRUE;
							THIS->BLOCKED = BTRUE;
						}
					}
				}
				ELSE IF(MOUSE_L){
					THIS->FOCUS   = BFALSE;
					THIS->BLOCKED = BTRUE;
				}
				
				IF(THIS->FOCUS){
					FOR(ATYPE I=0; I<KEY_MAX; I++){
						IF(KB_UP(I)){
							IF(GET_SIZE(*(THIS->VAR)) < THIS->MAX_LENGTH){
								IF(I >= KEY_A AND I <= KEY_Z){
									PUSH( *(THIS->VAR), 'A' + I - KEY_A);
								}
								IF(I >= KEY_0 AND I <= KEY_9){
									PUSH( *(THIS->VAR), '0' + I - KEY_0);
								}
								IF( I == KEY_SPACE){
									PUSH( *(THIS->VAR), ' ');
								}
							}
							
							IF( I == KEY_BACKSPACE){
								IF(GET_SIZE(*(THIS->VAR)) > 0)
									POP( *(THIS->VAR) );
							}
							IF( I == KEY_DEL){
								FREE( *(THIS->VAR) );
							}
						}
					}
				}
			}
			
			VOID INTERNAL_DRAW(){
				// BACKGROUND
				BOX B0X;
				B0X.DESTINY = THIS->DESTINY;
				SETPOS(B0X, GET_POS_I(THIS->R));
				SETSIZE(B0X, GET_SIZE(THIS->R));
				SET(B0X, THIS->FOCUS);
				SETBC(B0X, THIS->BC);
				B0X.DRAW();
				
				VEC2 POS;
				POS = GET_MID(THIS->R);
				POS.Y = POS.Y - 8/2;
				SETPOS(THIS->H, POS);
				SETPOS(THIS->L, POS);
				
				IF( EMPTY(*(THIS->VAR)) ){
					THIS->H.ALIGN = BASGUI_CENTER;
					THIS->H.DESTINY = THIS->DESTINY;
					THIS->H.DRAW();
				}
				ELSE{
					SET(THIS->L, *(THIS->VAR));
					THIS->L.ALIGN = BASGUI_CENTER;
					THIS->L.DESTINY = THIS->DESTINY;
					THIS->L.DRAW();
				}
			}
			
		PUBLIC:
			LABEL L;
			LABEL H; // WHEN SIZE OF L IS ZERO, THIS LABEL WILL BE DRAW
			COLOR BC; // BACKGROUND COLOR
			
			TEXTBOX(){
				THIS->VAR = NULL;
				THIS->BC = COLOR_DARKGRAY;
				SET(THIS->H, "WRITE");
				THIS->BLOCKED     = BFALSE;
				THIS->FOCUS       = BFALSE;
				THIS->MAX_LENGTH  = 0;
			}
			
			FRIEND VOID SETL(TEXTBOX &TB, LABEL L){
				TB.L = L;
			}
			
			FRIEND VOID SETH(TEXTBOX &TB, LABEL H){
				TB.H = H;
			}
			
			FRIEND VOID SETBC(TEXTBOX &TB, COLOR BC){
				TB.BC = BC;
			}
			
			FRIEND VOID SETPOS(TEXTBOX &TB, VEC2 POS){
				SET_POS(TB.R, POS);
			}
			
			FRIEND VOID SET(TEXTBOX &TB, U8 MAX_LENGTH){
				TB.MAX_LENGTH = MAX_LENGTH;
				UPDATE(TB);
			}
			
			FRIEND VOID SETF(TEXTBOX &TB, VOID (*F)()){
				TB.F = F;
			}
			
			FRIEND VOID SET(TEXTBOX &TB, EXT<S8> &VAR){
				TB.VAR = &VAR;
				FREE(*(TB.VAR));
			}
	};
	
	STRUCT SPBAR : PUBLIC ENTITY{
		PRIVATE:
			U16 P; // PROGRESS: ]0, 256[
			EXT<U16> LIMITS;   // LIMIT OF STATE
			EXT<COLOR> COLORS; // COLOR OF STATE
			
			FRIEND VOID UPDATE(SPBAR &B){
				
			}
			
			VOID INTERNAL_LOGIC(){
				IF( !MOUSE_L ){
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
			}
			
			VOID INTERNAL_DRAW(){
				PBAR PB4R;
				PB4R.DESTINY = THIS->DESTINY;
				SET(PB4R, THIS->P);
				SETPOS(PB4R, GET_POS_I(THIS->R));
				SETSIZE(PB4R, GET_SIZE(THIS->R));
				
				// PROGRESS COLOR
				FOR(ATYPE I=0; I<GET_SIZE(THIS->LIMITS); I++){
					IF(THIS->P >= THIS->LIMITS[I]) THIS->PC = THIS->COLORS[I];
				}
				SETPC(PB4R, THIS->PC);
				PB4R.DRAW();
			}

		PUBLIC:
			COLOR PC; // PROGRESS COLOR
			COLOR MC; // BORDER/MARGEN COLOR
			COLOR BC; // BACKGROUND COLOR

			SPBAR(){
				THIS->P = 0;
				THIS->PC = COLOR_GREEN;
				THIS->MC = COLOR_WHITE;
				THIS->BC = COLOR_DARKGRAY;
			}
			
			FRIEND VOID SETPC(SPBAR &B, COLOR PC){
				B.PC = PC;
			}
			
			FRIEND VOID SETMC(SPBAR &B, COLOR MC){
				B.MC = MC;
			}
			
			FRIEND VOID SETBC(SPBAR &B, COLOR BC){
				B.BC = BC;
			}
			
			FRIEND VOID SETPOS(SPBAR &B, VEC2 POS){
				SET_POS(B.R, POS);
			}
			
			FRIEND VOID SETSIZE(SPBAR &B, VEC2 SIZE){
				SET_SIZE(B.R, SIZE);
			}
			
			FRIEND VOID SETF(SPBAR &B, VOID (*F)()){
				B.F = F;
			}
			
			FRIEND VOID ADD(SPBAR &B, U16 LIMIT, COLOR C){
				PUSH(B.LIMITS, LIMIT);
				PUSH(B.COLORS, C);
			}
			
			FRIEND VOID SET(SPBAR &B, U16 PROGRESS){
				B.P = MIN(PROGRESS, 256);
			}
			
			FRIEND VOID ADD(SPBAR &B, U16 ADD){
				B.P = MIN(B.P + ADD, 256);
			}
	};
	
	STRUCT IMAGE : PUBLIC ENTITY{
		PRIVATE:
			BITMAP* BMP;
			
			FRIEND VOID UPDATE(IMAGE &I){
				IF(I.BMP){
					VEC2 SIZE;
					SIZE.X = I.BMP->w;
					SIZE.Y = I.BMP->h;
					SET_SIZE(I.R, SIZE);
				}
			}
			
			VOID INTERNAL_LOGIC(){
				IF( !MOUSE_L ){
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
			}
			
			VOID INTERNAL_DRAW(){
				VEC2 I,F; GET_2POS(THIS->R, I, F);
				draw_sprite(THIS->DESTINY, THIS->BMP, I.X, I.Y);
				
				IF(THIS->BORDER)
					rect(THIS->DESTINY, I.X, I.Y, F.X, F.Y, THIS->MC);
			}
			
		PUBLIC:
			BOOL BORDER;
			COLOR MC;
			
			IMAGE(){
				THIS->BMP = NULL;
				THIS->BORDER = BTRUE;
				THIS->MC = COLOR_WHITE;
			}
			
			FRIEND VOID SET(IMAGE &I, BOOL BORDER){
				I.BORDER = BORDER;
			}
			
			FRIEND VOID SETMC(IMAGE &I, COLOR MC){
				I.MC = MC;
			}
			
			FRIEND VOID SET(IMAGE &I, BITMAP* BMP){
				I.BMP = BMP;
				UPDATE(I);
			}
			
			FRIEND VOID SETF(IMAGE &I, VOID (*F)()){
				I.F = F;
			}
			
			FRIEND VOID SETPOS(IMAGE &I, VEC2 POS){
				SET_POS(I.R, POS);
			}
	};
	
	STRUCT CANVAS : PUBLIC ENTITY{
		PRIVATE:
			BITMAP* BMP;
			BOOL FOCUS;
			BOOL BLOCKED;
			ATYPE BMP_INDEX;
			
			FRIEND VOID UPDATE(CANVAS &C){
				IF(C.BMP_INDEX != -1){
					FREE_BMP(C.BMP_INDEX);
				}
				VEC2 SIZE;
				SIZE = GET_SIZE(C.R);
				C.BMP_INDEX = CREATE_BMP(C.BMP, SIZE);
				clear(C.BMP);
			}
			
			VOID INTERNAL_LOGIC(){
				IF( !MOUSE_L ){
					THIS->BLOCKED = BFALSE;
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
				IF( INSIDE2(THIS->R, V2_MOUSE) ){
					IF(!THIS->BLOCKED){
						IF(MOUSE_L){
							THIS->FOCUS   = BTRUE;
							THIS->BLOCKED = BTRUE;
						}
					}
				}
				ELSE IF(MOUSE_L){
					THIS->FOCUS   = BFALSE;
					THIS->BLOCKED = BTRUE;
				}
			}
			
			VOID INTERNAL_DRAW(){
				VEC2 I;
				I = GET_POS_I(THIS->R);
				draw_sprite(THIS->DESTINY, THIS->BMP, I.X, I.Y);
			}

		PUBLIC:
			
			CANVAS(){
				THIS->BMP = NULL;
				THIS->FOCUS = BFALSE;
				THIS->BLOCKED = BFALSE;
				THIS->BMP_INDEX = -1;
			}
			
			FRIEND VOID SETPOS(CANVAS &C, VEC2 POS){
				SET_POS(C.R, POS);
			}
			
			FRIEND VOID SETSIZE(CANVAS &C, VEC2 SIZE){
				SET_SIZE(C.R, SIZE);
				UPDATE(C);
			}
			
			FRIEND VOID SETF(CANVAS &C, VOID (*F)()){
				C.F = F;
			}
			
			FRIEND VOID PIXEL(CANVAS &C, VEC2 POS, COLOR COL){
				putpixel(C.BMP, POS.X, POS.Y, COL);
			}
			
			FRIEND VOID VRECT(CANVAS &C, RECT R, COLOR COL){
				VEC2 I,F; GET_2POS(R, I, F);
				rect(C.BMP, I.X, I.Y, F.X, F.Y, COL);
			}
			
			FRIEND VOID FRECT(CANVAS &C, RECT R, COLOR COL){
				VEC2 I,F; GET_2POS(R, I, F);
				rectfill(C.BMP, I.X, I.Y, F.X, F.Y, COL);
			}
			
			FRIEND VOID BLIT(CANVAS &C, BITMAP* BMP, VEC2 SOURCE, VEC2 DEST, VEC2 SIZE){
				blit(BMP, C.BMP, SOURCE.X, SOURCE.Y, DEST.X, DEST.Y, SIZE.X, SIZE.Y);
			}
			
			FRIEND VOID DRAW(CANVAS &C, BITMAP* BMP, VEC2 POS){
				draw_sprite(C.BMP, BMP, POS.X, POS.Y);
			}
			
			FRIEND VOID CLEAR(CANVAS &C, COLOR CC = COLOR_BLACK){
				IF( CC == COLOR_BLACK ){
					clear(C.BMP);
				}
				ELSE{
					VEC2 I,F; GET_2POS(C.R, I, F);
					rectfill(C.BMP, I.X, I.Y, F.X, F.Y, CC);
				}
			}
	};
	
	STRUCT SLIDER_BASE{
		PROTECTED:
			ATYPE IV; // INIT  VALUE OF RANGE
			ATYPE FV; // FINAL VALUE OF RANGE
			ATYPE *VAR; // ATTACHED VAR, SEE TYPE!
			BOOL BLOCKED;
		
		PUBLIC:
			COLOR BC; // BACKGROUND COLOR
			COLOR IC; // INDICATOR COLOR
			
			SLIDER_BASE(){
				THIS->VAR = NULL;
				THIS->BLOCKED = BFALSE;
				THIS->BC = COLOR_DARKGRAY;
				THIS->IC = COLOR_YELLOW;
			}
	};
	
	STRUCT SLIDER : PUBLIC ENTITY, PUBLIC SLIDER_BASE{
		PRIVATE:
			FRIEND VOID UPDATE(SLIDER &S){
				IF(S.VAR) *(S.VAR) = S.IV;
			}
			
			VOID INTERNAL_LOGIC(){
				IF( !MOUSE_L ){
					THIS->BLOCKED = BFALSE;
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
				IF( INSIDE2(THIS->R, V2_MOUSE) ){
					IF(!THIS->BLOCKED AND MOUSE_L){
						VEC2 I,F; GET_2POS(THIS->R, I, F);
						*(THIS->VAR) = MAP(MOUSE_X, I.X, F.X, THIS->IV, THIS->FV);
					}
				}
				ELSE{
					IF(!THIS->BLOCKED AND MOUSE_L){
						VEC2 I,F; GET_2POS(THIS->R, I, F);
						     IF(MOUSE_X > F.X) *(THIS->VAR) = THIS->FV;
						ELSE IF(MOUSE_X < I.X) *(THIS->VAR) = THIS->IV;
					}
					ELSE IF(!MOUSE_L) THIS->BLOCKED = BTRUE;
				}
			}
			
			VOID INTERNAL_DRAW(){
				VEC2 I,F; GET_2POS(THIS->R, I, F);
				rectfill(THIS->DESTINY, I.X, I.Y, F.X, F.Y, THIS->BC);
				ATYPE INDEX = MAP(*(THIS->VAR), THIS->IV, THIS->FV, I.X, F.X);
				line(THIS->DESTINY, INDEX, I.Y, INDEX, F.Y, THIS->IC);
			}
		
		PUBLIC:
			
			SLIDER(){
				
			}
			
			FRIEND VOID SETF(SLIDER &S, VOID (*F)()){
				S.F = F;
			}
			
			FRIEND VOID SETBC(SLIDER &S, COLOR BC){
				S.BC = BC;
			}
			
			FRIEND VOID SETIC(SLIDER &S, COLOR IC){
				S.IC = IC;
			}
			
			FRIEND VOID SETPOS(SLIDER &S, VEC2 POS){
				SET_POS(S.R, POS);
			}
			
			FRIEND VOID SETSIZE(SLIDER &S, VEC2 SIZE){
				SET_SIZE(S.R, SIZE);
			}
			
			FRIEND VOID SET(SLIDER &S, ATYPE IV, ATYPE FV){
				S.IV = IV;
				S.FV = FV;
				UPDATE(S);
			}
			
			FRIEND VOID SET(SLIDER &S, ATYPE &VAR){
				S.VAR = &VAR;
				UPDATE(S);
			}
	};
	
	STRUCT VSLIDER : PUBLIC ENTITY, PUBLIC SLIDER_BASE{
		PRIVATE:
			FRIEND VOID UPDATE(VSLIDER &S){
				IF(S.VAR) *(S.VAR) = S.IV;
			}
			
			VOID INTERNAL_LOGIC(){
				IF( !MOUSE_L ){
					THIS->BLOCKED = BFALSE;
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
				IF( INSIDE2(THIS->R, V2_MOUSE) ){
					IF(!THIS->BLOCKED AND MOUSE_L){
						VEC2 I,F; GET_2POS(THIS->R, I, F);
						*(THIS->VAR) = MAP(MOUSE_Y, I.Y, F.Y, THIS->IV, THIS->FV);
					}
				}
				ELSE{
					IF(!THIS->BLOCKED AND MOUSE_L){
						VEC2 I,F; GET_2POS(THIS->R, I, F);
						     IF(MOUSE_Y > F.Y) *(THIS->VAR) = THIS->FV;
						ELSE IF(MOUSE_Y < I.Y) *(THIS->VAR) = THIS->IV;
					}
					ELSE IF(!MOUSE_L) THIS->BLOCKED = BTRUE;
				}
			}
			
			VOID INTERNAL_DRAW(){
				VEC2 I,F; GET_2POS(THIS->R, I, F);
				rectfill(THIS->DESTINY, I.X, I.Y, F.X, F.Y, THIS->BC);
				ATYPE INDEX = MAP( *(THIS->VAR) , THIS->IV, THIS->FV, I.Y, F.Y);
				line(THIS->DESTINY, I.X, INDEX, F.X, INDEX, THIS->IC);
			}
		
		PUBLIC:
			
			VSLIDER(){
				
			}
			
			FRIEND VOID SETBC(VSLIDER &S, COLOR BC){
				S.BC = BC;
			}
			
			FRIEND VOID SETIC(VSLIDER &S, COLOR IC){
				S.IC = IC;
			}
			
			FRIEND VOID SETF(VSLIDER &S, VOID (*F)()){
				S.F = F;
			}
			
			FRIEND VOID SETPOS(VSLIDER &S, VEC2 POS){
				SET_POS(S.R, POS);
			}
			
			FRIEND VOID SETSIZE(VSLIDER &S, VEC2 SIZE){
				SET_SIZE(S.R, SIZE);
			}
			
			FRIEND VOID SET(VSLIDER &S, ATYPE IV, ATYPE FV){
				S.IV = IV;
				S.FV = FV;
				UPDATE(S);
			}
			
			FRIEND VOID SET(VSLIDER &S, ATYPE &VAR){
				S.VAR = &VAR;
				UPDATE(S);
			}
	};
	
	STRUCT F32SLIDER : PUBLIC ENTITY{
		PRIVATE:
			F32 IV; // INIT  VALUE OF RANGE
			F32 FV; // FINAL VALUE OF RANGE
			F32 *VAR; // ATTACHED VAR, SEE TYPE!
			BOOL BLOCKED;
			F32 DX;
			
			FRIEND VOID UPDATE(F32SLIDER &S){
				IF(S.VAR) *(S.VAR) = S.IV;
				VEC2 SIZE; SIZE = GET_SIZE(S.R);
				S.DX = (S.FV - S.IV) / F32(SIZE.X);
			}
			
			VOID INTERNAL_LOGIC(){
				IF( !MOUSE_L ){
					THIS->BLOCKED = BFALSE;
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
				IF( INSIDE2(THIS->R, V2_MOUSE) ){
					IF(!THIS->BLOCKED AND MOUSE_L){
						VEC2 I,F; GET_2POS(THIS->R, I, F);
						*(THIS->VAR) = THIS->DX * F32(MOUSE_X - I.X);
					}
				}
				ELSE{
					IF(!THIS->BLOCKED AND MOUSE_L){
						VEC2 I,F; GET_2POS(THIS->R, I, F);
						     IF(MOUSE_X > F.X) *(THIS->VAR) = THIS->FV;
						ELSE IF(MOUSE_X < I.X) *(THIS->VAR) = THIS->IV;
					}
					ELSE IF(!MOUSE_L) THIS->BLOCKED = BTRUE;
				}
			}
			
			VOID INTERNAL_DRAW(){
				VEC2 I,F,SIZE; GET_3POS(THIS->R, I, F, SIZE);
				rectfill(THIS->DESTINY, I.X, I.Y, F.X, F.Y, THIS->BC);
				ATYPE INDEX = INT(MAP(*(THIS->VAR), THIS->IV, THIS->FV, F32(I.X), F32(F.X))); !!!!
				//ATYPE INDEX = INT((*(THIS->VAR) / (THIS->FV - THIS->IV)) * F32(SIZE.X)) + I.X;
				line(THIS->DESTINY, INDEX, I.Y, INDEX, F.Y, THIS->IC);
			}
		
		PUBLIC:
			COLOR BC; // BACKGROUND COLOR
			COLOR IC; // INDICATOR COLOR
			
			F32SLIDER(){
				THIS->VAR = NULL;
				THIS->BLOCKED = BFALSE;
				THIS->BC = COLOR_DARKGRAY;
				THIS->IC = COLOR_PURPLE;
			}
			
			FRIEND VOID SETBC(F32SLIDER &S, COLOR BC){
				S.BC = BC;
			}
			
			FRIEND VOID SETIC(F32SLIDER &S, COLOR IC){
				S.IC = IC;
			}
			
			FRIEND VOID SETF(F32SLIDER &S, VOID (*F)()){
				S.F = F;
			}
			
			FRIEND VOID SETPOS(F32SLIDER &S, VEC2 POS){
				SET_POS(S.R, POS);
			}
			
			FRIEND VOID SETSIZE(F32SLIDER &S, VEC2 SIZE){
				SET_SIZE(S.R, SIZE);
				UPDATE(S);
			}
			
			FRIEND VOID SET(F32SLIDER &S, F32 IV, F32 FV){
				S.IV = IV;
				S.FV = FV;
				UPDATE(S);
			}
			
			FRIEND VOID SET(F32SLIDER &S, F32 &VAR){
				S.VAR = &VAR;
				UPDATE(S);
			}
	};
	
	STRUCT LIST : PUBLIC ENTITY{
		PRIVATE:
			EXT< EXT<S8> > ITEM;
			ATYPE *VAR; // ATTACHED VAR, SEE TYPE!
			U8 VE; // VISUAL_ELEMENTS
			ATYPE YD;
			BOOL BLOCKED;
			ATYPE VVAR; // VSLIDER VAR
			
			FRIEND VOID UPDATE(LIST &L){
				VEC2 SIZE;
				SIZE.X = 0;
				FOR(ATYPE I=0; I<GET_SIZE(L.ITEM); I++){
					ATYPE LENGTH;
					LENGTH = GET_SIZE(L.ITEM[I]) * 8;
					IF(LENGTH > SIZE.X) SIZE.X = LENGTH;
				}
				SIZE.Y = MIN(L.VE, GET_SIZE(L.ITEM)) * 8;
				SET_SIZE(L.R, SIZE);
				
				IF(GET_SIZE(L.ITEM) > L.VE){
					L.V.VISIBLE = BTRUE;
					
					VEC2 I,F,S; GET_3POS(L.R, I, F, S);
					SET_POS(L.V.R, VEC2(F.X+1, I.Y));
					SET_SIZE(L.V.R, VEC2(15, S.Y));
					SET(L.V, 0, GET_SIZE(L.ITEM) - L.VE);
					SET(L.V, L.VVAR);
				}
			}
			
			VOID INTERNAL_LOGIC(){
				IF(THIS->V.VISIBLE){
					THIS->V.LOGIC();
					THIS->YD = THIS->VVAR;
				}
				
				IF( !MOUSE_L ){
					THIS->BLOCKED = BFALSE;
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
				IF(INSIDE2(THIS->R, V2_MOUSE)){
					IF(MOUSE_L AND !THIS->BLOCKED){
						VEC2 POS;
						POS = GET_POS_I(THIS->R);
						*(THIS->VAR) = THIS->YD + (MOUSE_Y - POS.Y) / 8;
						THIS->BLOCKED = BTRUE;
					}
				}
				ELSE{
					IF(MOUSE_L) THIS->BLOCKED = BTRUE;
				}
			}
			
			VOID INTERNAL_DRAW(){
				IF(THIS->V.VISIBLE){
					THIS->V.DESTINY = THIS->DESTINY;
					THIS->V.DRAW();
				}
				
				// BACKGROUND
				BOX B0X;
				B0X.DESTINY = THIS->DESTINY;
				B0X.R = THIS->R;
				B0X.BORDER = BTRUE;
				B0X.MC = THIS->MC;
				B0X.BC  = THIS->BC;
				B0X.DRAW();
				
				VEC2 POS;
				THIS->L.DESTINY = THIS->DESTINY;
				THIS->L.ALIGN = BASGUI_RIGHT;
				
				FOR(U8 I=0; I<MIN(THIS->VE, GET_SIZE(THIS->ITEM)); I++){
					SET(THIS->L, THIS->ITEM[THIS->YD + I]);
					
					POS = GET_POS_I(THIS->R);
					POS.X = POS.X;
					POS.Y = POS.Y + I * 8;
					SET_POS(THIS->L.R, POS);
					
					IF( THIS->YD + I == *(THIS->VAR) ){
						SET(THIS->SL, THIS->L.TEXT);
						THIS->SL.DESTINY = THIS->DESTINY;
						SET_POS(THIS->SL.R, POS);
						THIS->SL.DRAW();
					}
					ELSE THIS->L.DRAW();
				}
			}
		
		PUBLIC:
			VSLIDER V;
			LABEL L;
			LABEL SL; // LABEL OF SELECTION ITEM
			COLOR MC; // MARGIN COLOR
			COLOR BC; // BACKGROUND COLOR
			
			LIST(){
				THIS->VAR = NULL;
				THIS->VVAR = 0;
				THIS->BLOCKED = BFALSE;
				THIS->MC   = COLOR_WHITE;
				THIS->L.C  = COLOR_GRAY;
				THIS->SL.C = COLOR_GREEN;
				THIS->BC   = COLOR_DARKGRAY;
				THIS->YD   = 0;
				THIS->V.VISIBLE = BFALSE;
			}
			
			FRIEND VOID SET(LIST &L, VSLIDER V){
				L.V = V;
			}
			
			FRIEND VOID SETL(LIST &L, LABEL LL){
				L.L = LL;
			}
			
			FRIEND VOID SETSL(LIST &L, LABEL SL){
				L.SL = SL;
			}
			
			FRIEND VOID SETMC(LIST &L, COLOR MC){
				L.MC = MC;
			}
			
			FRIEND VOID SETBC(LIST &L, COLOR BC){
				L.BC = BC;
			}
			
			FRIEND VOID SETF(LIST &L, VOID (*F)()){
				L.F = F;
			}
			
			FRIEND VOID SETPOS(LIST &L, VEC2 POS){
				SET_POS(L.R, POS);
				UPDATE(L);
			}
			
			FRIEND VOID SET(LIST &L, U8 VISUAL_ELEMENTS){
				L.VE = VISUAL_ELEMENTS;
				UPDATE(L);
			}
			
			FRIEND VOID SET(LIST &L, ATYPE &VAR){
				L.VAR = &VAR;
				*(L.VAR) = 0;
			}
			
			FRIEND VOID PUT(LIST &L, EXT<S8> TEXT){
				PUSH(L.ITEM, TEXT);
				UPDATE(L);
			}
			
			FRIEND VOID PUT(LIST &L, CONST CHAR* TEXT){
				EXT<S8> E;
				E = TEXT;
				PUT(L, E);
			}
	};
	
	STRUCT DLIST : PUBLIC ENTITY{
		PRIVATE:
			// ENTITIES THAT ARE OBFUSCATED BY DISPLAYED LIST RECTANGLE NEED BE MANAGEMENT EXTERNALLY
			EXT< EXT<S8> > ITEM;
			EXT<ATYPE> NOT_SELECTABLE; // INDEX OF ITEMS THAT ARE NOT SELECTABLE
			ATYPE *VAR;
			BOOL DISPLAYED;
			RECT DR; // DISPLAYED RECT
			BOOL BLOCKED;
			ATYPE FI; // FOCUS ITEM
			
			FRIEND VOID UPDATE(DLIST &L){
				VEC2 SIZE;
				SIZE.X = GET_SIZE(L.DL.TEXT) * 8;
				FOR(ATYPE I=0; I<GET_SIZE(L.ITEM); I++){
					ATYPE LENGTH;
					LENGTH = GET_SIZE(L.ITEM[I]) * 8;
					IF(LENGTH > SIZE.X) SIZE.X = LENGTH;
				}
				SIZE.Y = 1 * 8;
				SET_SIZE(L.R, SIZE);
				
				VEC2 I,F; GET_3POS(L.R, I, F, SIZE);
				L.DR = MAKERECTS(VEC2(I.X, I.Y + 8), VEC2(SIZE.X, GET_SIZE(L.ITEM) * 8));
				L.DEPLOYED_R = L.DR;
			}
			
			VOID INTERNAL_LOGIC(){
				IF( !MOUSE_L ){
					THIS->BLOCKED = BFALSE;
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
				IF(INSIDE2(THIS->R, V2_MOUSE)){
					IF(MOUSE_L AND !THIS->BLOCKED){
						THIS->BLOCKED = BTRUE;
						IF(THIS->DISPLAYED){
							THIS->DISPLAYED = BFALSE;
						}
						ELSE{
							THIS->DISPLAYED = BTRUE;
						}
						THIS->DEPLOYED = THIS->DISPLAYED;
					}
				}
				ELSE{
					IF(MOUSE_L){
						THIS->BLOCKED = BTRUE;
						IF(THIS->DISPLAYED){
							THIS->DISPLAYED = BFALSE;
							THIS->DEPLOYED  = BFALSE;
						}
					}
				}
				
				IF(THIS->DISPLAYED){
					IF(INSIDE2(THIS->DR, V2_MOUSE)){
						THIS->DO_F();
						
						VEC2 POS;
						POS = GET_POS_I(THIS->DR);
						ATYPE SELECTION;
						SELECTION = (MOUSE_Y - POS.Y) / 8;
						THIS->FI = SELECTION;
						IF(MOUSE_L AND !THIS->BLOCKED){
							ATYPE INDEX;
							IF( !FIND(THIS->NOT_SELECTABLE, THIS->FI, INDEX) )
								*(THIS->VAR) = SELECTION;
							THIS->FI = -1;
							
							THIS->DISPLAYED = BFALSE;
							THIS->DEPLOYED  = BFALSE;
							THIS->BLOCKED = BTRUE;
						}
					}
				}
			}
			
			VOID INTERNAL_DRAW(){
				BOX B0X;
				B0X.DESTINY = THIS->DESTINY;
				B0X.R = THIS->R;
				B0X.BORDER = BTRUE;
				B0X.MC = THIS->MC;
				B0X.BC  = THIS->BC;
				B0X.DRAW();
				
				THIS->DL.DESTINY = THIS->DESTINY;
				THIS->DL.ALIGN = BASGUI_RIGHT;
				THIS->DL.R = THIS->R;
				THIS->DL.DRAW();
				
				IF(THIS->DISPLAYED){
					B0X.R = THIS->DR;
					B0X.DRAW();
					
					THIS->L.DESTINY = THIS->DESTINY;
					THIS->L.ALIGN = BASGUI_RIGHT;
					
					FOR(ATYPE I=0; I<GET_SIZE(THIS->ITEM); I++){
						VEC2 POS;
						POS = GET_POS_I(THIS->R);
						POS.X = POS.X + 1;
						POS.Y = POS.Y + (I * 8);
						
						ATYPE INDEX;
						IF( FIND(THIS->NOT_SELECTABLE, I, INDEX) ){
							SET_POS(THIS->NL.R, POS);
							THIS->NL.DESTINY = THIS->DESTINY;
							THIS->NL.ALIGN   = BASGUI_RIGHT;
							SET(THIS->NL, THIS->ITEM[I]);
							THIS->NL.DRAW();
							CONTINUE;
						}
						IF(THIS->FI == I){
							SET_POS(THIS->FL.R, POS);
							THIS->FL.DESTINY = THIS->DESTINY;
							THIS->FL.ALIGN   = BASGUI_RIGHT;
							SET(THIS->FL, THIS->ITEM[I]);
							THIS->FL.DRAW();
						}
						ELSE{
							SET_POS(THIS->L.R, POS);
							THIS->L.DESTINY = THIS->DESTINY;
							THIS->L.ALIGN   = BASGUI_RIGHT;
							SET(THIS->L, THIS->ITEM[I]);
							THIS->L.DRAW();
						}
					}
				}
			}
		
		PUBLIC:
			LABEL L;
			LABEL DL; // DESCRIPTION LABEL
			LABEL FL; // FOCUS ITEM LABEL (WHEN DISPLAYED)
			LABEL NL; // NO SELECTABLE ITEM LABEL
			COLOR MC; // MARGIN COLOR
			COLOR BC; // BACKGROUND COLOR
			
			DLIST(){
				THIS->VAR = NULL;
				THIS->BC = COLOR_DARKGRAY;
				THIS->MC = COLOR_GRAY;
				SET(THIS->DL, "CHOOSE OPTION");
				THIS->DL.C = COLOR_ORANGE;
				THIS->DISPLAYED = BFALSE;
				THIS->BLOCKED = BFALSE;
				THIS->FI = -1;
				THIS->FL.C = COLOR_GREEN;
				THIS->NL.C = COLOR_BLACK;
				THIS->CANDEPLOY = BTRUE;
			}
			
			FRIEND VOID SETL(DLIST &L, LABEL LL){
				L.L = LL;
			}
			
			FRIEND VOID SETDL(DLIST &L, LABEL DL){
				L.DL = DL;
			}
			
			FRIEND VOID SETNL(DLIST &L, LABEL NL){
				L.NL = NL;
			}
			
			FRIEND VOID SETFL(DLIST &L, LABEL FL){
				L.FL = FL;
			}
			
			FRIEND VOID SETMC(DLIST &L, COLOR MC){
				L.MC = MC;
			}
			
			FRIEND VOID SETBC(DLIST &L, COLOR BC){
				L.BC = BC;
			}
			
			FRIEND VOID SETF(DLIST &L, VOID (*F)()){
				L.F = F;
			}
			
			FRIEND VOID SET(DLIST &L, ATYPE &VAR){
				L.VAR = &VAR;
				*(L.VAR) = 0;
			}
			
			FRIEND VOID SETPOS(DLIST &L, VEC2 POS){
				SET_POS(L.R, POS);
				UPDATE(L);
			}
			
			FRIEND VOID PUT(DLIST &L, EXT<S8> TEXT, BOOL SELECTABLE = BTRUE){
				PUSH(L.ITEM, TEXT);
				UPDATE(L);
				IF(!SELECTABLE){
					PUSH(L.NOT_SELECTABLE, GET_SIZE(L.ITEM) - 1);
				}
			}
			
			FRIEND VOID PUT(DLIST &L, CONST CHAR* TEXT, BOOL SELECTABLE = BTRUE){
				EXT<S8> E;
				E = TEXT;
				PUT(L, E, SELECTABLE);
			}
	};
	
	STRUCT GRAPHIC : PUBLIC ENTITY{
		PRIVATE:
			EXT<ATYPE> POINT;
			
			FRIEND VOID UPDATE(GRAPHIC &G){
				
			}
			
			VOID INTERNAL_LOGIC(){
				IF( !MOUSE_L ){
					IF( INSIDE2(THIS->R, V2_MOUSE) ){
						THIS->DO_F();
					}
				}
			}
			
			VOID INTERNAL_DRAW(){
				BOX B0X;
				B0X.DESTINY = THIS->DESTINY;
				B0X.R = THIS->R;
				B0X.BC = THIS->BC;
				B0X.DRAW();
				
				FOR(ATYPE I=0; I<GET_SIZE(THIS->POINT); I++){
					IF(THIS->POINT[I] == 0) CONTINUE;
					VEC2 II, F, SIZE; GET_3POS(THIS->R, II, F, SIZE);
					line(THIS->DESTINY, II.X + I, F.Y, II.X + I, F.Y - THIS->POINT[I], THIS->PC);
				}
			}
		
		PUBLIC:
			COLOR PC; // POINT COLOR
			COLOR BC; // BACKGROUND COLOR
			
			GRAPHIC(){
				THIS->PC = COLOR_BLUE;
				THIS->BC = COLOR_DARKGRAY;
			}
			
			
			FRIEND VOID SETPC(GRAPHIC &G, COLOR PC){
				G.PC = PC;
			}
			
			FRIEND VOID SETBC(GRAPHIC &G, COLOR BC){
				G.BC = BC;
			}
			
			FRIEND VOID SETPOS(GRAPHIC &G, VEC2 POS){
				SET_POS(G.R, POS);
			}
			
			FRIEND VOID SETSIZE(GRAPHIC &G, VEC2 SIZE){
				SET_SIZE(G.R, SIZE);
				RESIZE(G.POINT, SIZE.X);
				FILL(G.POINT, 0);
			}
			
			FRIEND VOID SETF(GRAPHIC &G, VOID (*F)()){
				G.F = F;
			}
			
			FRIEND VOID PUT(GRAPHIC &G, ATYPE POINT){
				INSERT(G.POINT, 0, POINT);
				POP(G.POINT);
			}
	};
	
	/*STRUCT CARTESIAN : PUBLIC ENTITY{
		NDSJKSD
	};*/
	
	STRUCT FIELD : PUBLIC ENTITY{
		PRIVATE:
			EXT<ENTITY*> ENTITIE;
			BITMAP* BMP;
			RECT GLOBAL;  // GLOBAL AREA THAT CONTAIN ALL ENTITIES
			RECT RENDER;  // REAL AREA THAT FIELD USE IN SCREEN
			RECT FINAL;   // RENDER LESS SPACE USED BY SLIDERS
			RECT CAMERA;  // ALLOWS MOVE WITHIN THE GLOBAL AREA
			
			SLIDER  HS; // HORIZONTAL SLIDER
			VSLIDER VS; // VERTCAL    SLIDER
			
			ATYPE HSVAR;
			ATYPE VSVAR;
			
			ATYPE BMP_INDEX;
			
			FRIEND VOID UPDATE(FIELD &F){
				
			}
			
			VOID INTERNAL_LOGIC(){
				
				// UPDATE GLOBAL SIZE
				FOR(ATYPE I=0; I<GET_SIZE(THIS->ENTITIE); I++){
					ENTITY* E;
					E = THIS->ENTITIE[I];
					
					VEC2 EPOS, GPOS, NEWSIZE;
					EPOS = GET_POS_F(E->R);
					GPOS = GET_POS_F(THIS->GLOBAL);
					
					NEWSIZE.X = MAX(EPOS.X, GPOS.X);
					NEWSIZE.Y = MAX(EPOS.Y, GPOS.Y);
					
					SET_SIZE(THIS->GLOBAL, NEWSIZE);
					
					LOG("NEWSIZE", NEWSIZE);
				}
				
				VEC2 GPOS, FPOS;
				GPOS = GET_POS_F(THIS->GLOBAL);
				FPOS = GET_SIZE (THIS->FINAL );
				
				IF(GPOS.X > FPOS.X AND !(THIS->HS.VISIBLE)){
					LOG("ENTRE AQUI SOLO 1 VEZ");
					THIS->HS.VISIBLE = BTRUE;
					SET(THIS->HS, THIS->HSVAR);
					
					VEC2 SIZE;
					SIZE = GET_SIZE(THIS->FINAL);
					SET_SIZE(THIS->FINAL, VEC2(SIZE.X, SIZE.Y - 10));
					SET_SIZE(THIS->CAMERA, SIZE);
					
					SET_POS(THIS->HS.R, VEC2(0, SIZE.Y));
					SET_SIZE(THIS->HS.R, VEC2(SIZE.X, 10));
					SET(THIS->HS, 0, GET_SIZE(THIS->GLOBAL).X - GET_SIZE(THIS->CAMERA).X);
				}
				ELSE IF(GPOS.X <= FPOS.X AND THIS->HS.VISIBLE){
					THIS->HS.VISIBLE = BFALSE;
					
					VEC2 SIZE;
					SIZE = GET_SIZE(THIS->FINAL);
					SET_SIZE(THIS->FINAL, VEC2(SIZE.X, SIZE.Y + 10));
					SET_SIZE(THIS->CAMERA, SIZE);
				}
				
				IF(GPOS.Y > FPOS.Y AND !(THIS->VS.VISIBLE)){
					//LOG("NO DEBO ESTAR AQUI...");
					THIS->VS.VISIBLE = BTRUE;
					SET(THIS->VS, THIS->VSVAR);
					
					VEC2 SIZE;
					SIZE = GET_SIZE(THIS->FINAL);
					SET_SIZE(THIS->FINAL, VEC2(SIZE.X - 10, SIZE.Y));
					SET_SIZE(THIS->CAMERA, SIZE);
					
					SET_POS(THIS->VS.R, VEC2(SIZE.X, 0));
					SET_SIZE(THIS->VS.R, VEC2(10, SIZE.Y));
					SET(THIS->VS, 0, GET_SIZE(THIS->GLOBAL).Y - GET_SIZE(THIS->CAMERA).Y);
				}
				ELSE IF(GPOS.Y <= FPOS.Y AND THIS->VS.VISIBLE){
					THIS->VS.VISIBLE = BFALSE;
					
					VEC2 SIZE;
					SIZE = GET_SIZE(THIS->FINAL);
					SET_SIZE(THIS->FINAL, VEC2(SIZE.X + 10, SIZE.Y));
					SET_SIZE(THIS->CAMERA, SIZE);
				}
				
				IF(THIS->HS.VISIBLE){
					LOG("HS.LOGIC()");
					VEC2 POS, TEMP_POS;
					POS = GET_POS_I(THIS->HS.R);
					TEMP_POS = POS + GET_POS_I(THIS->R);
					SET_POS(THIS->HS.R, TEMP_POS);
					THIS->HS.LOGIC();
					SET_POS(THIS->HS.R, POS);
					
					SET_POS(THIS->CAMERA, VEC2(THIS->HSVAR, GET_POS_I(THIS->CAMERA).Y));
				}
				
				IF(THIS->VS.VISIBLE){
					VEC2 POS, TEMP_POS;
					POS = GET_POS_I(THIS->VS.R);
					TEMP_POS = POS + GET_POS_I(THIS->R);
					SET_POS(THIS->VS.R, TEMP_POS);
					THIS->VS.LOGIC();
					SET_POS(THIS->VS.R, POS);
					
					SET_POS(THIS->CAMERA, VEC2(GET_POS_I(THIS->CAMERA).X, THIS->VSVAR));
				}
				
				FOR(ATYPE I=0; I<GET_SIZE(THIS->ENTITIE); I++){
					ENTITY* E;
					E = THIS->ENTITIE[I];
					E->LOGIC();
					
					//IF(E->CANDEPLOY)
				}
				
				//LOG("------------------");
				//LOG("CAMERA.POS", GET_POS_I(THIS->CAMERA));
				//LOG("------------------");
			}
			
			VOID INTERNAL_DRAW(){
				clear(THIS->BMP);
				
				VEC2 S;
				S = GET_SIZE(THIS->RENDER);
				rectfill(THIS->BMP, 0, 0, S.X, S.Y, COLOR_GREEN); // DEBUG
				S = GET_SIZE(THIS->FINAL);
				rectfill(THIS->BMP, 0, 0, S.X, S.Y, MAKECOLOR(20, 20, 20)); // DEBUG
				
				IF(THIS->HS.VISIBLE){
					LOG("HS.DRAW()");
					THIS->HS.DESTINY = THIS->BMP;
					THIS->HS.DRAW();
				}
				
				IF(THIS->VS.VISIBLE){
					THIS->VS.DESTINY = THIS->BMP;
					THIS->VS.DRAW();
				}
				
				IF(THIS->HS.VISIBLE AND THIS->VS.VISIBLE){
					BOX B0X;
					B0X.BC = COLOR_RED;
					//B0X.BC = COLOR_GRAY; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					SET_POS(B0X.R, GET_SIZE(THIS->FINAL));
					SET_SIZE(B0X.R, VEC2(10, 10));
					B0X.DESTINY = THIS->BMP;
					B0X.DRAW();
				}
				
				FOR(ATYPE I=0; I<GET_SIZE(THIS->ENTITIE); I++){
					ENTITY* E;
					E = THIS->ENTITIE[I];
					//IF( EDGE_HIT(E->R, THIS->CAMERA) OR INSIDE2(THIS->CAMERA, E->R)){
						VEC2 POS, TEMP_POS;
						POS = GET_POS_I(E->R);
						TEMP_POS = POS - GET_POS_I(THIS->CAMERA);
						SET_POS(E->R, TEMP_POS);
						E->DRAW();
						//LOG("TEMP_POS", TEMP_POS);
						SET_POS(E->R, POS);
					//}
				}
				
				VEC2 POS;
				POS = GET_POS_I(THIS->RENDER);
				draw_sprite(THIS->DESTINY, THIS->BMP, POS.X, POS.Y);
			}
		
		PUBLIC:
			
			FIELD(){
				THIS->BMP = NULL;
				THIS->HS.VISIBLE = BFALSE;
				THIS->VS.VISIBLE = BFALSE;
				THIS->BMP_INDEX  = -1;
			}
			
			FRIEND VOID SETF(FIELD &F, VOID (*FF)()){
				F.F = FF;
			}
			
			FRIEND VOID SETPOS(FIELD &F, VEC2 POS){
				SET_POS(F.R     , POS);
				SET_POS(F.RENDER, POS);
				SET_POS(F.FINAL , POS);
			}
			
			FRIEND VOID SETSIZE(FIELD &F, VEC2 SIZE){
				SET_SIZE(F.GLOBAL, SIZE);
				SET_SIZE(F.FINAL , SIZE);
				SET_SIZE(F.RENDER, SIZE);
				SET_SIZE(F.CAMERA, SIZE);
				SET_SIZE(F.R     , SIZE);
				
				SET_POS(F.CAMERA, V2_ZERO);
				SET_POS(F.GLOBAL, V2_ZERO);
				
				IF(F.BMP_INDEX != -1){
					FREE_BMP(F.BMP_INDEX);
				}
				CREATE_BMP(F.BMP, SIZE);
				clear(F.BMP);
			}
			
			FRIEND VOID PUT(FIELD &F, ENTITY* E){
				E->DESTINY = F.BMP;
				PUSH(F.ENTITIE, E);
			}
	};
}
