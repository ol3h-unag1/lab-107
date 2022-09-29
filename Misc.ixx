export module Misc;

import <iostream>;
import <algorithm>;
import <vector>;

/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// 

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

/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// returns number of spaces removed
std::size_t RoundTrim(std::string& s)
{
    auto const beforeTrim{ s.size() };

    auto isSpace{ [](auto const& e) { return std::isspace(e); } };

    s.erase(cbegin(s), std::find_if_not(cbegin(s), cend(s), isSpace));
    s.erase(std::find_if_not(crbegin(s), crend(s), isSpace).base(), end(s));

    return beforeTrim - s.size();
};

/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// template template parameter
template <typename T, template< typename, typename = std::allocator< T > > typename Container = std::vector >
class ContainerAdapter
{
public:
    void Push(T value);

    template< typename Iter >
    void Push(T value, Iter where);

    auto begin() { return _container.begin(); }

private:
    Container< T > _container;
};

template <typename T, template< typename, typename > typename Container>
void ContainerAdapter<T, Container>::Push(T value)
{
    _container.insert(end(_container), std::move(value));
}

template <typename T, template< typename, typename > typename Container >
template<typename Iter>
void ContainerAdapter<T, Container>::Push(T value, Iter where)
{
    _container.insert(where, std::move(value));
}
