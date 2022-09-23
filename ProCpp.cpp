import <iostream>;
import <format>;

import <string>;

import <exception>;

import <numeric>;
import <algorithm>;

import <functional>;

import <memory>;

import <vector>;
import <iterator>;

import <span>;
import <ranges>;

import Misc;
import ExceptionsLab;
import RangesLab;

import FireErrorXception;

using std::begin;
using std::end;

using namespace std::literals::string_literals;

auto isSpace{ [](auto const& e) { return std::isspace(e); } };

// return number of spaces removed
std::size_t RoundTrim(std::string& s)
{
    auto distanceLeft{ std::distance(cbegin(s), std::find_if_not(cbegin(s), cend(s), isSpace)) };
    auto distanceRight{ std::distance(crbegin(s), std::find_if_not(crbegin(s), crend(s), isSpace)) };

    s.erase( std::shift_left(begin(s), end(s), distanceLeft), end(s) );
    s.erase( begin(s), std::shift_right(begin(s), end(s), distanceRight) );

    return distanceLeft + distanceRight;
};

int main()
{
    auto s{"   HELLO MOTHER FUCKERS!    "s};

    std::cout << "Original string: \"" << s << "\"" << std::endl;
    auto count{ RoundTrim(s) };
    std::cout << "Trimmed string: \"" << s << "\"" << std::endl;
    std::cout << "Spaces removed: " << count << std::endl;
}
