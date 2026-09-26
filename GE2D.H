// 2D GRID LAYER DRAWABLE

namespace GE2D{
	
	// GE2D GENERAL ATRIBUTES
	bool DEBUG = false;
	void DEBUGMODE(bool _DEBUG){
		DEBUG = _DEBUG;
	}
	
	struct CAMERA{
		private:
			VEC2 RENDER_ZONE; // HORIZONTAL & VERTICAL BLOCKS
			
		public:
			VEC2 POS;// LEFT UP CORNER
			BASGUI::CANVAS* C;
			
			CAMERA(){
				this->RENDER_ZONE = MAKEV2(4);
			}
			friend VEC2 GET_RENDERZONE(CAMERA C);
			friend void SET(CAMERA &C, VEC2 RENDER_SIZE_ZONE);
			friend void TP(CAMERA &C, VEC2 NEW_POS);
	};
	
	VEC2 GET_RENDERZONE(CAMERA C){
		return C.RENDER_ZONE;
	}
	
	void SET(CAMERA &C, VEC2 RENDER_SIZE_ZONE){
		C.RENDER_ZONE = ABS(RENDER_SIZE_ZONE);
	}
	
	void TP(CAMERA &C, VEC2 NEW_POS){
		C.POS = NEW_POS;
	}
	
	// CAPAS
	template <SSET_TYPE _W_, SSET_TYPE _H_>
	struct LAYER{
		private:
			struct BLOCK_TYPE{
				VEC2 TILE_POS;
				bool TEXTURED;
				BLOCK_TYPE(){TEXTURED = false;}
			};
			SSET<BLOCK_TYPE, 256> BLOCK_TYPES;
			// SPRITESHEET BLOCKS
			BITMAP* BMP;
			bool TEXTURED;
			// SIZE OF LAYER (IN BLOCKS)
			VEC2 SIZE;
			// SIZE OF LAYER (IN PIXELS)
			VEC2 SIZE_PX;
			// BLOCK SIZE OF LAYER (IN PIXELS)
			VEC2 BLOCK_SIZE;
			
		public:
			//SSET<SSET<U8, _W_>, _H_> DATA;
			MAT2<U8, _W_, _H_> DATA;
			
			LAYER(){
				this->TEXTURED = false;
				this->SIZE = MAKEV2(_W_, _H_);
				FILL(this->DATA, ' ');
			}
			
			friend void SET_BLOCKSIZE(LAYER<_W_,_H_> &L, VEC2 BLOCK_SIZE){
				L.BLOCK_SIZE = ABS(BLOCK_SIZE);
				L.SIZE_PX = MUL(L.SIZE, L.BLOCK_SIZE);
			}

			friend void SET_TEXTURE(LAYER<_W_,_H_> &L, BITMAP* BMP){
				if(BMP != NULL){
					L.BMP = BMP;
					L.TEXTURED = true;
				}
			}
			
			friend void SET_BLOCK_TYPE(LAYER<_W_,_H_> &L, U8 BLOCK_ID, VEC2 INITIAL_TILE_POS, bool TEXTURED){
				L.BLOCK_TYPES[BLOCK_ID].TILE_POS = INITIAL_TILE_POS;
				L.BLOCK_TYPES[BLOCK_ID].TEXTURED = TEXTURED;
			}
			
			friend void DRAW(CAMERA &C, LAYER<_W_,_H_> &L){
				VEC2 RENDER_ZONE;
				RENDER_ZONE = GET_RENDERZONE(C);
				
				VEC2 I = DIV(C.POS, L.BLOCK_SIZE);
				
				for(U16 X=0; X<RENDER_ZONE.X+1; X++){      // eje X
					for(U16 Y=0; Y<RENDER_ZONE.Y+1; Y++){  // eje Y
						
						// TEST HAVE THE POSITION OF THE ACTUAL BLOCK
						VEC2 TEST = MAKEV2(X,Y);
						TEST = ADD(I, TEST);
						
						// ID OF THE ACTUAL BLOCK
						if( !IS_INSIDE(L.DATA, TEST) ) continue;
						U8 BLOCK_ID;
						READ(L.DATA, TEST, BLOCK_ID);
						
						// COORDS TO DRAW BLOCK TEXTURE IN SCREEN
						VEC2 DRAW_POS;
						DRAW_POS = MUL(TEST, L.BLOCK_SIZE);
						DRAW_POS = SUB(DRAW_POS, C.POS);
						
						// DRAW TEXTURE
						if(L.TEXTURED){
							if(L.BLOCK_TYPES[BLOCK_ID].TEXTURED){
								// TEXTURE INITIAL COORDS (IN SPRITESHEET)
								VEC2 TEXTURE = L.BLOCK_TYPES[BLOCK_ID].TILE_POS;
								BLIT(*C.C, L.BMP, TEXTURE, DRAW_POS, L.BLOCK_SIZE);
							}
						}
						
						if(GE2D::DEBUG){
							// DEBUG-MODE
							// DRAW GRID EDGES
							RECT R;
							R = MAKERECTS(IADD(DRAW_POS, 1), ISUB(L.BLOCK_SIZE, 1));
							VRECT(*C.C, R, _WHITE_);
						}
					}
				}
			}
	};
}

/*
void GE2D::DRAW_ISOMETRIC(OBJ_CAMERA &C, OBJ_LAYER L){
	
	// ABREVIATION OF "L.BLOCK_SIZE"
	static VEC2 BS = L.BLOCK_SIZE;
	
	// HALF BLOCK
	static VEC2 HB = IDIV(BS, 2);
	
	// INITIAL POSITION IN Y-AXIS (OF X=0) FOR EACH 'Y' ITERATOR
	VEC2 INITIAL;
	INITIAL = MAKEV2(WINDOW::WIDTH/2-HB.X, 0); // DEBUG
	
	// ISOMETRIC RENDER
	for(uint8_t Y=0; Y<L.SIZE.Y; Y++){      // eje Y
		for(uint8_t X=0; X<L.SIZE.X; X++){  // eje X
			
			// DRAW_POS IS THE VEC2 TO DRAW TEXTURE
			VEC2 DRAW_POS;
			DRAW_POS.X = INITIAL.X + HB.X * X;
			DRAW_POS.Y = INITIAL.Y + HB.Y * X;
			
			
			if(GE2D::DEBUG){
				// DEBUG-MODE
				// DRAW GRID EDGES
				VEC2 POINTS[4];
				POINTS[0] = MAKEV2(DRAW_POS.X+HB.X, DRAW_POS.Y);
				POINTS[1] = MAKEV2(DRAW_POS.X+BS.X, DRAW_POS.Y+HB.Y);
				POINTS[2] = MAKEV2(DRAW_POS.X+HB.X, DRAW_POS.Y+BS.Y);
				POINTS[3] = MAKEV2(DRAW_POS.X, DRAW_POS.Y + HB.Y);
				
				for(uint8_t i=0; i<4; i++){
					VEC2 INDEX;
					if(i==3) INDEX = MAKEV2(3, 0);
					else     INDEX = MAKEV2(i, i+1);
					
					al_draw_line(POINTS[INDEX.X].X, POINTS[INDEX.X].Y, POINTS[INDEX.Y].X, POINTS[INDEX.Y].Y, al_map_rgb(255,255,255), 0);
				}
			}
		}
		INITIAL.X -= HB.X;
		INITIAL.Y += HB.Y;
	}
}
*/
