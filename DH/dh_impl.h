#pragma once

#include <random>

namespace dh
{

    using namespace common;

    namespace
    {

        inline int_t random_int()
        {
            static std::random_device rd;
            static std::mt19937 rng(rd());
            static std::uniform_int_distribution<int_t> uni;
            return uni(rng);
        }

        inline int_t gcd(int_t a, int_t b, int &x, int &y)
        {
            if (a == 0)
            {
                x = 0; y = 1;
                return b;
            }
            int x1, y1;
            int_t d = gcd(b % a, a, x1, y1);
            x = y1 - int(b / a) * x1;
            y = x1;
            return d;
        }

        inline int_t gcd(int_t a, int_t b)
        {
            int x, y;
            return gcd(a, b, x, y);
        }

        inline int_t pows(long_t base, long_t exp, long_t modulus)
        {
            base %= modulus;
            long_t result = 1;
            while (exp > 0)
            {
                if (exp & 1) result = (result * base) % modulus;
                base = (base * base) % modulus;
                exp >>= 1;
            }
            return int_t(result);
        }

        inline bool is_prime_ferma(int_t x)
        {
            if (x < 2) return false;
            if (x == 2)
                return true;
            for (int i = 0; i < 100; i++)
            {
                int_t a = (random_int() % (x - 2)) + 2;
                if (gcd(a, x) != 1)
                    return false;
                if (pows(a, x - 1, x) != 1)
                    return false;
            }
            return true;
        }

        inline int_t factor_ferma(int_t n)
        {
            int_t x = int_t(sqrt(n)), y = 0;
            long long r = 1ll * x*x - 1ll * y*y - 1ll * n;
            for (;;)
            {
                if (r == 0)
                {
                    return x != y ? x - y : x + y;
                }
                else
                {
                    if (r > 0)
                    {
                        r -= y + y + 1;
                        ++y;
                    }
                    else
                    {
                        r += x + x + 1;
                        ++x;
                    }
                }
            }
        }

        inline void factorize(int_t n, int_t &m, std::vector<int_t> &factors)
        {
            if (!(n & 1))
            {
                factors.push_back(2);
                while (n && !((n >>= 1) & 1))
                {
                    m >>= 1;
                }
                if (!(m & 1))
                {
                    while (m && !((m >>= 1) & 1));
                }
            }
            while (n > 1)
            {
                int_t factor = factor_ferma(n);
                if (factor == 1)
                {
                    factors.push_back(n);
                    while (m && !((m /= n) % n));
                    break;
                }
                while (m && !((m /= factor) % factor));
                factorize(factor, n, factors);
            }
        }

        inline std::vector<int_t> factorize(int_t n)
        {
            std::vector<int_t> factors;
            int_t _zero = 0;
            factorize(n, _zero, factors);
            return factors;
        }

    }

    inline int_t random_prime()
    {
        int_t r;
        while(!is_prime_ferma(r = random_int()));
        return r;
    }

    inline int_t primitive_root(int_t p)
    {
        if (p == 2) return 1;
        int_t phi = p - 1;
        std::vector<int_t> factors = factorize(phi);
        for (int_t res = 2; res <= p; ++res)
        {
            bool ok = true;
            for (size_t i = 0; i < factors.size() && ok; ++i)
                ok &= (pows(res, phi / factors[i], p) != 1);
            if (ok) return res;
        }
        return 0;
    }
}