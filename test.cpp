#include <iostream>
#include <thread>
#include "block_queue.h"
#include "threadpool.h"

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

int sum(int a, int b)
{
    return a + b;
}

void add_to(int& a, int b)
{
    a += b;
}

int main()
{
    block_queue<int> my_queue(5);
    std::cout << "hello" << std::endl;

   // std::thread t1(produce, std::ref(my_queue));
   // std::thread t2(consume, std::ref(my_queue));
   // 
   // t1.join();
   // t2.join();
   
    threadpool pool;
    auto ret1 = pool.submit(sum, 3, 4);
    std::cout << "get sum: " << ret1.get() << std::endl;

    auto ret2 = pool.submit([] () { return 42; });
    std::cout << "god themselves: " << ret2.get() << std::endl;

    int b = 5;
    auto ret3 = pool.submit(add_to, std::ref(b), 8);
    ret3.get();
    std::cout << "The end of Eternity: " << b << std::endl;

    std:: cout << "No more functors to queue" << std::endl;

    std::cout << "done" << std::endl;

    

    return 0;
}
