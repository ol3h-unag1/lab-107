import <iostream>;


import type_name;

import Misc;

import TraceLifeTime;

// https://stackoverflow.com/questions/74046714/call-non-virtual-method-of-derived-class-in-polymorphic-hierarchy-of-library-tha
namespace LibraryCode // can't be changed
{
    class Animal
    {
    public:
        virtual ~Animal() = default;

    public:
        virtual void move() const { std::cout << "Animal: moving!" << std::endl; };

        void breath() const { std::cout << "Animal: breathing!" << std::endl; }
    };

    class Duck : public Animal
    {
    public:
        void move() const override { std::cout << "Duck: flying!" << std::endl; }

        void make_nest() const { std::cout << "Duck: making nest!" << std::endl; }
    };

    class Dolphin : public Animal
    {
    public:
        void move() const override { std::cout << "Dolphin: swimming!" << std::endl; }

        void make_air_bubble() const { std::cout << "Dolphin: making air  bubble!" << std::endl; }
    };
}

void Bar(LibraryCode::Animal const& a)
{
    a.breath();
    a.move();

    // w/o using dynamic_cast call make_nest if it's a Duck

    // w/o using dynamic_cast call make_air_bubble if it's a Dolphin
}

int main()
{
    Bar(LibraryCode::Duck{});
    Bar(LibraryCode::Dolphin{});
}