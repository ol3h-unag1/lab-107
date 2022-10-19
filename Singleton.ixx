export module Singleton;

import <iostream>;
import <source_location>;

import TraceLifeTime;

export template< int ID >
class Singleton
{
private:
    Singleton() = default;
    ~Singleton() = default;

    Singleton(Singleton const&) = delete;
    Singleton(Singleton&&) = delete;

    Singleton& operator=(Singleton const&) = delete;
    Singleton& operator=(Singleton&&) = delete;

private:
    class MakeSharedEnabler : public Singleton {};

public:
    using type = std::shared_ptr< Singleton >;

public:
    static type instance()
    {
        if (hasInstance())
        {
            return _instance.lock();
        }
        else
        {
            auto s{ std::make_shared<MakeSharedEnabler>() };
            _instance = s;
            return s;
        }
    }

    static bool hasInstance() { return static_cast<bool>(_instance.lock()); }
    static int getID() { return ID; }

    void doSomething() { std::cout << ID << " " << std::source_location::current().function_name() << std::endl; }

private:
    static std::weak_ptr< Singleton > _instance;

    TraceLifeTime tlt;
};

template< int ID >
std::weak_ptr< Singleton< ID > > Singleton< ID >::_instance;


/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// 
