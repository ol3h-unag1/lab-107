export module Singleton;

import <iostream>;
import <source_location>;

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
};

template< int ID >
std::weak_ptr< Singleton< ID > > Singleton< ID >::_instance;


/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// 
namespace TGGTTN
{
    class Top;
    class Middle;
    class Bottom;

    auto GetName(Top const*)
    {
        return "Top";
    }    
    
    auto GetName(Middle const*)
    {
        return "Middle";
    }    
    
    auto GetName(Bottom const*)
    {
        return "Bottom";
    }

    export class Top
    {
    public:
        virtual ~Top() = default;

    public:
        virtual std::string_view getName() const noexcept {
            return GetName(this); 
        }

    public:
        void top() { std::cout << "top" << std::endl; }
    };

    export class Middle : public Top
    {
    public:
        std::string_view getName() const noexcept override {
            return GetName(this);
        }

    public:
        void middle() { std::cout << "middle" << std::endl; }
    };

    export class Bottom : public Middle
    {
    public:
        std::string_view getName() const noexcept override {
            return GetName(this);
        }

        void bottom() { std::cout << "bottom" << std::endl; }
    };

    export
    template< class T, int Tag >
    class Taggetton
    {
    private:
        Taggetton() = default;
        ~Taggetton() = default;

    public:
        static bool hasInstance() {
            return not _weakInstance.expired();
        }

        using element_type = T;
        using value_type = std::shared_ptr< element_type >;

        template< class Dynamic = T >
        static value_type getInstance()
        {
            if (hasInstance()) {
                return _weakInstance.lock();
            }
            else {
                auto s{ std::make_shared<Dynamic>() };
                _weakInstance = s;
                return s;
            }
        }

        T* operator->() const {
            if (hasInstance())
                return _weakInstance.lock().get();
        }

    private:
        static std::weak_ptr < T > _weakInstance;;
    };

    template< class T, int Tag >
    std::weak_ptr < T > Taggetton< T, Tag >::_weakInstance{};
}