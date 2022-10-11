export module TraceLifeTime;

import <iostream>;

export class TraceLifeTime final
{
public:
    TraceLifeTime() 
    { 
        std::cout << this << " TraceLifeTime default c-tor: int = " << _i << "\n";
    }

    TraceLifeTime(int i) 
        : _i(i) 
    { 
        std::cout << this << " TraceLifeTime(int) c-tor: int = " << _i << "\n"; 
    }

    TraceLifeTime(TraceLifeTime const& other) 
        : _i(other._i) 
    {
        std::cout << this << " TraceLifeTime copy c-tor: int = " << _i << "\n"; 
    
    }
    
    TraceLifeTime& operator=(TraceLifeTime const& other) // no self-check, no all-or-nothing (copy/swap) by intention
    { 
        _i = other._i; 
        std::cout << this << " TraceLifeTime copy operator=: int = " << _i << "\n"; 
        return *this;
    } 

    TraceLifeTime(TraceLifeTime&& other) 
        : _i(std::move(other._i)) // no necessary
    { 
        std::cout << this << " TraceLifeTime move c-tor: int = " << _i << "\n"; 
    }

    TraceLifeTime& operator=(TraceLifeTime&& other) 
    { 
        _i = std::move(other._i);
        std::cout << this << " TraceLifeTime move operator=: int = " << _i << "\n"; 
        return *this; 
    }

    ~TraceLifeTime() 
    { 
        std::cout << this << " TraceLifeTime destructor: int = " << _i << "\n";
    }

private:
    int _i = -111;
};