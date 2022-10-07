import <iostream>;

template <typename Init, typename T > 
concept addable = requires (T&& t) { Init{} + t; };

template <typename Init, typename T>
Init helper(T&&)
{
    return Init{};
}

template <typename Init, typename T> requires addable< Init, T >
auto helper(T&& value)
{
    return value;
}

template< typename Init, typename ... Args >
auto Add(Args&& ... args)
{
    return (Init{} + ... + helper< Init >( args ));
}


template< typename Init, typename T > requires !addable< Init, T >
auto Value(T&&)
{
    std::cout << "T&&" << std::endl;
    return Init{};
}

template< typename Init >
auto Value(int const& i)
{
    std::cout << "int" << std::endl;
    return i;
}

template< typename Init >
auto Value(double const& d)
{
    std::cout << "double" << std::endl;
    return d;
}

template< typename Init, typename ... Args >
auto Sum(Args&& ... args)
{
    return (Init{} + ... + Value< Init >(args));
}


int main()
{
    //auto result1{ Add<int>(10, 20, 1.1) }; // works
    //std::cout << result1 << std::endl;

    //auto result2{ Add<int>(20, 40, 2.2, "string")}; // doesn't compile
    //std::cout << result2 << std::endl;

    auto result3{ Sum< double >(1, 2, 3.3)};
    std::cout << result3 << std::endl;
}
