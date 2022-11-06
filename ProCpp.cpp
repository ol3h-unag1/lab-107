import <iostream>;

import <map>;

import <vector>;

import <span>;

import type_name;

import Misc;

import TraceLifeTime;

import Singleton;


int main()
{
    //auto s8080{ Singleton< 8080 >::instance() };

    //s8080->doSomething();

    //auto t1488{ TGGTTN::Taggetton< TGGTTN::Top, 308 >::getInstance< TGGTTN::Bottom >()};

    //std::cout << t1488->getName() << std::endl;
    ////std::shared_ptr< TGGTTN::Top >  pTop{
    ////    std::make_shared< TGGTTN::Bottom >()
    ////};

    ////std::cout << pTop->getName() << std::endl;

    //Tracer tr(std::cout);


    //std::vector< int > v;
    //auto i{ foo(v) };


    Test_PointersViolatingConstSemantics();

}