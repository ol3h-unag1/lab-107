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

auto lambda_filter = [](UnderlyingNumericType i) 
{ 
    static UnderlyingNumericType step{ from };

    auto mod{ i % divisor };
    auto division{ i / divisor };

    auto magical{ (step / division + step / mod) * std::numbers::phi };


    bool filter{ magical / std::numbers::sqrt2 < std::numbers::egamma };

    step += mod;
    
    return filter;
};

auto lambda_transform = [](UnderlyingNumericType i)
{
    UnderlyingNumericType transformed{ static_cast<UnderlyingNumericType>( std::ceil(i * std::numbers::e) ) };

    return transformed;
};

int main()
{
    auto look{ std::views::iota(from) | std::views::filter(lambda_filter) | std::views::take(much) };

    std::vector< int > numbers;
    numbers.reserve(much * insurance);
    std::ranges::copy(look, std::back_inserter(numbers));
    std::ranges::copy(numbers, std::ostream_iterator<int>(std::cout, " "));

}
