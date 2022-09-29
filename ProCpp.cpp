import <iostream>;

import <vector>;

import <string>;


template <typename T>
constexpr auto type_name() {
    std::string_view name, prefix, suffix;
#ifdef __clang__
    name = __PRETTY_FUNCTION__;
    prefix = "auto type_name() [T = ";
    suffix = "]";
#elif defined(__GNUC__)
    name = __PRETTY_FUNCTION__;
    prefix = "constexpr auto type_name() [with T = ";
    suffix = "]";
#elif defined(_MSC_VER)
    name = __FUNCSIG__;
    prefix = "auto __cdecl type_name<";
    suffix = ">(void)";
#endif
    name.remove_prefix(prefix.size());
    name.remove_suffix(suffix.size());
    return name;
}

template< typename T >
int SAI_BF_helper(T&&) { return 0; }

int SAI_BF_helper(int i) { return i; }

// function that take any number parameters of any type and then return sum of all ints using binary left folding expressions
template< typename ... Types >
int SumAllInts_BinaryLeftFold(Types ... args)
{
    return (0 + ... + SAI_BF_helper(args));
}

template< typename T >
void PrintTypeName(T&& t)
{
    std::cout << type_name< decltype( std::forward< T >(t) )> () << std::endl;
}

// if this overload is removed then 'PrintTypeName({1,2,3});' code will not compile
template< typename T >
void PrintTypeName( std::initializer_list< T >&& t)
{
    std::cout << type_name< decltype(std::forward< std::initializer_list< T > >(t))>() << std::endl;
}

int main()
{
    std::vector< int > numbers{ 1, 2, 3 };
    auto il = { 1, 2, 3, 4, 5 }; 
    PrintTypeName(numbers); // output: class std::vector<int,class std::allocator<int> >&
    PrintTypeName(il); // output: class std::initializer_list<int>&
    PrintTypeName({1,2,3}); // output: class std::initializer_list<int>&&

    std::cout << SumAllInts_BinaryLeftFold() << std::endl; // 0 
    std::cout << SumAllInts_BinaryLeftFold("", 0, 1, 2.2, 'a', "char*", 10, std::string("str"), numbers, il) << std::endl; // 11
    //std::cout << SumAllInts_BinaryLeftFold( 1, {1, 2}) << std::endl; // MSVC error message: 'initializer list': is not a valid template argument for 'Types'
}
