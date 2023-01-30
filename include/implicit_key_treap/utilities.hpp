#pragma once

#include <limits>
#include <random>

namespace Utilities
{

template <std::integral T>
class DefaultRandomNumberGenerator
{
   public:
    DefaultRandomNumberGenerator()
        : m_device(std::random_device{})
        , m_engine(m_device())
        , m_distribution(std::uniform_int_distribution<T>{
            std::numeric_limits<T>::min(), std::numeric_limits<T>::max()}){};

    size_t operator()()
    {
        return m_distribution(m_engine);
    }

   private:
    std::random_device               m_device;
    std::default_random_engine       m_engine;
    std::uniform_int_distribution<T> m_distribution;
};

}  // namespace Utilities