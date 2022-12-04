import <iostream>;

import <chrono>;

int main() // turn on compiler warnings    
{

    auto step_minutes{ std::chrono::minutes(90) };
    auto total_minutes{ std::chrono::minutes(0) };
    auto minimum_hours{ std::chrono::hours(7) };

    std::cout << "start" << std::endl;

    std::size_t const breaker{ 1000u };
    std::size_t breaker_counter{ 0u };
    while (total_minutes < minimum_hours)
    {
        total_minutes += step_minutes;

        if (++breaker_counter >= breaker)
        {
            break;
        }
    }

    std::chrono::hours total_hours{ std::chrono::duration_cast< std::chrono::hours >( total_minutes ) };
    std::chrono::minutes leftover_minutes{ total_minutes - std::chrono::duration_cast< std::chrono::minutes >( total_hours ) };
    std::cout << total_hours << " " << leftover_minutes << std::endl;

    std::cout << "end" << std::endl;
    return 0;
}

