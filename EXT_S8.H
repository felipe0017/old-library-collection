// TEMPLATE SPECIALIZATION OF EXTENDER OBJECT FOR S8 TYPE
// CAN REPRESENT ASCII TEXT STRINGS

// IN TEXT SUBROUTINES, D MUST BE DIFFERENT THAN E

S8 HEXA_TABLE[] = {'0','1','2','3',
                   '4','5','6','7',
				   '8','9','A','B',
				   'C','D','E','F',};

TEMPLATE <>
STRUCT EXT <S8> {
	PRIVATE:
		ATYPE SIZE;
		ATYPE CAPACITY;
		S8* POINTER;
		
		// RESERVE DINAMIC MEMORY FOR NEW EXTENSOR
		FRIEND VOID RESERVE(EXT<S8> &E, ATYPE CAPACITY){
			IF(CAPACITY < 0) TEST(0);
			E.POINTER = NEW S8[CAPACITY];
			IF( !E.POINTER ) TEST(0);
		}
		
		BOOL IAR; // IS A REFERENCE
		
	PUBLIC:
		
		EXT(){
			THIS->SIZE = 0;
			THIS->CAPACITY = 0;
			RESERVE(*THIS, THIS->CAPACITY);
			THIS->IAR = BFALSE;
		}
		
		EXT(ATYPE CAPACITY){
			THIS->SIZE = 0;
			THIS->CAPACITY = CAPACITY;
			RESERVE(*THIS, THIS->CAPACITY);
			THIS->IAR = BFALSE;
		}
		
		~EXT(){
			IF( !THIS->IAR )
				DELETE[] THIS->POINTER;
		}
		
		EXT<S8>& OPERATOR=(EXT<S8> E){
			FREE(*THIS);
			COPY(*THIS, E);
			RETURN *THIS;
		}
		
		S8& OPERATOR[](ATYPE INDEX){
			IF( !INSIDE(*THIS, INDEX) ){
				TEST(0);
			}
			RETURN *(THIS->POINTER + INDEX);
		}
		
		FRIEND BOOL ASSIGN(EXT<S8> &E, ATYPE INDEX, S8 DATA){
			IF( INSIDE(E, INDEX)){
				E[INDEX] = DATA;
				RETURN BTRUE;
			}
			RETURN BFALSE;
		}
		
		FRIEND BOOL READ(EXT<S8> &E, ATYPE INDEX, S8 &DATA){
			IF( INSIDE(E, INDEX) ){
				DATA = E[INDEX];
				RETURN BTRUE;
			}
			RETURN BFALSE;
		}
		
		FRIEND BOOL INSIDE(EXT<S8> &E, ATYPE INDEX){
			RETURN (INDEX >= 0 AND INDEX < E.SIZE);
		}
		
		FRIEND VOID FILL(EXT<S8> &E, S8 DATA){
			FOR(ATYPE I=0; I<E.SIZE; I++)
			    E[I] = DATA;
		}
		
		FRIEND BOOL EMPTY(EXT<S8> &E){
			RETURN !E.SIZE;
		}

		FRIEND VOID PUSH(EXT<S8> &E, S8 DATA){
			IF( E.SIZE == E.CAPACITY ){
				IF( E.CAPACITY == EXT_MAX ){
					TEST(0);
				}
				
				// NUEVA CAPACIDAD DE EXTENSOR.
				// EN ESTE CASO, EL DOBLE DE SU CAPACIDAD ANTERIOR.
				ATYPE NEW_CAPACITY;
				NEW_CAPACITY = MIN(MAX(E.CAPACITY * 2, 1), EXT_MAX);

				S8* OLD_DATA;
				OLD_DATA = E.POINTER;
				E.CAPACITY = NEW_CAPACITY;
				RESERVE(E, NEW_CAPACITY);

				// TRANSFER DATA
				FOR(ATYPE I=0; I<E.SIZE; I++)
					E[I] = *(OLD_DATA + I);

				// FREE OLD MEMORY
				DELETE[] OLD_DATA;
			}
			// STORE NEW DATA
			E.SIZE++;
			E[E.SIZE-1] = DATA;
		}
		
		FRIEND VOID INSERT(EXT<S8> &E, ATYPE INDEX, S8 DATA){
		    IF( !INSIDE(E, INDEX) ) {
		        TEST(0);
		    }
		    S8 LAST_DATA;
		    LAST_DATA = LAST(E);
		    FOR(ATYPE I=0; I<E.SIZE - INDEX - 1; I++){
		    	ATYPE II;
		    	II = E.SIZE - I - 2;
		    	E[II+1] = E[II];
			}
		    E[INDEX] = DATA;
		    PUSH(E, LAST_DATA);
		}
		
		FRIEND S8& LAST(EXT<S8> &E){
		    RETURN E[E.SIZE - 1];
		}
		
		FRIEND S8& FIRST(EXT<S8> &E){
		    RETURN E[0];
		}
		
		FRIEND VOID RESIZE(EXT<S8> &E, ATYPE NEW_SIZE){
			// FREE DATA
			DELETE[] E.POINTER;
			
			E.SIZE = NEW_SIZE;
			E.CAPACITY = NEW_SIZE;
			RESERVE(E, NEW_SIZE);
		}
		
		FRIEND VOID ERASE(EXT<S8> &E, ATYPE INDEX){
			IF( !INSIDE(E, INDEX) ) {
				TEST(0);
			}
			
			FOR(ATYPE I=INDEX+1; I<E.SIZE; I++)
	            E[I-1] = E[I];
			E.SIZE--;
		}
		
		FRIEND VOID POP(EXT<S8> &E){
			ERASE(E, E.SIZE - 1);
		}
		
		// FREE UNUSED MEMORY
		FRIEND VOID FIT(EXT<S8> &E){
			S8* OLD_DATA;
			OLD_DATA = E.POINTER;
			E.CAPACITY = E.SIZE;
			RESERVE(E, E.SIZE);

			// TRANSFER DATA
			FOR(ATYPE I=0; I<E.SIZE; I++)
				E[I] = *(OLD_DATA + I);

			// FREE OLD MEMORY
			DELETE[] OLD_DATA;
		}
		
		// FREE ALL MEMORY
		FRIEND VOID FREE(EXT<S8> &E){
			CLEAR(E);
			FIT(E);
		}

		FRIEND BOOL FIND(EXT<S8> &E, S8 DATA, ATYPE &INDEX){
			FOR(ATYPE I=0; I<E.SIZE; I++){
				IF(E[I] == DATA){
					INDEX = I;
					RETURN BTRUE;
				}
			}
			RETURN BFALSE;
		}
		
		FRIEND VOID COPY(EXT<S8> &D, EXT<S8> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++){
				PUSH(D, E[I]);
			}
		}
		
		FRIEND BOOL ISEQUAL(EXT<S8> &E, EXT<S8> &E1){
			ATYPE S,S1;
			S  = E.SIZE ;
			S1 = E1.SIZE;
			IF(S != S1) RETURN BFALSE;
			
			FOR(ATYPE I=0; I<S; I++)
				IF( E[I] != E1[I] ) RETURN BFALSE;
			RETURN BTRUE;
		}
		
		FRIEND VOID REFERENCE(EXT<S8> &E, EXT<S8> &R){
			E.CAPACITY     = R.CAPACITY;
			E.POINTER      = R.POINTER;
			E.SIZE         = R.SIZE;
			E.IAR          = BTRUE;
		}
		
		FRIEND STD::OSTREAM& OPERATOR<<(STD::OSTREAM& OS, EXT<S8> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++){
				OS << E[I];
			}
			RETURN OS;
		}
		
		FRIEND VOID CLEAR(EXT<S8> &E){
			E.SIZE = 0;
		}

		FRIEND ATYPE GET_CAPACITY(EXT<S8> &E){
			RETURN E.CAPACITY;
		}
		
		FRIEND ATYPE GET_MAX_CAPACITY(EXT<S8> &E){
			RETURN EXT_MAX;
		}
		
		FRIEND S8* GET_DATA(EXT<S8> &E){
			RETURN E.POINTER;
		}
		
		FRIEND ATYPE GET_SIZE(EXT<S8> &E){
			RETURN E.SIZE;
		}
		
		EXT<S8>& OPERATOR=(CONST CHAR* TEXT){
			FREE(*THIS);
			ATYPE I = 0;
			WHILE(1){
				S8 SIMBOL;
				SIMBOL = *(TEXT + I);
				IF(SIMBOL == 0) break;
				PUSH(*THIS, SIMBOL);
				I++;
			}
		}
		
		FRIEND VOID UPPERCASE(EXT<S8> &D, EXT<S8> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++)
				IF(E[I] >= 'a' AND E[I] <= 'z')
					PUSH(D, 'A' + (E[I] - 'a'));
				ELSE PUSH(D, E[I]);
		}
		
		FRIEND VOID LOWERCASE(EXT<S8> &D, EXT<S8> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++)
				IF(E[I] >= 'A' AND E[I] <= 'Z')
					PUSH(D, 'a' + (E[I] - 'A'));
				ELSE PUSH(D, E[I]);
		}
		
		FRIEND VOID BASE10_POINTS(EXT<S8> &D, EXT<S8> &E){
			U8 COUNTER = 0;
			FOR(ATYPE I=0; I<E.SIZE; I++){
				ATYPE II;
				II = E.SIZE - I - 1;
				IF(COUNTER == 3){
					COUNTER = 0;
					PUSH(D, '.');
				}
				PUSH(D, E[II]);
				COUNTER++;
			}
		}
		
		FRIEND BOOL IS_A_NUMBER(EXT<S8> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++){
				IF(I==0 AND E[I] == '-' AND E.SIZE != 1) CONTINUE;
				IF(!(E[I] >= '0' AND E[I] <= '9')) RETURN BFALSE;
			}
			RETURN BTRUE;
		}
		
		FRIEND BOOL CONTAIN(EXT<S8> &E, EXT<S8> &WORD){
			FOR(ATYPE I=0; I<E.SIZE; I++){
				IF(E[I] == WORD[I]){
					BOOL CONTAINED = BTRUE;
					FOR(ATYPE J=1; J<WORD.SIZE; J++){
						IF(E[I+J] != WORD[J]) {CONTAINED = BFALSE; break;}
					}
					IF(CONTAINED) RETURN BTRUE;
				}
			}
			RETURN BFALSE;
		}
		
		FRIEND VOID DELETE_SIMBOL(EXT<S8> &D, EXT<S8> &E, S8 SIMBOL){
			FOR(ATYPE I=0; I<E.SIZE; I++){
				IF(E[I] == SIMBOL) CONTINUE;
				PUSH(D, E[I]);
			}
		}
		
		FRIEND VOID DELETE_SPACES(EXT<S8> &D, EXT<S8> &E){
			DELETE_SIMBOL(D, E, ' ');
		}
		
		// DELETE INSIGNIFICATIVE ZEROS OF A NUMBER
		FRIEND VOID DELETE_USELESS_ZEROS(EXT<S8> &D, EXT<S8> &E){
			BOOL USELESS = BTRUE;
			FOR(ATYPE I=0; I<E.SIZE; I++){
				IF(E[I] != '0') USELESS = BFALSE;
				IF(!USELESS) PUSH(D, E[I]);
			}
		}
		
		// EX, 7 TO 3 DIGITS -> 007
		FRIEND VOID FILL_ZEROS(EXT<S8> &D, EXT<S8> &E, ATYPE MAX_DIGITS){
			ATYPE ZEROS;
			ZEROS = MAX_DIGITS - E.SIZE;
			FOR(ATYPE I=0; I<ZEROS; I++) PUSH(D, '0');
			COPY(D, E);
		}
		
		FRIEND ATYPE COUNT_SIMBOL(EXT<S8> &E, S8 SIMBOL){
			ATYPE C = 0;
			FOR(ATYPE I=0; I<E.SIZE; I++)
				IF(E[I] == SIMBOL) C++;
			RETURN C;
		}
		
		FRIEND VOID FLIP(EXT<S8> &D, EXT<S8> &E){
			ATYPE SIZE = E.SIZE;
			FOR(ATYPE I=0; I<SIZE; I++)
				PUSH(D, E[SIZE - I - 1]);
		}
		
		FRIEND VOID CONCATENATE(EXT<S8> &D, EXT<S8> &E, EXT<S8> &E1){
			COPY(D, E);
			COPY(D, E1);
		}
		
		FRIEND BOOL ALPHA(EXT<S8> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++)
				IF(!(E[I]==' ' OR (E[I]>='A' AND E[I]<='Z') OR (E[I]>='a' AND E[I]<='z')))
					RETURN BFALSE;
			RETURN BTRUE;
		}
		
		FRIEND BOOL ALPHANUMERIC(EXT<S8> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++)
				IF(!(E[I]==' ' OR (E[I]>='A' AND E[I]<='Z') OR (E[I]>='a' AND E[I]<='z') OR (E[I]>='0' AND E[I]<='9')))
					RETURN BFALSE;
			RETURN BTRUE;
		}
		
		FRIEND BOOL PRINTABLE(EXT<S8> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++)
				IF(E[I] >= 0 AND E[I] <= 31) RETURN BFALSE;
			RETURN BTRUE;
		}
		
		FRIEND BOOL CONTROL(EXT<S8> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++)
				IF(!(E[I] >= 0 AND E[I] <= 31)) RETURN BFALSE;
			RETURN BTRUE;
		}
		
		FRIEND BOOL SPACE(EXT<S8> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++)
				IF(E[I] != ' ') RETURN BFALSE;
			RETURN BTRUE;
		}
		
		FRIEND VOID SEPARATE_WORDS(EXT<VEC2> &D, EXT<S8> &E){
			BOOL IN_WORD;
			VEC2 WORD;
			IN_WORD = BFALSE;
			FOR(ATYPE I=0; I<E.SIZE; I++){
				IF(E[I] != ' ' AND !IN_WORD){
					IN_WORD = BTRUE;
					WORD.X = I; // START OF WORD
					CONTINUE;
				}
				IF(E[I] == ' ' AND IN_WORD){
					IN_WORD = BFALSE;
					WORD.Y = I - 1; // END OF WORD
					PUSH(D, WORD);
				}
			}
		}
		
		// INCLUDE FINAL INDEX
		FRIEND VOID SUB_ORATION(EXT<S8> &D, EXT<S8> &E, ATYPE I, ATYPE F){
			FOR(ATYPE II=I; II<=F; II++)
				PUSH(D, E[II]);
		}
		
		FRIEND std::string STD_STRING(EXT<S8> &E){
			std::string S;
			FOR(ATYPE I=0; I<E.SIZE; I++){
				S += E[I];
			}
			RETURN S;
		}
		
		FRIEND CONST CHAR* CONST_CHAR_P(EXT<S8> &E){
			std::string S;
			S = STD_STRING(E);
			RETURN S.c_str();
		}
		
		FRIEND VOID BIN(EXT<S8> &D, ATYPE N){
		    D = "BIN";
		    U8 BYTE_COUNTER=0; // BYTE SEPARATOR
		    FOR(U8 I=0; I<64; I++){
		        BOOL DIGIT;
		        U8 II;
		        II = 64 - I - 1;
		        DIGIT = (N >> II) & 0X1;
		        PUSH(D, HEXA_TABLE[DIGIT]);
		        IF(BYTE_COUNTER == 7 AND I != 64) { PUSH(D, ' '); BYTE_COUNTER=0; }
		        ELSE BYTE_COUNTER++;
		    }
		}
		
		FRIEND VOID OCTAL(EXT<S8> &D, ATYPE N){
		    D = "OC";
		    IF(N == 0) { D = "OC0"; RETURN;     }
		    IF(N <  0) { D = "-OC"; N = ABS(N); }
		    BOOL USELESS = BTRUE;
		    FOR(U8 I=0; I<64; I+=3){
		        U8 DIGIT;
		        U8 II;
		        II = 64 - I - 1;
		        DIGIT = (N >> II) & 0X7;
		        IF(DIGIT != 0) USELESS = BFALSE;
		        IF(!USELESS) PUSH(D, HEXA_TABLE[DIGIT]);
		    }
		}
		
		FRIEND VOID DECIMAL(EXT<S8> &D, ATYPE N){
		    IF(N == 0) { D = "0"; RETURN;          }
		    IF(N <  0) { PUSH(D, '-'); N = ABS(N); }
		
		    //GET DECIMAL LENGHT
		    U8 LENGHT;
		    LENGHT = DECIMAL_LENGTH(N);
		
		    // GET EVERY DIGIT
		    ATYPE REST;
		    FOR(U8 I=0; I<LENGHT; I++){
		        ATYPE DIGIT;
		        ATYPE FACTOR;
		        FACTOR = POW10(LENGHT - I);
		        DMOD(N, FACTOR, DIGIT, REST);
		        PUSH(D, HEXA_TABLE[DIGIT]);
		        N = REST;
		    }
		}
		
		FRIEND VOID HEXA(EXT<S8> &D, ATYPE N){
			D = "0X";
			IF(N == 0) { D = "0X0"; RETURN;     }
		    IF(N <  0) { D = "-0X"; N = ABS(N); }
		    BOOL USELESS = BTRUE;
			FOR(U8 I=3; I<64; I+=4){
				U8 DIGIT;
		        U8 II;
		        II = 64 - I - 1;
				DIGIT = (N >> I) & 0x15;
				IF(DIGIT != 0) USELESS = BFALSE;
		        IF(!USELESS) PUSH(D, HEXA_TABLE[DIGIT]);
			}
		}
		
		FRIEND ATYPE NUMBER(EXT<S8> &E){
			IF( !IS_A_NUMBER(E) ) RETURN 0;
			ATYPE NUMBER;
			BOOL S=0; // SIGN
			FOR(ATYPE I=0; I<E.SIZE; I++){
				IF(I==0 AND E[I] == '-') { S=1; CONTINUE; }
				NUMBER += ( (E[I] - '0') * POW10(I) );
			}
			NUMBER = SETSIGN(NUMBER, S);
			RETURN NUMBER;
		}
};
