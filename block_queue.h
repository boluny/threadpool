#include <queue>
#include <mutex>
#include <condition_variable>

/// A bounded thread-safe blocking queue
template <typename T>
class block_queue
{
public:
    block_queue() = delete;
    block_queue(const block_queue&) = delete;
    block_queue& operator=(const block_queue&) = delete;

    block_queue(std::size_t size)
        : max_size_(size)
    {}

    void push(const T& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_can_push_.wait(lock, [this]() { return queue_.size() < max_size_; });
        queue_.push(std::move(value));
        cv_can_pop_.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_can_pop_.wait(lock, [this]() {return !queue_.empty();});
        auto val = std::move(queue_.front());
        queue_.pop();
        cv_can_push_.notify_one();
        return val;
    }


private:
    std::mutex mutex_;
    std::condition_variable cv_can_push_;
    std::condition_variable cv_can_pop_;
    std::queue<T> queue_;
    std::size_t max_size_;

};

