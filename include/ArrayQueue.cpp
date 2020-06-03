#include <iostream>
#include <vector>

template <class T>
class ArrayQueue
{
  public:
    ArrayQueue(const unsigned capacity)
        : m_data(capacity), m_readIndex(0), m_writeIndex(0), m_isFull(false)
    {
        // assert(capacity > 0);
    }

    bool Push(const T val)
    {
        if (m_isFull)
        {
            return false;
        }
        m_data.at(m_writeIndex) = val;
        m_writeIndex++;
        m_writeIndex %= m_data.size();
        if (m_writeIndex == m_readIndex)
        {
            m_isFull = true;
        }
        return true;
    }

    bool Pop(T& val)
    {
        if (m_readIndex == m_writeIndex && !m_isFull)
        {
            return false;
        }
        val = m_data.at(m_readIndex);
        m_readIndex++;
        m_readIndex %= m_data.size();
        if (m_isFull)
        {
            m_isFull = false;
        }
        return true;
    }

    unsigned Size() { return 0; }

    unsigned Capacity() { return m_data.size(); }

    // this resets the queue to given size
    bool Resize(const unsigned capacity)
    {
        m_readIndex = 0;
        m_writeIndex = 0;

        return m_data.resize();
    }

    void Display()
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
    std::vector<T> m_data;
    unsigned m_readIndex;
    unsigned m_writeIndex;
    bool m_isFull;
};
