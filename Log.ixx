// (c) Kamenschykov Oleh, 2024
// free to use by extraterestials only
//
// v.1


export module Log;

import <exception>;
import <memory>;
import <queue>;
import <string>;
import <cstdint>;
import <functional>;
import <exception>;



export namespace Log {
    
    /// <summary>
    /// 
    /// </summary>
    class StringExc : public std::exception
    {
    public:
        using NumericType = std::size_t;
    public:
        StringExc(std::string const& str)
            : actual_size_(str.size())
        {
            auto message_size = std::min(size(), c_string_exc_buff_size_);
            memcpy(buff_, str.c_str(), message_size);
            buff_[message_size] = '\0';
        }

        const char* what() const override {

            return buff_;
        }

        NumericType size() const {

            return actual_size_;
        }

        constexpr NumericType capacity() const {

            return c_string_exc_buff_capacity_;
        }

        bool overflow() const {

            return size() > capacity();
        }


    private:
        static NumericType constexpr c_string_exc_buff_size_ = 512;
        static NumericType constexpr c_string_exc_buff_capacity_ = c_string_exc_buff_size_ + 1;
        char buff_[c_string_exc_buff_capacity_];
        NumericType actual_size_;
    };


    /// <summary>
    /// 
    /// 
    /// </summary>
    /// <typeparam name="Output"></typeparam>
    template<typename Output>
    class Logger
    {
    public:
        // out must outlive this please
        explicit Logger(Output& out )
            : _out(out) {

        }

        virtual ~Logger() nothrow {

            print();
            flush();
        }

        // no default please
        Logger() = delete;

        // no any copy/assign ops please
        Logger(Logger&) = default;

        

    public:
        using Representation = Logger;

    public:
        static
        typename Representation build(Output& data) {

            return Logger(data);
        }

        // adds messages to queue
        void log(std::string msg) {

            _messages.emplace(std::move(msg));
            if (_messages.size() > c_flush_on_size) {

                print();
                flush();
            }
        }

        //// adds messages to queue
        //Logger& operator<<(std::string msg) {
        //    
        //    log(std::move(msg));
        //    return *this;;
        //}

        // outputs messages queue
        void print() {

            while ( not _messages.empty() )
            {
                _out << _messages.front() << "\n";
                _messages.pop();
            }
        }

    public:
        template<typename Callable>
        void execute(Callable callable) {

            try 
            {
                callable();
            }
            catch (StringExc& se)
            {
                log(se.what());
            }
            catch (...)
            {
                throw;
            }
        }
    private:
        void flush() nothrow {

            try
            {
                _out.flush();
            }
            catch (std::exception& e)
            {
                // ?
            }
            catch(...)
            {

            }
        }
    private:
        std::queue< std::string > _messages; 
        const std::uint32_t c_flush_on_size = 32u;

        Output& _out;
 };

}