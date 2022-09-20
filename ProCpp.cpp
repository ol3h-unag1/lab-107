import <iostream>;
import <format>;

import <exception>;

import <memory>;

import <vector>;
import <iterator>;
import <span>;
import <ranges>;

import <numbers>;

//import Misc;
//import ExceptionsLab;

using std::begin;
using std::end;

using UnderlyingNumericType = int;
UnderlyingNumericType constexpr from{ 1 };
UnderlyingNumericType constexpr much{ 33 };
UnderlyingNumericType constexpr insurance{ 8 };

UnderlyingNumericType constexpr divisor{ 3377 };

auto lambda_filter_by_divisor = [](UnderlyingNumericType i) 
{ 
    bool mod{ i % divisor };
    
    return mod;
};

auto lambda_transform = [](UnderlyingNumericType i)
{
    UnderlyingNumericType transformed 
};

int main()
{
    auto look{ std::views::iota(from) | std::views::filter([](UnderlyingNumericType) {return true; }) | std::views::take(much) };

    std::vector< int > numbers;
    numbers.reserve(much * insurance);
    std::ranges::copy(look, std::back_inserter(numbers));
    std::ranges::copy(numbers, std::ostream_iterator<int>(std::cout, " "));

}
