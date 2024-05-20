/*

import <iostream>;
import <string>;
import <vector>;

enum class EState
{
    Online,
    Offline,
    Disabled
};

enum class EFeature
{
    LifeSupport,
    Power,
    Pumps,
    Navigation
};

// Conversion to string for EState
std::string toString(EState state)
{
    switch (state)
    {
    case EState::Online:
        return "Online";
    case EState::Offline:
        return "Offline";
    case EState::Disabled:
        return "Disabled";
    default:
        return "Unknown";
    }
}

// Conversion to string for EFeature
std::string toString(EFeature feature)
{
    switch (feature)
    {
    case EFeature::LifeSupport:
        return "LifeSupport";
    case EFeature::Power:
        return "Power";
    case EFeature::Pumps:
        return "Pumps";
    case EFeature::Navigation:
        return "Navigation";
    default:
        return "Unknown";
    }
}

struct Message1
{
    using Type = EState;
    static std::string getName() { return "EState"; }
    EState value;
};

struct Message2
{
    using Type = EFeature;
    static std::string getName() { return "EFeature"; }
    EFeature value;
};

template< typename T, typename U, bool flag>
class Base;

template< typename T, typename U>
class Base<T, U, true>
{
public:
    template< typename Message >
    void sendMessage(typename Message::Type value)
    {
        std::cout << std::boolalpha << true << " " << Message::getName() << ":"
            << toString(value) << std::endl;
    }
};

template< typename T, typename U>
class Base<T, U, false>
{
public:
    template< typename Message >
    void sendMessage(typename Message::Type value)
    {
        std::cout << std::boolalpha << false << " " << Message::getName() << ":"
            << toString(value) << std::endl;
    }
};

template< typename T>
class Base<T, void, true>
{
public:
    void sendMessage() { std::cout << "void spec true" << std::endl; }
};

template< typename T>
class Base<T, void, false>
{
public:
    void sendMessage() { std::cout << "void spec false" << std::endl; }
};

template<typename T, typename U, bool flag>
class Custom : public Base<T, U, flag>
{

};
*/
