import <iostream>;

import <map>;

import <vector>;

import type_name;

import Misc;

import TraceLifeTime;

import Singleton;

int main()
{
    auto s8080{ Singleton< 8080 >::instance() };

    s8080->doSomething();

    auto t1488{ TGGTTN::Taggetton< TGGTTN::Top, 308 >::getInstance< TGGTTN::Bottom >()};

    std::cout << t1488->getName() << std::endl;
    //std::shared_ptr< TGGTTN::Top >  pTop{
    //    std::make_shared< TGGTTN::Bottom >()
    //};

    //std::cout << pTop->getName() << std::endl;

    Tracer tr(std::cout);

    bool a{ true };
    bool b{ false };

    if (a and not b)
    {
        std::cout << "a and not b" << std::endl;
    }
}