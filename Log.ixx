export module Log;

import <exception>;
import <memory>;
import <queue>;
import <string>;
import <cstdint>;

export namespace Log {

    template<typename Output>
    class Logger
    {
    public:
        // out must outlive this please

        explicit Logger(Output const& out) 
            : _out(out)
            , _owner(nullptr) {

        }

        // no default please
        Logger() = delete;

        // no any copy/assign ops please
        Logger(Logger&) = delete;
        
    private:
        // you're lost, start here (TMP >)

        using OutputType = Output;
        using Specialized = Logger<OutputType>;
        using SpecializingData = OutputType;

    public:
        using Representation = std::shared_ptr<Logger<Output>>;

    private:
        struct AccessManager : Specialized
        {
            AccessManager(SpecializingData const& data)
                : Specialized(data) {

            }
        };

    public:
        // template static function hell incoming
        // it's just a builder that bends access thru inheritance
        // AccessManager is publicly inherited to Specialized
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
        typename Representation build(SpecializingData const& data)
        {
            return std::make_shared<AccessManager>(data);
        }

        // your journey ends here (< TMP)


    public:

        // flush messages
        ~Logger() {
            flush();
        }

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

            //bool flush{ not _messages.empty() };
            //while ( not _messages.empty() )
            //{
            //    _out << _messages.front() << "\n";
            //    _messages.pop();
            //}
            //if (flush)
            //    _out.flush();
        }

    private:
        std::queue< std::string > _messages; 
        const std::uint32_t _flush_on_size = 32u;

        Output const& _out;

    protected:
        void* _owner = nullptr;
    };

}