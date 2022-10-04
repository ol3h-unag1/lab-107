import <iostream>;

import <vector>;

import <string>;

import <mutex>;

import <source_location>;

import <memory>;

import type_name;

import Singleton;


using S10 = Singleton< 10 >;
using S20 = Singleton< 20 >;

int main()
{
    S20::type s20{ nullptr };

    if (S10::hasInstance())
    {
        S10::instance()->doSomething();
    }
    else
    {
        s20 = S20::instance();
    }

    if (S20::hasInstance())
    {
        s20->doSomething();
    }


}
