
/// logger main.cpp
// (c) Kamenschykov Oleh, 2024
// free to use by extraterestials only
//
// v.1

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
import <optional>;
import <utility>;
import <tuple>;
import <stdexcept>;

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

namespace SteeringDetails::Direction::Convertion
{
    namespace Types
    {
        enum class Direction
        {
            Absolute,
            ClockWise,
            CounterCW
        };

        auto default_direction{ Direction::ClockWise };

        using Angle = std::int16_t;
        using Error = std::optional<Log::StringExc>;

        using Result = std::tuple<Direction, Angle, Error>;
    }

    Types::Result ResultBuilder(Types::Direction&& d, Types::Angle&& a, Types::Error&& e) {

        using namespace Types;
        return std::make_tuple<Direction, Angle, Error>(
            std::forward<Direction>(d),
            std::forward<Angle>(a),
            std::forward<Error>(e)
        );
    }

    template<Types::Direction D>
    Types::Result convertImplentation(Types::Angle const a, bool clamp = false);

    template<>
    Types::Result convertImplentation<Types::Direction::Absolute>(Types::Angle const a, bool clamp) {

        using namespace Types;
        if (a == 0)
        {
            return ResultBuilder(
                std::move(Direction{ default_direction }), 
                Angle{},
                Error{});
        }
        else if (a < -180)
        {
            return ResultBuilder(
                Direction::Absolute,
                clamp ? -180 : a,
                Log::StringExc{
                    std::format("{} converting Absolute angle out range, angle = {}; clamp = {}", __FUNCTION__, a, clamp)
                });
        }
        else if (a > 180)
        {
            return ResultBuilder(
                Direction::Absolute,
                clamp ? 180 : a,
                Log::StringExc{
                    std::format("{} converting Absolute angle out range, angle = {}; clamp = {}", __FUNCTION__, a, clamp)
                });
        }
        else if (a < 0)
        {
            auto direction = Direction::CounterCW;
            auto angle = -180 + a;
            return ResultBuilder(
                std::move(direction),
                std::move(angle),
                Error{}
            );
        }
        else if (a > 0)
        {
            auto direction = Direction::ClockWise;
            auto angle = 180 - a;
            return ResultBuilder(
                std::move(direction),
                std::move(angle),
                Error{}
            );
        }
        else
        {
            throw std::invalid_argument(std::format("unreachable line of code in {} {}:{}", __FUNCTION__, __FILE__, __LINE__));
        }
    }

    auto convert(Types::Angle a, Types::Direction d, bool clamp = false) {

        using namespace Types;
        if (Direction::Absolute == d)
        {
            return convertImplentation<Direction::Absolute>(a, clamp);
        }
        else if (Direction::ClockWise == d)
        {
            return convertImplentation<Direction::Absolute>(a, clamp); // ?
        }
        else if (Direction::CounterCW == d)
        {
            return convertImplentation<Direction::Absolute>(a, clamp); // ? 
        }
        else
        {
            return convertImplentation<Direction::Absolute>(a, clamp); // ? 
        }   //Error{ Log::StringExc(std::format("{} NO IMPLEMENTATION!", __FUNCTION__)) };
       
    }
 
};




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
    // 1 - turn CW, -1 - turn CCW; angle[0;90]
private:
    void turn_Implementation(std::int16_t direction, std::int16_t angle);

private:
    std::int16_t _current_direction;
    std::int16_t _current_angle;

private:
    static std::int16_t constexpr _c_absolute_direction = 0;
    static inline messy_details::Int16Range const _c_absolute_direction_angles_range{ -180, 180 };
    static inline messy_details::Int16Range const _c_directed_range{ 0, 90 };
    static inline messy_details::Int16Range const _c_direction_numerical_representation_range{ -1, 1 };

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
void Auto<Logger>::turn(std::int16_t direction, std::int16_t angle) {

    auto impl = [&]() {
        //throw std::exception("chekka");
        messy_details::turnWheelInputCheck(
            (Auto<Logger>::_c_absolute_direction == direction ? Auto<Logger>::_c_absolute_direction_angles_range : Auto<Logger>::_c_directed_range),
            Auto<Logger>::_c_direction_numerical_representation_range,
            direction,
            angle);

        turn_Implementation(direction, angle);
        };

    auto logger = _logger;
    auto excopt = logger.execute(impl);
    if (excopt)
    {
        std::rethrow_exception(excopt.value());
    }
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

class Logable;
class Client
{

};

int main()
{
    try
    {
        using OutType = std::decay_t<decltype(std::cout)>;
        using LoggerType = Log::Logger<OutType>;
        using LogerRep = LoggerType::Representation;

        auto logger = LoggerType::build(std::cout);

        Auto<LogerRep> a(logger);
        a.turn(-2, 100);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "hello world!";
    }
    return 0;
}

////