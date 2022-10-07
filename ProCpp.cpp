import <iostream>;

// Approach 1. Concept.
template <typename Init, typename T > 
concept AddableC = requires (T&& t) { Init{} + t; };

template <typename Init, typename T>
Init AccumValue(T const&)
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
    return (Init{} + ... + AccumValue< Init, Args >( std::forward< Args >( args ) ));
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

int main()
{
    auto result1{ Accum<int>(10, 20, 1.1) }; // works
    std::cout << result1 << std::endl;

    //auto result2{ Accum<int>(20, 40, 2.2, "string")}; // doesn't compile
    //std::cout << result2 << std::endl;

    auto result3{ Sum< double >(1, 2, 3.3, "asda") + Sum< double >()}; // works
    std::cout << result3 << std::endl;
}
