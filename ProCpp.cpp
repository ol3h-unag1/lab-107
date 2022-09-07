import <iostream>;
import <format>;

import <exception>;

import Dump;


class WillThrow
{
public:
    WillThrow(int i)
    {
        if ( i == 1 )
            throw std::runtime_error(std::format("EXCEPTION! {} object<{}> argument<{}>", __func__, static_cast<void*>(this), i));
    }
};

class Naive
{
public:
    Naive(int i)
    try
        : _wt(i)
    {
        try
        {
            throw std::runtime_error(std::format("EXCEPTION! {} object<{}> argument<{}>", __func__, static_cast<void*>(this), i+100));
        }
        catch (std::exception const& e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (...)
        {
            std::cout << std::format("UNHANDLED EXCEPTION! {}", __func__);
        }

        throw std::runtime_error(std::format("EXCEPTION! {} object<{}> argument<{}>", __func__, static_cast<void*>(this), i+200));
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << std::format("UNHANDLED EXCEPTION! {}", __func__);
    }


private:
    WillThrow _wt;
};

int main()
{
    try
    {
        Naive n{ 0 };
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << std::format("UNHANDLED EXCEPTION! {}", __func__);
    }
}
