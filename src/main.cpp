#include "ArrayQueue.h"
#include "ArrayStack.h"
#include <iostream>

// act as test for DS implementations
// act as benchmarks for DS implementations

// DS TO IMPLEMENT
// queue as array
// linked list
// stack as array

// LockFree DS TO IMPLEMENT
//  queue as array
//  linked list
//  stack as array

// Refs
//  https://github.com/facebook/folly/blob/master/folly/ProducerConsumerQueue.h
//  https://www.youtube.com/watch?v=ZQFzMfHIxng&t=3871s
//  https://github.com/craflin/LockFreeQueue
//  https://github.com/dodng/fast_ring_queue
//  https://github.com/supermartian/lockfree-queue
//  https://github.com/kevinlynx/lockfree-list
//  https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/
//  https://www.youtube.com/watch?v=RBSGKlAvoiM

int main()
{
    constexpr int capacity = 5;
    ArrayQueue<int> queue(capacity);

    int sample;

    queue.Display();
    queue.Push(2);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Push(3);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Push(4);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Push(6);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Push(2);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Push(2);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Push(3);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Push(4);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Push(6);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Push(2);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Pop(sample);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Pop(sample);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Pop(sample);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Pop(sample);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Pop(sample);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    queue.Pop(sample);
    std::cout << "size: " << queue.Size() << '\n';

    queue.Display();
    std::cout << "size: " << queue.Size() << '\n';

    return 0;
}
