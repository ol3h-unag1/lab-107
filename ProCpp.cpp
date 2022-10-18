import <iostream>;

import <map>;

import <vector>;

import type_name;

import Misc;

import TraceLifeTime;

class NoDefaultCtor
{
public:
    NoDefaultCtor() = delete;
    NoDefaultCtor(int i) {}
};

int main()
{
    std::map< int, NoDefaultCtor > i2NDC;

    NoDefaultCtor ndc1{ 1 };
    NoDefaultCtor ndc2{ 2 };
    
    i2NDC.insert({ 1, ndc1 });
    i2NDC.insert({ 2, ndc2 });

    auto a{ i2NDC[1] };

}