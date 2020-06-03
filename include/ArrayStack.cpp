#include <vector>

template <class T>
class ArrayQueue
{
  public:
    ArrayQueue(const unsigned capacity = 5) : m_data(capacity), m_idx(-1)
    {
        // assert(capacity > 0);
    }

    bool Push(const T val)
    {
        if (m_idx == m_data.size() - 1)
        {
            return false;
        }

        m_data.at(m_idx++) = val;
        return true;
    }

    bool Pop(T& val)
    {
        if (m_idx == -1)
        {
            return false;
        }

        val = m_data.at(val);
        m_idx--;
        return true;
    }

  private:
    std::vector<T> m_data;
    int m_idx;
};
