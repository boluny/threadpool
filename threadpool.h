#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <thread>
#include <vector>
#include <atomic>
#include <functional>
#include <future>
#include <utility>
#include <memory>
#include <type_traits>
#include "block_queue.h"

class threadpool
{
public:
    threadpool()
    : running_(true),
      queue_(10000)
    {
        // default initialize to real parallel number
        unsigned int n = std::thread::hardware_concurrency();
        for (unsigned int i = 0; i < n; ++i)
        {
            workers_.push_back(std::thread(&threadpool::worker_loop, this));
        }
    }

    threadpool(const threadpool&) = delete;
    threadpool& operator=(const threadpool&) = delete;

    ~threadpool()
    {
        running_ = false;
        for (auto& t: workers_)
        {
            t.join();
        }
    }

    // perfect forwarding here
    template<class R, class ...Args>
    std::future<std::result_of_t<R&&(Args&&...)>> submit(R&& func, Args&&... args)
    {
        using return_type = std::result_of_t<R&&(Args&&...)>;

        // magic here is to wrap the passed in params callable with lambda expression.
        // then wrap the labmda with packaged_task so we can get a future of return type
        // then wrap it to std::function<void()> to execute, as function itself and params 
        // are declared binded in the labmda mentioned above, so it can use `(*t)()` to call
        // the first lambda, the second lambda is queued and retrieved then execute within 
        // worker_loop, return values already saved in the future so it becomes ready when 
        // asynchoronize exection done in thread.
        
        // shared_ptr rather than unique_ptr here is because std::function must be 
        // CopyConstructible and CopyAssignable
        auto task = std::make_shared<std::packaged_task<return_type()>> (
                [&] () -> return_type { // `&` captures fuction parameter `func` and `args`
                    return func(args...);
                }
            );

        std::future<return_type> f = task->get_future();
        queue_.push(
            [t=std::move(task)]() {
                    (*t)();
            }
        );

        return f;
    }

private:
    void worker_loop()
    {
        // think it's a check point here?
        while (running_)
        {
            // TODO: need to add timeout for method try_pop
            // or it can't be terminated when no input functor
            auto task = queue_.try_pop();
            if (task) 
            {
                // use trait to check callable
                task();
            }
        }
    }

private:
    std::atomic<bool> running_;
    std::vector<std::thread> workers_;
    block_queue<std::function<void()> > queue_;
};

#endif //THREADPOOL_H
