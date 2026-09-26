// EXTENDER OBJECT
// DINAMIC SIZE ARRAY

// NOT RETURN EXTENSORS FOR SUBROUTINES

// THE MAX SIZE OF A EXTENSOR
#define EXT_MAX  S64_MAX

TEMPLATE <TYPENAME T>
STRUCT EXT{
	PRIVATE:
		ATYPE SIZE;
		ATYPE CAPACITY;
		T* POINTER;
		
		// RESERVE DINAMIC MEMORY FOR NEW EXTENSOR
		FRIEND VOID RESERVE(EXT<T> &E, ATYPE CAPACITY){
			IF(CAPACITY < 0) TEST(0);
			E.POINTER = NEW T[CAPACITY];
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
		
		EXT<T>& OPERATOR=(EXT<T> E){
			FREE(*THIS);
			COPY(*THIS, E);
			RETURN *THIS;
		}
		
		T& OPERATOR[](ATYPE INDEX){
			IF( !INSIDE(*THIS, INDEX) ){
				TEST(0);
			}
			RETURN *(THIS->POINTER + INDEX);
		}
		
		FRIEND BOOL ASSIGN(EXT<T> &E, ATYPE INDEX, T DATA){
			IF( INSIDE(E, INDEX)){
				E[INDEX] = DATA;
				RETURN BTRUE;
			}
			RETURN BFALSE;
		}

		FRIEND BOOL READ(EXT<T> &E, ATYPE INDEX, T &DATA){
			IF( INSIDE(E, INDEX) ){
				DATA = E[INDEX];
				RETURN BTRUE;
			}
			RETURN BFALSE;
		}
		
		FRIEND BOOL INSIDE(EXT<T> &E, ATYPE INDEX){
			RETURN (INDEX >= 0 AND INDEX < E.SIZE);
		}
		
		FRIEND VOID FILL(EXT<T> &E, T DATA){
			FOR(ATYPE I=0; I<E.SIZE; I++)
			    E[I] = DATA;
		}
		
		FRIEND BOOL EMPTY(EXT<T> &E){
			RETURN !E.SIZE;
		}

		FRIEND VOID PUSH(EXT<T> &E, T DATA){
			IF( E.SIZE == E.CAPACITY ){
				IF( E.CAPACITY == EXT_MAX ){
					TEST(0);
				}
				
				// NUEVA CAPACIDAD DE EXTENSOR.
				// EN ESTE CASO, EL DOBLE DE SU CAPACIDAD ANTERIOR.
				ATYPE NEW_CAPACITY;
				NEW_CAPACITY = MIN(MAX(E.CAPACITY * 2, 1), EXT_MAX);

				T* OLD_DATA;
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
		
		FRIEND VOID INSERT(EXT<T> &E, ATYPE INDEX, T DATA){
		    IF( !INSIDE(E, INDEX) ) {
		        TEST(0);
		    }
		    T LAST_DATA;
		    LAST_DATA = LAST(E);
		    FOR(ATYPE I=0; I<E.SIZE - INDEX - 1; I++){
		    	ATYPE II;
		    	II = E.SIZE - I - 2;
		    	E[II+1] = E[II];
			}
		    E[INDEX] = DATA;
		    PUSH(E, LAST_DATA);
		}
		
		FRIEND T& LAST(EXT<T> &E){
		    RETURN E[E.SIZE - 1];
		}
		
		FRIEND T& FIRST(EXT<T> &E){
		    RETURN E[0];
		}
		
		FRIEND VOID RESIZE(EXT<T> &E, ATYPE NEW_SIZE){
			// FREE DATA
			DELETE[] E.POINTER;
			
			E.SIZE = NEW_SIZE;
			E.CAPACITY = NEW_SIZE;
			RESERVE(E, NEW_SIZE);
		}
		
		FRIEND VOID ERASE(EXT<T> &E, ATYPE INDEX){
			IF( !INSIDE(E, INDEX) ) {
				TEST(0);
			}
			
			FOR(ATYPE I=INDEX+1; I<E.SIZE; I++)
	            E[I-1] = E[I];
			E.SIZE--;
		}
		
		FRIEND VOID POP(EXT<T> &E){
			ERASE(E, E.SIZE - 1);
		}
		
		// FREE UNUSED MEMORY
		FRIEND VOID FIT(EXT<T> &E){
			T* OLD_DATA;
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
		FRIEND VOID FREE(EXT<T> &E){
			CLEAR(E);
			FIT(E);
		}

		FRIEND BOOL FIND(EXT<T> &E, T DATA, ATYPE &INDEX){
			FOR(ATYPE I=0; I<E.SIZE; I++){
				IF(E[I] == DATA){
					INDEX = I;
					RETURN BTRUE;
				}
			}
			RETURN BFALSE;
		}
		
		FRIEND VOID COPY(EXT<T> &D, EXT<T> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++){
				PUSH(D, E[I]);
			}
		}
		
		FRIEND BOOL ISEQUAL(EXT<T> &E, EXT<T> &E1){
			ATYPE S,S1;
			S  = E.SIZE ;
			S1 = E1.SIZE;
			IF(S != S1) RETURN BFALSE;
			
			FOR(ATYPE I=0; I<S; I++)
				IF( E[I] != E1[I] ) RETURN BFALSE;
			RETURN BTRUE;
		}
		
		FRIEND VOID REFERENCE(EXT<T> &E, EXT<T> &R){
			E.CAPACITY     = R.CAPACITY;
			E.POINTER      = R.POINTER;
			E.SIZE         = R.SIZE;
			E.IAR          = BTRUE;
		}
		
		FRIEND STD::OSTREAM& OPERATOR<<(STD::OSTREAM& OS, EXT<T> &E){
			FOR(ATYPE I=0; I<E.SIZE; I++){
				OS << "E[" << I << "]: " << E[I];
				IF(I < E.SIZE - 1) OS << STD::ENDL;
			}
			RETURN OS;
		}
		
		FRIEND VOID CLEAR(EXT<T> &E){
			E.SIZE = 0;
		}

		FRIEND ATYPE GET_CAPACITY(EXT<T> &E){
			RETURN E.CAPACITY;
		}
		
		FRIEND ATYPE GET_MAX_CAPACITY(EXT<T> &E){
			RETURN EXT_MAX;
		}
		
		FRIEND T* GET_DATA(EXT<T> &E){
			RETURN E.POINTER;
		}
		
		FRIEND ATYPE GET_SIZE(EXT<T> &E){
			RETURN E.SIZE;
		}
};
