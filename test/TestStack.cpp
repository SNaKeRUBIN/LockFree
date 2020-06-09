#include "ArrayStack.h"
#include <array>
#include <iostream>
#include <set>
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

void Produce(ArrayStack<int>& stack, const std::array<int, capacity>& arr)
{
    for (auto val : arr)
    {
        CheckReturnTrue(stack.Push(std::move(val)));
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ns);
    }
}

void Consume(ArrayStack<int>& stack, const std::array<int, capacity>& arr)
{
    int retVal;
    std::set<int> setLocal(std::cbegin(arr), std::cend(arr));
    for (size_t i = 0; i < capacity; ++i)
    {
        while (!stack.Pop(retVal))
            ;
        setLocal.erase(retVal);
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ns);
    }
    CheckReturnTrue(setLocal.empty());
}

void TestSingleThreaded()
{
    ArrayStack<int> stack(capacity);
    [[maybe_unused]] int retVal;

    CheckReturnTrue(stack.SizeGuess() == 0);

    CheckReturnTrue(stack.Push(0));
    CheckReturnTrue(stack.SizeGuess() == 1);

    CheckReturnTrue(stack.Push(1));
    CheckReturnTrue(stack.SizeGuess() == 2);

    CheckReturnTrue(stack.Pop(retVal));
    CheckReturnTrue(stack.SizeGuess() == 1);

    CheckReturnTrue(stack.Pop(retVal));
    CheckReturnTrue(stack.SizeGuess() == 0);

    // pop fails when emtpy
    CheckReturnFalse(stack.Pop(retVal));
    CheckReturnTrue(stack.SizeGuess() == 0);

    // push fails once stack is full
    for (int i = 1; i <= capacity; ++i)
    {
        // move degrades to copy for standard types
        CheckReturnTrue(stack.Push(std::move(i)));
        CheckReturnTrue(stack.SizeGuess() == i);
    }
    CheckReturnFalse(stack.Push(0));

    // pop fails once stack is empty
    for (int i = capacity; i > 0; --i)
    {
        CheckReturnTrue(stack.SizeGuess() == i);
        CheckReturnTrue(stack.Pop(retVal));
    }
    CheckReturnTrue(stack.SizeGuess() == 0);
    CheckReturnFalse(stack.Pop(retVal));

    // chk order is maintained
    std::array<int, capacity> arr = {4, 8, 6, 9, 0};
    for (auto val : arr)
    {
        stack.Push(std::move(val));
    }
    for (int idx = capacity - 1; idx >= 0; --idx)
    {
        CheckReturnTrue(stack.Pop(retVal));
        CheckReturnTrue(arr[idx] == retVal);
    }
}

void TestMultiThreaded()
{
    ArrayStack<int> stack(capacity);
    std::array<int, capacity> arr = {4, 8, 6, 9, 0};

    auto producerThread = std::thread(&Produce, std::ref(stack), arr);
    auto consumerThread = std::thread(&Consume, std::ref(stack), arr);

    producerThread.join();
    consumerThread.join();
}
} // namespace

int main()
{
    TestSingleThreaded();
    std::cout << "Single Threaded : Passed\n";

    TestMultiThreaded();
    std::cout << "Multi Threaded : Passed\n";

    return 0;
}
