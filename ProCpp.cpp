import <iostream>;

import <map>;

import <vector>;

import type_name;

import Misc;

import TraceLifeTime;

import Singleton;

int main()
{
    auto s8080{ Singleton< 8080 >::instance() };

    s8080->doSomething();
}