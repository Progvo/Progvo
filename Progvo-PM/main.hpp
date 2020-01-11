// (c) 2019-2020 Pttn (https://Progvo.dev)

#ifndef HEADER_Main_hpp
#define HEADER_Main_hpp

typedef uint64_t Word;
#define	WORDSIZE	64ULL
#define	WORDMAX 	18446744073709551615ULL

typedef long double Real;

#define PROGVO_VERSION "0.9-skizo"

#define BUG(message) std::cerr << "CIMO! " << __func__ << ": " << message << " :| !" << std::endl

#endif