#include <stdlib.h>     /* exit, EXIT_FAILURE */
#define TEST(b){                              \
	if(!b){                                   \
		std::cout<<"TEST ALERT: \n";          \
		std::cout<<"FILE: "<<__FILE__ <<"\n"; \
		std::cout<<"LINE: "<<__LINE__ <<"\n"; \
		std::cout<<"FINISHED PROGRAM."<<"\n"; \
		exit(EXIT_FAILURE);                   \
	}                                         \
}

VOID LOG(const char* TEXT){
	STD::COUT << TEXT << STD::ENDL;
}

TEMPLATE <TYPENAME T>
VOID LOG(const char* VARNAME, T VAR){
	STD::COUT << VARNAME << ": " << VAR << STD::ENDL;
}
