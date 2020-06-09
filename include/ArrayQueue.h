#include <algorithm>
#include <atomic>
#include <iostream>
#include <type_traits>
#include <vector>

// Single Producer Single Consumer Lockfree Queue

template <class T>
class ArrayQueue
{
  public:
    explicit ArrayQueue(const unsigned capacity)
        : m_data(std::max<unsigned>(capacity, 1)), m_readIndex(0),
          m_writeIndex(0), m_isFull(false)
    {
        static_assert(std::is_copy_assignable_v<T> &&
                      std::is_move_assignable_v<T>);
    }

    ~ArrayQueue() = default;

    ArrayQueue(const ArrayQueue&) = delete;
    ArrayQueue& operator=(const ArrayQueue&) = delete;

    ArrayQueue(ArrayQueue&&) = delete;
    ArrayQueue& operator=(ArrayQueue&&) = delete;

    bool Push(T&& val)
    {
        if (m_isFull.load(std::memory_order_relaxed)) // READ
        {
            return false;
        }
        const auto curWriteIdx =
            m_writeIndex.load(std::memory_order_relaxed); // READ
        m_data.at(curWriteIdx) = val;                     // COPY

        const auto nextIdx =
            (curWriteIdx + 1 == m_data.size()) ? 0 : curWriteIdx + 1;
        m_writeIndex.store(nextIdx, std::memory_order_release); // UPDATE

        if (nextIdx == m_readIndex.load(std::memory_order_relaxed))
        {
            m_isFull.store(true, std::memory_order_relaxed); // UPDATE
        }
        return true;
    }

    bool Pop(T& val)
    {
        const auto curWriteIdx = m_writeIndex.load(std::memory_order_acquire);
        const auto curReadIdx =
            m_readIndex.load(std::memory_order_relaxed); // READ
        if (curReadIdx == curWriteIdx &&
            !m_isFull.load(std::memory_order_relaxed))
        {
            return false;
        }

        val = m_data.at(curReadIdx); // COPY

        const auto nextReadIdx =
            (curReadIdx + 1 == m_data.size()) ? 0 : curReadIdx + 1;
        m_readIndex.store(nextReadIdx, std::memory_order_relaxed); // UPDATE
        if (m_isFull.load(std::memory_order_relaxed))              // READ
        {
            m_isFull.store(false, std::memory_order_relaxed); // UPDATE
        }
        return true;
    }

    size_t SizeGuess() const
    {
        if (m_isFull.load(std::memory_order_relaxed))
        {
            return m_data.size();
        }
        int size =
            static_cast<int>(m_writeIndex.load(std::memory_order_relaxed)) -
            static_cast<int>(m_readIndex.load(std::memory_order_relaxed));

        if (size < 0)
        {
            size += static_cast<int>(m_data.size());
        }
        return static_cast<size_t>(size);
    }

    size_t Capacity() const { return m_data.size(); }

    void Display() const
    {
        if (m_isFull)
        {
            bool isStart = true;
            for (int i = m_readIndex; i != m_writeIndex || isStart;)
            {
                isStart = false;
                std::cout << m_data.at(i) << " ";
                i++;
                i %= m_data.size();
            }
        }
        else
        {
            for (int i = m_readIndex; i != m_writeIndex;)
            {
                std::cout << m_data.at(i) << " ";
                ++i;
                i %= m_data.size();
            }
        }
        std::cout << '\n';
    }

  private:
    using AtomicIndex = std::atomic<unsigned>;

    std::vector<T> m_data;
    alignas(std::hardware_destructive_interference_size)
        AtomicIndex m_readIndex;
    char cacheLinePad1[std::hardware_destructive_interference_size -
                       sizeof(AtomicIndex)];
    alignas(std::hardware_destructive_interference_size)
        AtomicIndex m_writeIndex;
    char cacheLinePad2[std::hardware_destructive_interference_size -
                       sizeof(AtomicIndex)];
    alignas(
        std::hardware_destructive_interference_size) std::atomic_bool m_isFull;
    char cacheLinePad3[std::hardware_destructive_interference_size -
                       sizeof(std::atomic_bool)];
};
