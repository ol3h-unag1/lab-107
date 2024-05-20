#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <vector>
#include <exception>
#include <memory>

import <string>;
import <format>;
import <cstdint>;

import Log; 

namespace messy_details
{ 

    class StringExc : public std::exception
    {
    public:
        using NumericType = std::size_t;
    public:
        StringExc(std::string const& str)
            : actual_size_(str.size())
        {
            auto message_size = std::min(size(), capacity());
            memcpy(buff_, str.c_str(), message_size);
        }

        const char* what() const override {

            return buff_;
        }
 
        NumericType size() const {

            return actual_size_;
        }

        NumericType capacity() const {

            return c_string_exc_buff_size_; 
        }

        bool overflow() const {

            return size() > capacity();
        }


    private:
        static NumericType constexpr c_string_exc_buff_size_ = 512;
        char buff_[c_string_exc_buff_size_];
        NumericType actual_size_;
    };

    /// <summary>
    /// 
    /// </summary>

    class Int16Range
    {
    public:
        using ValueType = std::int16_t;

        Int16Range(ValueType min, ValueType max) {
            if (min > max)
                throw StringExc(std::format("{} | min:{} argument is bigger then max:{}", __FUNCTION__, min, max));
                
            min_ = min;
            max_ = max;
        }
        
        ValueType max() const { return max_; }
        ValueType min() const { return min_; }

    private:
        ValueType min_;
        ValueType max_;
    };

    bool operator>(Int16Range::ValueType const& value, Int16Range const& range)
    {
        return value > range.max();
    }

    bool operator<(Int16Range::ValueType const& value, Int16Range const& range)
    {
        return value < range.min();
    }  

     /// <summary>
     /// 
     /// </summary>
     
    template<typename Range, typename Value>
    void includingRangeThrow(Range range, Value value)
    {
        if (value > range)
            throw StringExc(std::format("value {} is too big for range [{},{}]", value, range.min(), range.max()));

        if (value < range)
            throw StringExc(std::format("value {} is too small for range [{},{}]", value, range.min(), range.max()));
    }

    template<typename RangeType>
    void turnWheelInputCheck(RangeType const& range_angle, RangeType const& range_distance, std::int16_t distance, std::int16_t angle)
    {
        includingRangeThrow(range_distance, distance);
        includingRangeThrow(range_angle, angle);
    }
}

/// <summary>
/// 
/// </summary>
/// 
template<typename Logger>
class Auto : public Logger // 1nterface
{
public:
    Auto(Logger logger)
        : _logger(logger) {
        throw messy_details::StringExc(std::format("{} NO IMPLEMENTATION!", __FUNCTION__));
    }
    Auto() {
        throw messy_details::StringExc(std::format("{} NO IMPLEMENTATION!", __FUNCTION__));
    }

    bool ignition(); // toggle ignition; returns = true ignitinon=On, false igntino=Off
    void acc(); // power app accesories
    void turn(std::int16_t direction, std::int16_t angle); // 0 - absolute direction; angle[-180:180]   
                                                           // 1 - CW, -1 CCW; angle[0;90]
private:
    void turn_Implementation(std::int16_t direction, std::int16_t angle);

private:
    std::int16_t _current_direction;
    std::int16_t _current_angle;

private:
    static std::int16_t constexpr _c_absolute_direction  = 0;
    static inline messy_details::Int16Range const _c_absolute_direction_angles_range{-180, 180};
    static inline messy_details::Int16Range const _c_directed_range{0, 90};
    static inline messy_details::Int16Range const _c_direction_numerical_representation_range{-1, 1};

    std::shared_ptr<Logger> _logger;
};

template<typename Logger>
bool Auto<Logger>::ignition() {
    // state?
    throw messy_details::StringExc(std::format("{} NO IMPLEMENTATION!", __FUNCTION__));
    return false;
}

template<typename Logger>
void Auto<Logger>::acc() {
    // side effect acc state?
    throw messy_details::StringExc(std::format("{} NO IMPLEMENTATION!", __FUNCTION__));

}
template<typename Logger>
void Auto<Logger>::turn(std::int16_t direction, std::int16_t angle)
{
    messy_details::turnWheelInputCheck(
        Auto<Logger>::_c_absolute_direction == direction ? Auto<Logger>::_c_absolute_direction_angles_range : Auto<Logger>::_c_directed_range, 
        Auto<Logger>::_c_direction_numerical_representation_range,
        direction,
        angle);

    turn_Implementation(direction, angle);
}

template<typename Logger>
void Auto<Logger>::turn_Implementation(std::int16_t direction, std::int16_t angle)
{
    if (Auto<Logger>::_c_absolute_direction == direction)
    {
        _current_direction = angle > 0 ? 1 : -1;
        _current_angle = std::abs(angle) - Auto<Logger>::_c_directed_range.max();
        
        return;
    }
    else
    {
        _current_direction = direction;
        _current_angle += angle;
        _current_angle = std::clamp(_current_angle, _c_directed_range.min(), _c_directed_range.max());

        return;
    }

    throw messy_details::StringExc(std::format("{} unexpected branch!", __FUNCTION__));
}

class Dummy
{
public:
    Dummy() = default;
};

int main()
{

    try {
        Auto<Dummy> a;
        a.turn(1, 30);
        a.ignition();
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "hello world!";
    }
    return 0;
}