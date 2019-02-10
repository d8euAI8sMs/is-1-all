#pragma once

namespace rsa
{

    using namespace common;

    namespace
    {

//         std::bitset<65536> is_not_prime_bs;
// 
//         inline void gen_erath()
//         {
//             const int m = 65536;
//             for (int i = 1; i < m / 2 + m % 2; i++) is_not_prime_bs[i] = false;
//             is_not_prime_bs[0] = true;
//             int end = (int) ceil(sqrt((double) m));
//             for (int i = 3; i <= end; i += 2)
//             {
//                 if (is_not_prime_bs[i / 2] == true) continue;
//                 for (int j = i * i; j <= m; j += i * 2)
//                 {
//                     is_not_prime_bs[j / 2] = true;
//                 }
//             }
//             return;
//         }


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
                int_t a = (rand() % (x - 2)) + 2;
                if (gcd(a, x) != 1)
                    return false;
                if (pows(a, x - 1, x) != 1)
                    return false;
            }
            return true;
        }
    }

    inline void random_keys(int_t &n, int_t &e, int_t &d)
    {
        const int_t max_pq = 65535;
        const int_t max_att = 100;
        int_t att = 0;
        int_t p, q, phi;
        int t, d1;
        do
        {
            p = (rand() + rand() + rand() + rand()) % (max_pq + 1);
        } while (!is_prime_ferma(p));
        int_t a = p / 10;
        int_t b = p * 2;
        if (b > max_pq)
        {
            a = ((b - max_pq) < a) ? (a - (b - max_pq)) : 0;
            b = max_pq;
        }
        do
        {
            q = (((rand() + rand() + rand() + rand()) % (b - a + 1))) + a;
            if (++att > max_att)
            {
                random_keys(n, e, d);
                return;
            }
        } while (!is_prime_ferma(q) || q == p || q*p < 256);

        att = 0;

        n = p * q;
        phi = (p - 1) * (q - 1);
        do
        {
            e = (rand() + rand() + rand() + rand()) % (phi + 1);
            if (++att > max_att)
            {
                random_keys(n, e, d);
                return;
            }
        } while ((gcd(e, phi, d1, t) != 1) || e < 3);

        d = int_t(d1 < 0 ? d1 + phi : d1);

        for (size_t i = 0; i < max_att; i++)
        {
            p = rand(); q = rand(); phi = rand();
        }
    }
}