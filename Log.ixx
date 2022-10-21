export module Log;

import <exception>;
import <queue>;
import <string>;

export namespace Log {

    template< class Output  >
    class Logger final
    {
    public:

        // out must outlive this please
        Logger(Output& out) 
            : _out( out ) {
        }

        // no default please
        Logger() = delete;

        // no any copy/assign ops please
        Logger(Logger&) = delete;

        // flush messages
        ~Logger() {
            flush();
        }

        // adds messages to queue
        void log(std::string msg) {
            _messages.emplace(std::move(msg));
        }

        // adds messages to queue
        Logger& operator<<(std::string msg) {
            log(std::move(msg));
            return *this;;
        }

        // outputs messages queue
        void flush() {
            bool flush{ not _messages.empty() };
            while ( not _messages.empty() )
            {
                _out << _messages.front() << "\n";
                _messages.pop();
            }
            if (flush)
                _out.flush();
        }

    private:
        std::queue< std::string > _messages;

        Output& _out;
    };



}