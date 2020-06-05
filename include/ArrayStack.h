#include <atomic>
#include <type_traits>
#include <vector>

template <class T>
class ArrayStack
{
  public:
    ArrayStack(const unsigned capacity = 5) : m_data(capacity), m_idx(-1)
    {
        static_assert(std::is_copy_assignable_v<T> &&
                      std::is_move_assignable_v<T>);

        if (capacity <= 0)
        {
            throw std::runtime_error(
                "ArrayStack capacity must be greater than 0");
        }
    }

    bool Push(const T&& val)
    {
        const auto curIdx = m_idx.load(std::memory_order_relaxed);
        if (curIdx == m_data.size() - 1)
        {
            return false;
        }

        const auto nextIdx = curIdx + 1;
        m_data.at(nextIdx) = val;

        m_idx.store(nextIdx, std::memory_order_release);
        return true;
    }

    bool Pop(T& val)
    {
        const auto curIdx = m_idx.load(std::memory_order_acquire);
        if (curIdx == -1)
        {
            return false;
        }

        val = std::move(m_data.at(curIdx));

        const auto prevIdx = curIdx - 1;
        m_idx.store(prevIdx, std::memory_order_release);
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

    bool Resize(const unsigned capacity)
    {
        m_data.resize(capacity);
        m_idx.store(-1, std::memory_order_release);
    }

  private:
    using AtomicIndex = std::atomic<int>;

    std::vector<T> m_data;
    alignas(std::hardware_destructive_interference_size) AtomicIndex m_idx;
    char cacheLinePad[std::hardware_destructive_interference_size -
                      sizeof(AtomicIndex)];
};
