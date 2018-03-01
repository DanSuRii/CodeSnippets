#include <iostream>

enum ETYPE: unsigned int 
{
    FOO,
    BAR,
    ETYPE_CNT
};

struct BASE
{

};

template<class Derived>
struct IBASE
{

};

template< unsigned int CURTYPE >
struct IMPL_DERIVED : public IBASE< IMPL_DERIVED<CURTYPE> >
{

};

#define MAC_BASE_T( E ) IBASE< IMPL_DERIVED< ETYPE:: ## E > >
#define MAC_SELF_T( E ) IMPL_DERIVED< ETYPE:: ## E >

#define SPEC_TYPE( E ) template<> struct MAC_SELF_T(E) : public MAC_BASE_T(E)

#define DECL_SELF_BASE( E ) \
 typedef MAC_SELF_T( E ) _MyT; \
 typedef MAC_BASE_T( E ) _BaseT;

#define BEGIN_SPEC_TYPE( E, CONST_X, CONST_Y ) \
    SPEC_TYPE( E ) \
    { DECL_SELF_BASE( E ) \
    static const int _FIRST_PROP = CONST_X; \
    static const int _SECOND_PROP = CONST_Y; 

#define END_SPEC_TYPE };

#define IMPL_SPEC( ... ) \
    BEGIN_SPEC_TYPE( __VA_ARGS__ ) \
    END_SPEC_TYPE

IMPL_SPEC( FOO, 10, 2 )
IMPL_SPEC( BAR, 20, 3 )

int main(void)
{

}
