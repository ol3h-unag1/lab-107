import <iostream>;
import <syncstream>;

import <type_traits>;
import <concepts>;


import <condition_variable>;
import <thread>;
import <chrono>;

import type_name;

import Misc;

auto& sout = std::cout;

std::mutex mut;

std::condition_variable cv;

int i = 0;

void work()
{
    //std::osyncstream sout(std::cout);

    sout << std::this_thread::get_id() << " work: before locking.\n";
    std::unique_lock ul(mut);
    sout << std::this_thread::get_id() << " work: locked! waiting... (unlocked & suspended)\n";
    cv.wait(ul, [] 
        { 
            sout << std::this_thread::get_id() << " work: sleeping\n";
            return i == 1;
        });
    sout << std::this_thread::get_id() << " work: locked! finished waiting!\n";

}

void notify()
{
    //std::osyncstream sout(std::cout);

    sout << std::this_thread::get_id() << " notify: before sleep.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    {
        sout << std::this_thread::get_id() << " notify: before lock. \n";
        std::lock_guard lg(mut);
        sout << std::this_thread::get_id() << " notify: locked!\n";
    }

    sout << std::this_thread::get_id() << " notify: unlocked. NOTIFY ALL\n";
    cv.notify_all();

    sout << std::this_thread::get_id() << " notify: before sleep (again).\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    {
        sout << std::this_thread::get_id() << " notify: before lock (again). \n";
        std::lock_guard lg(mut);
        sout << std::this_thread::get_id() << " notify: locked! (again)\n";
        i = 1;
    }

    sout << std::this_thread::get_id() << " notify: unlocked! NOTIFY ALL (again)\n";
    cv.notify_all();
}

int main()
{
   //std::osyncstream sout(std::cout);

    sout << "main!\n";
    std::thread t1(work), t2(work), t3(work), t4(notify);
    sout << "main: threads created!\n";
    
    sout << t1.get_id() << " main: before joining!\n";
    t1.join();

    sout << t2.get_id() << " main: before joining!\n";
    t2.join();

    sout << t3.get_id() << " main: before joining!\n";
    t3.join();

    sout << t4.get_id() << " main: before joining!\n";
    t4.join();
}