export module Misc;

import <iostream>;
import <algorithm>;
import <vector>;

import type_name;

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

/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// https://stackoverflow.com/questions/73889051/stdinitializer-list-rvalue-literal-as-parameter-of-variadic-template/73889222#73889222
// function that take any number parameters of any type and then return sum of all ints using binary left folding expressions
template< typename T >
int SAI_BF_helper(T&&) { return 0; }

int SAI_BF_helper(int i) { return i; }


template< typename ... Types >
int SumAllInts_BinaryLeftFold(Types ... args)
{
    return (0 + ... + SAI_BF_helper(args));
}

template< typename T >
void PrintTypeName(T&& t)
{
    std::cout << type_name< decltype(std::forward< T >(t))>() << std::endl;
}

// if this overload is removed then 'PrintTypeName({1,2,3});' code will not compile
template< typename T >
void PrintTypeName(std::initializer_list< T >&& t)
{
    std::cout << type_name< decltype(std::forward< std::initializer_list< T > >(t))>() << std::endl;
}

void TestStdInitializerListRVLiteral()
{
    std::vector< int > numbers{ 1, 2, 3 };
    auto il = { 1, 2, 3, 4, 5 };
    PrintTypeName(numbers); // output: class std::vector<int,class std::allocator<int> >&
    PrintTypeName(il); // output: class std::initializer_list<int>&
    PrintTypeName({ 1,2,3 }); // output: class std::initializer_list<int>&&

    std::cout << SumAllInts_BinaryLeftFold() << std::endl; // 0 
    std::cout << SumAllInts_BinaryLeftFold("", 0, 1, 2.2, 'a', "char*", 10, std::string("str"), numbers, il) << std::endl; // 11
    //std::cout << SumAllInts_BinaryLeftFold( 1, {1, 2}) << std::endl; // MSVC error message: 'initializer list': is not a valid template argument for 'Types' 
}


/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// 
// https://stackoverflow.com/questions/73982002/c20-concepts-accumulate-values-of-all-good-types-passed-to-function-variadic
// Approach 1. Concept.
template <typename Init, typename T >
concept AddableC = requires (T && t) { Init{} + t; t + Init{}; static_cast<Init>(t); };

template <typename Init, typename T >
Init AccumValue(T const& value)
{
    return Init{};
}

template <typename Init, typename T> requires AddableC< Init, T >
auto AccumValue(T const& value)
{
    return value;
}

template< typename Init, typename ... Args >
auto Accum(Args&& ... args)
{
    return (Init{} + ... + AccumValue< Init, Args >(std::forward< Args >(args)));
}

// Approach 2. Overloads for target types.
template< typename Init, typename T >
auto Value(T const&)
{
    return Init{};
}

template< typename Init >
auto Value(int const& i)
{
    return i;
}

template< typename Init >
auto Value(double const& d)
{
    return d;
}

template< typename Init, typename ... Args >
auto Sum(Args&& ... args)
{
    return (Init{} + ... + Value< Init >(std::forward< Args >(args)));
}

class TestAccumAndSum_UserDefinedClass {};

export void TestAccumAndSum()
{
    auto result1{ Accum<int>(10, 20, 1.1) }; // works
    std::cout << result1 << std::endl;

    int* pi{ nullptr };
    TestAccumAndSum_UserDefinedClass c;

    auto result2{ Accum<int>(20, 40, 2.2, "string", nullptr, pi, c) }; // doesn't compile
    std::cout << result2 << std::endl;

    auto result3{ Sum< double >(1, 2, 3.3, "asda") + Sum< double >() }; // works
    std::cout << result3 << std::endl;
}
