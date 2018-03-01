#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <map>

#include <algorithm>

#include "mktCsvData.h"

/****
 * 1. Usage every member Function calls...
 * lock1, fnCall (depth 1)
 * lock2, fnCall (depth 2)
 * lock3, fnCall (depth 3)
 * release3
 * release2
 * release1
 * 
 * 2. Usage, one time Lock and function calls free
 * lock
 *  ,fnCall(depth 1)
 *  ,fnCall(depth 2)
 *  ,fnCall(depth 3)
 * Release
 * 
 * conclusion
 * Lock method must be outside of member function call :D....
 ****/


/*
template< class T >
class InterlockedMethodCall
{
    T& _target;
public:
    InterlockedMethodCall( T& rhs )
    :_target(rhs)
    {

    };

    // template<  >
    template< class TRet >
    inline auto CallMethod( std::mem_fn< ReturnType(TRet) ,T>& fn )
    {
        //Lock lock ( _target.lockInst )   
        return fn(_target);
    }
};

template<class T>
class CallerWrapper: public InterlockedMethodCall<T>
{
    CallerWrapper() = delete;
public:

    CallerWrapper(T& rhs):InterlockedMethodCall<T>(rhs)
    {

    }
};

class Callee
{
private:
    int OutOne(){
        std::cout << "" << std::endl;
    }
public:
    int OutTwo(){
        std::cout << "" << std::endl;
    }
    Callee(){}
};

void main()
    Callee caller;

    caller.CallMethod( &Callee::OutTwo );

*/


//아무리 달라도, enumeration은 밖에서 정의 할 수 있다.

namespace ZAK
{
    enum EFELD{
        FELD_NULL,
        SATZART,
        BEARBETIUNGS_WOCHE,
        ERSTELLUNGS_DATUM,
        GLN_DATENSENDER,
        GLN_DATENEMPFAENGER,
        FELD_CNT,
    };

    const int E3CC = 'ZAK';
}

namespace ZBK
{
    enum EFELD{
        FELD_NULL,
        SATZART,

        FELD_CNT,
    };
}

struct BASE_MKT_FELD
{
    virtual const char* GetFeldDesc() = 0;

    //std::function<const char*(void )> GetFeldDesc;
    //const char (*FN_GETFELDDESC)(void) pGetFeldDesc;
};

template<typename Derived>
struct IMKT_FELD : public BASE_MKT_FELD
{
    virtual const char* GetFeldDesc()
    {
        return static_cast< Derived* >(this)->FeldDesc;
    }
};

template< class COwner, int nCur>
struct IMPL_MARKANT_FELD : public IMKT_FELD< IMPL_MARKANT_FELD<COwner, nCur> >
{
    const char * FeldDesc = "INVALID OBJCT, DOES NOT EXISTS";
};


// template<>

typedef std::shared_ptr< BASE_MKT_FELD > PBASE_MKT_FELD ;
typedef std::vector< PBASE_MKT_FELD > CONT_MKTFELD;


/*
    Generic..... more , Generalize

    class TRepo, class TEntity, class ...Args(X)
    just only passing method Repo Call, Current Numeber as TemplateParameter to use Create<>
    AbstractFactory?

    Repository.Contain<..Args>( Pos )
*/

/*
//ToCreate to BaseClass
template< class Derived, class ToCreate, class... Args >
class ComnFactory
{
public:
    virtual ToCreate Create( Args ) = 0;
};

template< int nCur >
class MarkanFeldFactory : public ComnFactory< MarkanFeldFactory<nCur>, PBASE_MKT_FELD >
{
public:
    template<int nCur>
    PBASE_MKT_FELD CreateEntity( )
    {
        return std::make_shared<  >
    }
    

    virtual PBASE_MKT_FELD Create()
    {
        return std::make_shared<  >();
    }
};


template < class TRepo, class Factory ,unsigned int nCur >
class ComnRecursiveRegister
{
    typedef ComnRecursiveRegister<TRepo,TEntity,nCur> _MyT;
public:
    static void DoRegist( TRepo& rhsRepo)
    {
        ComnRecursiveRegister<TRepo,TEntity,nCur-1>::DoRegist( rhsRepo );

        rhsRepo.Regist(  nCur, Factory::Create( nCur ) );

        return ;
    }
};

template < class TRepo, class TEntity >
class ComnRecursiveRegister<  TRepo,  TEntity, 0 >
{
    static void DoRegist( TRepo& rhsRepo)
    {
        return ;        
    }
};
*/

//rhsRepo.Regist<nCur>();
/**/
template< class TRepo, unsigned int nCur >
class ComnResursiveRegister
{

};

class Repository 
{ 
    typedef Repository COwner;
    typedef std::vector<PBASE_MKT_FELD> CONT_TYPE;
    template<unsigned int nCur> 
    void Regist()
    {
        _cont[ nCur ] = std::make_shared< IMPL_MARKANT_FELD< COwner, nCur > >();
    } 

private:
    CONT_TYPE _cont;
};

class RepositoryFELDINFO
{
    typedef int E3CC;
    typedef std::map< E3CC, std::shared_ptr< struct BASE_MKT_FELDINFO> > CONT_TYPE;

    template<unsigned int nCur>
    void Regist()
    {
        _cont[typeinfo<nCur>::E3CC_VALUE] = std::make_shared< typeinfo<nCur>::IMPLTYPE >();
    }

private:
    CONT_TYPE _cont;
};


template< class FELDINFO , unsigned int nCur >
class FELDRegister
{
public:
    static void Do( FELDINFO& rhs )
    {
        FELDRegister< FELDINFO ,nCur-1 >::Do(rhs);

/*
*/
        //rhs.Regist< Args >( key/pos )
        CONT_MKTFELD& cont = rhs.GetRawCont();
        if(nCur < cont.size())  cont[ nCur ] = std::make_shared< IMPL_MARKANT_FELD< FELDINFO, nCur > >();


/* failed Const
        const CONT_MKTFELD& cont = rhs.GetRawCont();
        cont[ nCur ] = std::make_shared< IMPL_MARKANT_FELD< FELDINFO, nCur > >();
*/        

        /* Possible
        Handler< CONT_MKTFELD > cont =  rhs.GetCont();
        auto it(cont->begin());
        std::advance( it , nCur );
        // cont->emplace( it, std::make_shared< IMPL_MARKANT_FELD< FELDINFO, nCur > >() );
        cont->insert( it, std::make_shared< IMPL_MARKANT_FELD< FELDINFO, nCur > >() );
        */

        return ;
    };
};

template< class FELDINFO >
class FELDRegister<FELDINFO,0>
{
public:
    static void Do( FELDINFO& )
    {
        return ;
    }
};



template< class T >
class Handler
{
    T& rfT;

    Handler();

public:
    Handler(T& rhs): rfT(rhs) {  }
    T* operator->(){ return &rfT; }
    T* GetPtr(){ return &rfT; }

    // T()
};

struct BASE_MKT_FELDINFO
{
    virtual Handler< CONT_MKTFELD > GetCont()=0;
    virtual void DoFeldRegist() = 0;
    virtual const CONT_MKTFELD& GetRawCont()=0;
};

typedef std::map< int, std::shared_ptr< BASE_MKT_FELDINFO > > CONT_MKTFELDINFO;


//each Objects have atomic operation.... std::atomic (c++20)
//Describ lifespan / lifecycle... how long survive this Object

/****
 *  상속에 있어서, 강제로 정의하도록 하느냐, 정의가 되어있지 않으면 기본값을
 * 반환하도록 하느냐는 충분한 고민 이후에 내려야 할 결론이다.
 * ***/

template<typename Derived>
struct IMKT_FELDINFO : public BASE_MKT_FELDINFO
{
    // enum{ CONT_CNT = Derived::CONT_CNT };

    virtual Handler< CONT_MKTFELD > GetCont()
    {
        return Handler<CONT_MKTFELD>( static_cast<Derived*>(this)->_cont );
    }

    virtual CONT_MKTFELD& GetRawCont()
    {
        return static_cast<Derived*>(this)->_cont;
    }

    virtual void DoFeldRegist()
    {
        FELDRegister< Derived::_MyT, Derived::CONT_CNT > instReg;
        instReg.Do( * static_cast<Derived*>(this) );

    }

    IMKT_FELDINFO()
    {
        /*
         FELDRegister< Derived::_MyT, Derived::CONT_CNT > instReg;
         instReg.Do( static_cast<> );
        */
    }

};

template< typename FENUM ,int nCC >
struct IMPL_MKT_FELDINFO : public IMKT_FELDINFO< IMPL_MKT_FELDINFO< FENUM, nCC> >
{
    typedef IMPL_MKT_FELDINFO<FENUM,nCC> _MyT;
    typedef IMKT_FELDINFO< _MyT > _BaseT;

    enum : int { 
        CONT_CNT=FENUM::FELD_CNT
         };

    // static std::vector< PBASE_MKT_FELD > _cont;
    static CONT_MKTFELD _cont;
};

#define DECL_SELF \
    constexpr auto helper() -> std::remove_reference< decltype(*this) >::type; \
    typedef decltype(helper()) self;

namespace ZAK
{


#define DECL_MARKANT_FELD( E ) template<> struct IMPL_MARKANT_FELD< FELDINFO, EFELD:: ## E > : public IMKT_FELD< IMPL_MARKANT_FELD< FELDINFO, EFELD:: ## E > >

    typedef IMPL_MKT_FELDINFO< EFELD ,E3CC > FELDINFO;

    
    
    template<>
    struct  IMPL_MARKANT_FELD< FELDINFO, EFELD::SATZART > : public IMKT_FELD<
            IMPL_MARKANT_FELD< FELDINFO, EFELD::SATZART >
      >
    {
        const char* FeldDesc = "Satzart, konstant";
        typedef IMPL_MARKANT_FELD< FELDINFO, EFELD::SATZART > _MyT;
        enum {MAXLEN = 3};
    };

    template<>
    struct  IMPL_MARKANT_FELD< FELDINFO, EFELD::BEARBETIUNGS_WOCHE > : public IMKT_FELD<
            IMPL_MARKANT_FELD< FELDINFO, EFELD::BEARBETIUNGS_WOCHE >
      >
    {
        const char* FeldDesc = "Die Bearbeitungswoche entspricht der Regulierungswoche bei MARKANT";
        typedef IMPL_MARKANT_FELD< FELDINFO, EFELD::BEARBETIUNGS_WOCHE > _MyT;
        
        enum {MAXLEN = 6};
    };

    template<>
    struct  IMPL_MARKANT_FELD< FELDINFO, EFELD::ERSTELLUNGS_DATUM > : public IMKT_FELD<
            IMPL_MARKANT_FELD< FELDINFO, EFELD::ERSTELLUNGS_DATUM >
      >
    {
        const char* FeldDesc = "Das Erstellungsdatum stellt das Datum der Erstellung der Datei war";
        typedef IMPL_MARKANT_FELD< FELDINFO, EFELD::ERSTELLUNGS_DATUM > _MyT;
        enum {MAXLEN = 8};        
    };


    DECL_MARKANT_FELD(GLN_DATENSENDER)
    {
        const char* FeldDesc = "GLN des Datensenders";
        //typedef decltype(*this) _MyT;
        //DECL_SELF;
    };

    DECL_MARKANT_FELD( GLN_DATENEMPFAENGER )
    {
        const char* FeldDesc = "GLN des Datenempfaengers";
    };

#undef DECL_MARKANT_FELD
}

CONT_MKTFELD IMPL_MKT_FELDINFO< ZAK::EFELD, 'ZAK' >::_cont(ZAK::FELD_CNT);

class FELDINFO_REPO
{
public:
    CONT_MKTFELDINFO _cont;
    static FELDINFO_REPO& GetInstance()
    {
        static FELDINFO_REPO _Instance;
        return _Instance;
    };

private:
    FELDINFO_REPO();
};


//MKTTYPES use RECURSIVE FUNCTION regist... or ,, etc
enum EMKTTYPES
{
    EMKTTYPES_NULL,
    EZAK,
    EZBA,
    EZBB,
    EMKTTYPES_CNT,
};

template< unsigned int cc >
struct MKT_TYPE_INFO;

template<>
struct MKT_TYPE_INFO<'ZAK'>
{
    const static unsigned int E3CC = 'ZAK';
    typedef ZAK::EFELD ENUMFELD;
    typedef IMPL_MKT_FELDINFO< ENUMFELD, E3CC >  FELDINFO;
};


/*
template<>
struct MKT_TYPE_INFO<'ZBA'>
{
    const static unsigned int E3CC = 'ZBA';
    typedef ::EFELD ENUMFELD;
    typedef IMPL_MKT_FELDINFO< ENUMFELD, E3CC >  FELDINFO;
    
};
*/





FELDINFO_REPO::FELDINFO_REPO()
:_cont{
    // {'ZAK', std::make_shared< IMPL_MKT_FELDINFO< ZAK::EFELD, 'ZAK' > >() },
    {'ZAK', std::make_shared< MKT_TYPE_INFO<'ZAK'>::FELDINFO >() },
    // {'ZBA', std::make_shared< IMPL_MKT_FELDINFO< ZBK::EFELD, 'ZBA' > >() },
    // {'ZBB', std::make_shared< IMPL_MKT_FELDINFO< ZBK::EFELD, 'ZBB' > >() },
}
{
    for( auto curPair : _cont)
    {
        // FELDRegister ;
        curPair.second->DoFeldRegist();
    }
}

int main(void)
{
    IMPL_MKT_FELDINFO< ZAK::EFELD ,ZAK::E3CC> _Instance;
    auto hContMktFeld = _Instance.GetCont();

    std::cout << hContMktFeld->size() << std::endl;
    printf( "%#016x\n", &(_Instance._cont ) );
    printf( "%#016x\n", hContMktFeld.GetPtr() );


    for( auto it : FELDINFO_REPO::GetInstance()._cont)
    {
        auto contFeld = it.second->GetCont();
        std::for_each( contFeld->begin(), contFeld->end(), [](auto& rhs)
        {
            if( nullptr != rhs ) std::cout << rhs->GetFeldDesc() << std::endl;
        } );
    }

    MKT_TYPE_INFO<'ZAK'> typInfo;
    //MKT_TYPE_INFO<'ZB'> typInfo2;

    for( auto toPrt :dataRAW )
		std::cout << toPrt << std::endl;


    return 0;
}