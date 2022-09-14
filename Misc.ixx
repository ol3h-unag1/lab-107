export module Misc;

import <iostream>;

class Base
{
public:
    virtual void go(int i = 0) const { std::cout << __FUNCTION__ << " " << i << std::endl; }
};

class Derived : public Base
{
public:
    void go(int i = 1) const override { std::cout << __FUNCTION__ << " " << i << std::endl; }
};


export void testDefaultParametersInOverridenMethods()
{
    Base b;
    Derived d;
    Base& bRefD{ d };

    b.go();
    d.go();
    bRefD.go();
}