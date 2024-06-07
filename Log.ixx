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
    private:
        // out must outlive this please

        explicit Logger(Output& out )
            : _out(out) {

        }

        virtual ~Logger() {

            flush();
        }

        // no default please
        Logger() = delete;

        // no any copy/assign ops please
        Logger(Logger&) = delete;

        

    public:
        using Representation = std::shared_ptr<Logger>;

    private:
        struct AccessManager : Logger
        {
            AccessManager(Output& data)
                : Logger(data) {

            }
        };

    public:
        // template static function hell incoming
        // it's just a builder that bends access thru inheritance
        // AccessManager is publicly inherited to Specialized (Logger)
        // Representation is a dynamically managed object of the Specialized 
        // Specialized doesn't have open constructors, only way to get instance is thru
        //  method Specialized::build.
        // 
        //template< 
        //    class U, 
        //    class Check = std::enable_if_t< 
        //        std::is_same_v<std::decay_t<U>, std::decay_t<SpecializingData>> ||
        //        std::is_convertible_v<std::decay_t<U>, std::decay_t<SpecializingData>>>>
        //    static 
        // bruh I can't make this work rn can't let me be stopped by this
        //
        static
        typename Representation build(Output& data) {

            return std::make_shared<AccessManager>(data);
        }

        // your journey ends here (< TMP)


        // adds messages to queue
        void log(std::string msg) {

            _messages.emplace(std::move(msg));
        }

        //// adds messages to queue
        //Logger& operator<<(std::string msg) {
        //    
        //    log(std::move(msg));
        //    return *this;;
        //}

        // outputs messages queue
        void flush() {

            bool flush{ not _messages.empty() };
            while ( not _messages.empty() )
            {
                _out << "\n";
                _messages.pop();
            }
            if (flush)
                _out.flush();
        }

    private:
        std::queue< std::string > _messages; 
        const std::uint32_t _flush_on_size = 32u;

        Output& _out;
 };

}