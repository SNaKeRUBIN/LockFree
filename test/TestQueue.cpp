#include "ArrayQueue.h"
#include <array>
#include <iostream>
#include <numeric>
#include <thread>

namespace
{
constexpr unsigned capacity = 5;

void CheckReturnTrue(const bool val)
{
    if (!val)
    {
        throw std::runtime_error("Condition Failed");
    }
}

void CheckReturnFalse(const bool val) { CheckReturnTrue(!val); }

void Produce(ArrayQueue<int>& queue,
             const std::array<int, capacity * capacity>& arr)
{
    for (auto val : arr)
    {
        while (!queue.Push(std::move(val)))
            ;
    }
}

void Consume(ArrayQueue<int>& queue,
             const std::array<int, capacity * capacity>& arr)
{
    int retVal;
    for (auto val : arr)
    {
        while (!queue.Pop(retVal))
            ;
        CheckReturnTrue(val == retVal);
    }
}

void TestSingleThreaded()
{
    ArrayQueue<int> queue(capacity);
    [[maybe_unused]] int retVal;

    CheckReturnTrue(queue.SizeGuess() == 0);

    CheckReturnTrue(queue.Push(0));
    CheckReturnTrue(queue.SizeGuess() == 1);

    CheckReturnTrue(queue.Push(1));
    CheckReturnTrue(queue.SizeGuess() == 2);

    CheckReturnTrue(queue.Pop(retVal));
    CheckReturnTrue(queue.SizeGuess() == 1);

    CheckReturnTrue(queue.Pop(retVal));
    CheckReturnTrue(queue.SizeGuess() == 0);

    // Pop fails when empty
    CheckReturnFalse(queue.Pop(retVal));
    CheckReturnTrue(queue.SizeGuess() == 0);

    // push fails once queue is full
    for (int i = 1; i <= capacity; ++i)
    {
        // move degrades to copy for standard types
        CheckReturnTrue(queue.Push(std::move(i)));
        CheckReturnTrue(queue.SizeGuess() == i);
    }
    CheckReturnFalse(queue.Push(0));

    // pop fails once queue is empty
    for (int i = capacity; i > 0; --i)
    {
        CheckReturnTrue(queue.SizeGuess() == i);
        CheckReturnTrue(queue.Pop(retVal));
    }
    CheckReturnTrue(queue.SizeGuess() == 0);
    CheckReturnFalse(queue.Pop(retVal));

    // chk order is maintained
    std::array<int, capacity> arr = {4, 8, 6, 9, 0};
    for (auto& val : arr)
    {
        queue.Push(std::move(val));
    }
    for (auto& val : arr)
    {
        CheckReturnTrue(queue.Pop(retVal));
        CheckReturnTrue(val == retVal);
    }
}

void TestMultiThreaded()
{
    ArrayQueue<int> queue(capacity);
    std::array<int, capacity * capacity> arr;
    std::iota(std::begin(arr), std::end(arr), 0);

    auto producerThread = std::thread(&Produce, std::ref(queue), arr);
    auto consumerThread = std::thread(&Consume, std::ref(queue), arr);

    producerThread.join();
    consumerThread.join();
}
} // namespace

int main()
{
    TestSingleThreaded();
    std::cout << "Single Threaded: Passed\n";

    TestMultiThreaded();
    std::cout << "Multi Threaded: Passed\n";

    return 0;
}
