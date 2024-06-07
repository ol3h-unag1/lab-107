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
using namespace Log;

namespace messy_details
{
    /// <summary>
    /// 
    /// </summary>

    class Int16Range
    {
    public:
        using ValueType = std::int16_t;

        Int16Range(ValueType min, ValueType max) {
            if (min > max)
                throw Log::StringExc(std::format("{} | min:{} argument is bigger then max:{}", __FUNCTION__, min, max));
                
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
            throw Log::StringExc(std::format("value {} is too big for range [{},{}]", value, range.min(), range.max()));

        if (value < range)
            throw Log::StringExc(std::format("value {} is too small for range [{},{}]", value, range.min(), range.max()));
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
template<typename LoggerRepType>
class Auto // 1nterface
{
public:
    Auto(typename LoggerRepType logger)
        : _logger(logger) {

    }
    Auto() = delete;

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

    LoggerRepType _logger;
};

template<typename Logger>
bool Auto<Logger>::ignition() {
    // state?
    throw Log::StringExc(std::format("{} NO IMPLEMENTATION!", __FUNCTION__));
    return false;
}

template<typename Logger>
void Auto<Logger>::acc() {
    // side effect acc state?
    throw Log::StringExc(std::format("{} NO IMPLEMENTATION!", __FUNCTION__));

}
template<typename Logger>
void Auto<Logger>::turn(std::int16_t direction, std::int16_t angle)
{ 
    auto impl = [&]() {
        messy_details::turnWheelInputCheck(
            (Auto<Logger>::_c_absolute_direction == direction ? Auto<Logger>::_c_absolute_direction_angles_range : Auto<Logger>::_c_directed_range),
            Auto<Logger>::_c_direction_numerical_representation_range,
            direction,
            angle);

        turn_Implementation(direction, angle);
        };
    auto logger = _logger;
    logger.execute(impl);
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

    throw Log::StringExc(std::format("{} unexpected branch!", __FUNCTION__));
}

class Dummy
{
public:
    Dummy() = default;
};

int main()
{
    try {
        using OutType = std::decay_t<decltype(std::cout)>;
        using LoggerType = Log::Logger<OutType>;
        using LogerRep = LoggerType::Representation;

        auto logger = LoggerType::build(std::cout);

        Auto<LogerRep> a(logger);
        a.turn(-2, 100);
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