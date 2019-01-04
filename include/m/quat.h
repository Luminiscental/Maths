#ifndef __m_quat_h__
#define __m_quat_h__

#include <ostream>

#include <m/vec.h>

namespace m {

    template <typename T>
    class tquat;

    template <typename T>
    auto operator+(const tquat<T> &lhs, const tquat<T> &rhs);

    template <typename T>
    auto operator+(const tquat<T> &lhs, T rhs);

    template <typename T>
    auto operator+(T lhs, const tquat<T> &rhs);

    template <typename T>
    auto operator-(const tquat<T> &rhs);

    template <typename T>
    auto operator-(const tquat<T> &lhs, const tquat<T> &rhs);

    template <typename T>
    auto operator-(const tquat<T> &lhs, T rhs);

    template <typename T>
    auto operator-(T lhs, const tquat<T> &rhs);

    template <typename T>
    auto operator*(const tquat<T> &lhs, const tquat<T> &rhs);

    template <typename T>
    auto operator*(T lhs, const tquat<T> &rhs);

    template <typename T>
    auto operator*(const tquat<T> &lhs, T rhs);

    template <typename T>
    auto operator/(const tquat<T> &lhs, const tquat<T> &rhs);

    template <typename T>
    auto operator/(T lhs, const tquat<T> &rhs);

    template <typename T>
    auto operator/(const tquat<T> &lhs, T rhs);

    template <typename T>
    auto &operator<<(std::ostream &lhs, const tquat<T> &rhs);

    template <typename T>
    class tquat {

    private:

        T w;
        tvec<T, 3> ijk;

    public:

#define BINDING(name, value)    const auto & name () const; \
                                auto & name ();

        BINDING(real, w)
        BINDING(imaginary, ijk)
        BINDING(i, ijk.x())
        BINDING(j, ijk.y())
        BINDING(k, ijk.z())

#undef BINDING

        constexpr tquat() noexcept 
            :w(0), ijk(0, 0, 0) {}

        constexpr tquat(T a, T b, T c, T d) noexcept
            :w(a), ijk(b, c, d) {}

        constexpr tquat(T r) noexcept
            :w(r), ijk(0, 0, 0) {}

        constexpr tquat(const tvec<T, 3> &vector) noexcept
            :w(0), ijk(vector) {}

        constexpr tquat(T r, const tvec<T, 3> &vector) noexcept
            :w(r), ijk(vector) {}

        auto magnSqr() const noexcept;

        auto magn() const noexcept;

        constexpr auto conjugate() const noexcept {

            return tquat<T>(w, -ijk);
        }

        auto inverse() const;

        auto unit() const;

        auto rotate(const m::tvec<T, 3> &vector) const;

        static auto identity();

                                                               //                        /-
        static auto rotation(T angle, const tvec<T, 3> &axis); // NOTE: Right-handed: ---|--> axis
                                                               //                        \-> rotation

        auto &operator+=(const tquat<T> &rhs);

        auto &operator+=(T rhs);

        auto &operator-=(const tquat<T> &rhs);

        auto &operator-=(T rhs);

        auto &operator*=(const tquat<T> &rhs);

        auto &operator*=(T rhs);

        auto &operator/=(const tquat<T> &rhs);

        auto &operator/=(T rhs);
    };

    using iquat = tquat<int>;
    using lquat = tquat<long>;
    using  quat = tquat<float>;
    using dquat = tquat<double>;
}

#include <m/quat_impl.h>

#endif
