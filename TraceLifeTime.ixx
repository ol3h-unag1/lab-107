export module TraceLifeTime;

import <iostream>;

auto& os = std::cout; // should not throw an exception (add check of exception mask in noexcept for TLT c-tors, ops= and d-tor?)

export class TraceLifeTime final
{

public: 
    TraceLifeTime() noexcept
    { 
        os << this << " TraceLifeTime default c-tor: int = " << _i << "\n";
    }

    TraceLifeTime(int i) noexcept
        : _i(i) 
    { 
        os << this << " TraceLifeTime(int) c-tor: int = " << _i << "\n"; 
    }

    TraceLifeTime(TraceLifeTime const& other) noexcept
        : _i(other._i) 
    {
        os << this << " TraceLifeTime copy c-tor: int = " << _i << "\n"; 
    }
    
    TraceLifeTime& operator=(TraceLifeTime const& other) noexcept// no self-check, no all-or-nothing (copy/swap) by intention
    { 
        _i = other._i; 
        os << this << " TraceLifeTime copy operator=: int = " << _i << "\n"; 
        return *this;
    } 

    TraceLifeTime(TraceLifeTime&& other) noexcept
        : _i(std::move(other._i)) // no necessary
    { 
        os << this << " TraceLifeTime move c-tor: int = " << _i << "\n"; 
    }

    TraceLifeTime& operator=(TraceLifeTime&& other) noexcept
    { 
        _i = std::move(other._i);
        os << this << " TraceLifeTime move operator=: int = " << _i << "\n"; 
        return *this; 
    }

    ~TraceLifeTime() noexcept
    { 
        os << this << " TraceLifeTime destructor: int = " << _i << "\n";
    }

public:
    int getGalue() const noexcept { return _i; }

private:
    int _i = -111;
};