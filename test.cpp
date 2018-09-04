#include <iostream>
#include <thread>
#include "block_queue.h"

void produce(block_queue<int>& queue_)
{
    std::cout << "producer started..." << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << '[' << std::this_thread::get_id() << ']'
            << "insert to queue: " << i << std::endl;
        queue_.push(i);
    }
}

void consume(block_queue<int>& queue_)
{
    std::cout << "consumer started..." << std::endl;
    while(true)
    {
        std::cout << '[' << std::this_thread::get_id() << ']'
            << "retrive from queue: " << queue_.pop() << std::endl;
    }
}

int main()
{
    block_queue<int> my_queue(5);
    std::cout << "hello" << std::endl;

    std::thread t1(produce, std::ref(my_queue));
    std::thread t2(consume, std::ref(my_queue));
    
    t1.join();
    t2.join();

    std::cout << "done" << std::endl;

    return 0;
}
