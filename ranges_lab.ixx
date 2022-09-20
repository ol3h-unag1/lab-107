export module RangesLab;

import <vector>;
import <ranges>;

import <numbers>;

import <iostream>;

using Integral = int;
using Floating = double;

Integral constexpr divisor{ 3377 };

Integral constexpr from{ 0 };
Integral constexpr much{ 33 };

Integral constexpr insurance{ 8 };



auto lambda_filter = [](Integral i)
{
    static Integral step{ 0 };

    auto mod{ i % divisor };
    auto mod_or_negative{ mod ? mod : -1 };

    step += mod_or_negative;
    step %= divisor;

    return step * std::numbers::inv_sqrt3 < mod* std::numbers::sqrt2;
};

auto lambda_transform = [](Integral i)
{
    auto transformed{ Integral(std::floor(i * std::numbers::e)) % much };

    return transformed;
};

export void TestRanges()
{
    auto look{ std::views::iota(from) | std::views::filter(lambda_filter) | std::views::transform(lambda_transform) | std::views::take(much) };

    std::vector< int > numbers;
    numbers.reserve(much * insurance);
    std::ranges::copy(look, std::back_inserter(numbers));
    std::ranges::copy(numbers, std::ostream_iterator<int>(std::cout, " "));
}
