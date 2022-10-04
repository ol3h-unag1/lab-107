export module TraceLifeTime;

import <iostream>;

export class TraceLifeTime final
{
public:
    TraceLifeTime() { std::cout << this << " TraceLifeTime default c-tor" << std::endl; }
    TraceLifeTime(int i) : _i(i) { std::cout << this << " TraceLifeTime(int) c-tor: int = " << i << std::endl; }

    TraceLifeTime(TraceLifeTime const&) { std::cout << this << " TraceLifeTime copy c-tor" << std::endl; }
    TraceLifeTime& operator=(TraceLifeTime const&) { std::cout << this << " TraceLifeTime copy operator=" << std::endl; return *this; }

    TraceLifeTime(TraceLifeTime&&) { std::cout << this << " TraceLifeTime move c-tor" << std::endl; }
    TraceLifeTime& operator=(TraceLifeTime&&) { std::cout << this << " TraceLifeTime move operator=" << std::endl; return *this; }
    ~TraceLifeTime() { std::cout << this << " TraceLifeTime destructor" << std::endl; }


private:
    int _i;
};