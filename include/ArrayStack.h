#include <atomic>
#include <type_traits>
#include <vector>

// Single Producer Single Consumer Lockfree stack

template <class T>
class ArrayStack
{
  public:
    explicit ArrayStack(const unsigned capacity = 5)
        : m_data(std::max<unsigned>(capacity, 1)), m_idx(-1)
    {
        static_assert(std::is_copy_assignable_v<T> &&
                      std::is_move_assignable_v<T>);
    }

    ~ArrayStack() = default;

    ArrayStack(const ArrayStack&) = delete;
    ArrayStack& operator=(const ArrayStack&) = delete;

    ArrayStack(ArrayStack&&) = delete;
    ArrayStack& operator=(ArrayStack&&) = delete;

    bool Push(T&& val)
    {
        const auto curIdx = m_idx.load(std::memory_order_relaxed); // READ
        if (curIdx == m_data.size() - 1)
        {
            return false;
        }

        const auto nextIdx = curIdx + 1;
        m_data.at(nextIdx) = val; // COPY

        m_idx.store(nextIdx, std::memory_order_release); // UPDATE
        return true;
    }

    bool Pop(T& val)
    {
        const auto curIdx = m_idx.load(std::memory_order_acquire); // READ
        if (curIdx == -1)
        {
            return false;
        }

        val = std::move(m_data.at(curIdx)); // COPY

        const auto prevIdx = curIdx - 1;
        m_idx.store(prevIdx, std::memory_order_relaxed); // UPDATE
        return true;
    }

    bool Seek(T&& val)
    {
        const auto curIdx = m_idx.load(std::memory_order_acquire);
        if (curIdx == -1)
        {
            return false;
        }
        val = m_data.at(curIdx);
        return true;
    }

    size_t SizeGuess() const
    {
        return m_idx.load(std::memory_order_relaxed) + 1;
    }

    size_t Capacity() const { return m_data.size(); }

  private:
    using AtomicIndex = std::atomic<int>;

    std::vector<T> m_data;
    alignas(std::hardware_destructive_interference_size) AtomicIndex m_idx;
    char cacheLinePad[std::hardware_destructive_interference_size -
                      sizeof(AtomicIndex)];
};
