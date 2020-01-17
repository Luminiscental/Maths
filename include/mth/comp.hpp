#pragma once

/// @file mth/comp.hpp Header containing definitions for complex number
/// expressions.

#include <array>
#include <cmath>
#include <cstdint>
#include <ostream>

#include <mth/mth.hpp>

namespace mth
{
    template <typename Derived>
    class tcomp_expr;

    template <typename Comp>
    class tcomp_info;

    /**
     * @brief Alias for the element type of a given complex number expression
     * type.
     *
     * @tparam Comp The complex number expression type.
     */
    template <typename Comp>
    using tcomp_elem_t = typename tcomp_info<Comp>::Elem;

    template <typename... Ts>
    using enable_if_comps_t =
        std::enable_if_t<((std::is_base_of_v<tcomp_expr<Ts>, Ts>) &&...)>;

    template <typename T>
    using enable_if_comp_t = enable_if_comps_t<T>;

    /**
     * @brief Complex number expression for a concrete / evaluated complex
     * value.
     *
     * Various aliases for this type are provided with the typical type prefixes
     * and default types: `icomp` for `tcomp<int>`, `dcomp` for `tcomp<double>`,
     * `ucomp` for `tcomp<unsigned int>`, `fcomp` for `tcomp<float>`. Leaving
     * out the type prefix defaults to float, so `comp` is `fcomp`;
     *
     * This class also implements the compound assignment operators that
     * `tcomp_expr` does not since operations on `tcomp_expr` change the type so
     * cannot be used in compound assignment.
     *
     * @tparam T The scalar type for the real and imaginary part of the value.
     */
    template <typename T>
    class tcomp : public tcomp_expr<tcomp<T>>
    {
    private:
        T _real;
        T _imag;

    public:
        /**
         * @brief Default constructor calls explicit default initialization for
         * the real and imaginary parts.
         *
         * Enabled when the element type `T` is default constructible.
         */
        template <
            typename = std::enable_if_t<std::is_default_constructible_v<T>>>
        constexpr tcomp() : _real{}, _imag{}
        {
        }

        /**
         * @brief Convert a complex number expression to a concrete complex
         * value.
         *
         * Enabled when `Comp` is derived from `mth::tcomp_expr` appropriately.
         *
         * @tparam Comp The complex number expression type.
         * @param expr The complex number expression value.
         */
        template <
            typename Comp,
            typename = enable_if_comp_t<Comp>,
            typename = std::enable_if_t<std::is_same_v<T, tcomp_elem_t<Comp>>>>
        constexpr tcomp(Comp expr) : _real{expr.real()}, _imag{expr.imag()}
        {
        }

        // Compound assignment operators:

        /**
         * @brief Compound assignment operator for addition, adds the given
         * complex number expression to this complex number.
         *
         * @tparam Vec The complex number expression type to add.
         * @param other The complex number expression value to add.
         */
        template <
            typename Comp,
            typename = enable_if_comp_t<Comp>,
            typename = std::enable_if_t<std::is_same_v<tcomp_elem_t<Comp>, T>>>
        constexpr tcomp<T>& operator+=(Comp other);

        /**
         * @brief Compound assignment operator for subtraction, subtracts the
         * given complex number expression from this complex number.
         *
         * @tparam Vec The complex number expression type to subtract.
         * @param other The complex number expression value to subtract.
         */
        template <
            typename Comp,
            typename = enable_if_comp_t<Comp>,
            typename = std::enable_if_t<std::is_same_v<tcomp_elem_t<Comp>, T>>>
        constexpr tcomp<T>& operator-=(Comp other);

        /**
         * @brief Compound assignment operator for multiplication, multiplies
         * this complex number by the given complex number expression.
         *
         * @tparam Vec The complex number expression type to multiply by.
         * @param other The complex number expression value to multiply by.
         */
        template <
            typename Comp,
            typename = enable_if_comp_t<Comp>,
            typename = std::enable_if_t<std::is_same_v<tcomp_elem_t<Comp>, T>>>
        constexpr tcomp<T>& operator*=(Comp other);

        /**
         * @brief Compound assignment operator for division, divides
         * this complex number by the given complex number expression.
         *
         * @tparam Vec The complex number expression type to divide by.
         * @param other The complex number expression value to divide by.
         */
        template <
            typename Comp,
            typename = enable_if_comp_t<Comp>,
            typename = std::enable_if_t<std::is_same_v<tcomp_elem_t<Comp>, T>>>
        constexpr tcomp<T>& operator/=(Comp other);

        /**
         * @brief Compound assignment operator for scalar addition, adds the
         * given scalar value to this complex number.
         *
         * @param scalar The scalar value to add.
         */
        constexpr tcomp<T>& operator+=(T scalar);

        /**
         * @brief Compound assignment operator for scalar subtraction, subtracts
         * the given scalar value to this complex number.
         *
         * @param scalar The scalar value to subtract.
         */
        constexpr tcomp<T>& operator-=(T scalar);

        /**
         * @brief Compound assignment operator for scalar multiplication,
         * multiplies this complex number by the given scalar value.
         *
         * @param scalar The scalar value to multiply by.
         */
        constexpr tcomp<T>& operator*=(T scalar);

        /**
         * @brief Compound assignment operator for scalar division,
         * divides this complex number by the given scalar value.
         *
         * @param scalar The scalar value to divide by.
         */
        constexpr tcomp<T>& operator/=(T scalar);

        /**
         * @brief Construct a concrete complex number given its real and
         * imaginary parts.
         *
         * @param real The real part.
         * @param imag The imaginary part.
         */
        constexpr tcomp(T real, T imag)
            : _real{std::move(real)}, _imag{std::move(imag)}
        {
        }

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The real part of this complex value.
         */
        constexpr T real() const
        {
            return _real;
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The imaginary part of this complex value.
         */
        constexpr T imag() const
        {
            return _imag;
        }
    };

    template <typename Comp, typename = enable_if_comp_t<Comp>>
    tcomp(Comp expr)->tcomp<tcomp_elem_t<Comp>>;

    template <typename T>
    class tcomp_info<tcomp<T>>
    {
    public:
        using Elem = T;
    };

    /**
     * @brief Complex number expression for a sum of two complex numbers.
     *
     * @tparam Lhs The complex number expression type of the left side operand.
     * @tparam Rhs The complex number expression type of the right side operand.
     */
    template <typename Lhs, typename Rhs>
    class tcomp_sum : public tcomp_expr<tcomp_sum<Lhs, Rhs>>
    {
    private:
        Lhs _lhs;
        Rhs _rhs;

    public:
        /**
         * @brief Construct the sum expression from its operands.
         *
         * @param lhs The complex number expression of the left side operand.
         * @param rhs The complex number expression of the right side operand.
         */
        explicit constexpr tcomp_sum(Lhs lhs, Rhs rhs)
            : _lhs{std::move(lhs)}, _rhs{std::move(rhs)}
        {
        }

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The sum of the real parts of the left and right side
         * operands.
         */
        constexpr tcomp_elem_t<Lhs> real() const
        {
            return _lhs.real() + _rhs.real();
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The sum of the imaginary parts of the left and right side
         * operands.
         */
        constexpr tcomp_elem_t<Lhs> imag() const
        {
            return _lhs.imag() + _rhs.imag();
        }
    };

    template <typename Lhs, typename Rhs>
    class tcomp_info<tcomp_sum<Lhs, Rhs>>
    {
    public:
        using Elem = tcomp_elem_t<Lhs>;
    };

    /**
     * @brief Complex number expression for the negation of a complex number.
     *
     * @tparam Comp The complex number expression type of the operand.
     */
    template <typename Comp>
    class tcomp_neg : public tcomp_expr<tcomp_neg<Comp>>
    {
    private:
        Comp _target;

    public:
        /**
         * @brief Construct the negation expression from its operand.
         *
         * @param comp The complex number expression to negate.
         */
        explicit constexpr tcomp_neg(Comp comp) : _target{std::move(comp)} {}

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The negation of the real part of the operand.
         */
        constexpr tcomp_elem_t<Comp> real() const
        {
            return -_target.real();
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The negation of the imaginary part of the operand.
         */
        constexpr tcomp_elem_t<Comp> imag() const
        {
            return -_target.imag();
        }
    };

    template <typename Comp>
    class tcomp_info<tcomp_neg<Comp>>
    {
    public:
        using Elem = tcomp_elem_t<Comp>;
    };

    /**
     * @brief Complex number expression for the conjugation of a complex number.
     *
     * @tparam Comp The complex number expression type of the operand.
     */
    template <typename Comp>
    class tcomp_conj : public tcomp_expr<tcomp_conj<Comp>>
    {
    private:
        Comp _target;

    public:
        /**
         * @brief Construct the conjugation expression from its operand.
         *
         * @param comp The complex number expression to conjugate.
         */
        explicit constexpr tcomp_conj(Comp comp) : _target{std::move(comp)} {}

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The real part of the operand unchanged.
         */
        constexpr tcomp_elem_t<Comp> real() const
        {
            return _target.real();
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * numer expresison.
         *
         * @return The negation of the imaginary part of the operand.
         */
        constexpr tcomp_elem_t<Comp> imag() const
        {
            return -_target.imag();
        }
    };

    template <typename Comp>
    class tcomp_info<tcomp_conj<Comp>>
    {
    public:
        using Elem = tcomp_elem_t<Comp>;
    };

    /**
     * @brief Complex number expression for a difference of two complex numbers.
     *
     * @tparam Lhs The complex number expression type of the left side operand.
     * @tparam Rhs The complex number expression type of the right side operand.
     */
    template <typename Lhs, typename Rhs>
    class tcomp_diff : public tcomp_expr<tcomp_diff<Lhs, Rhs>>
    {
    private:
        Lhs _lhs;
        Rhs _rhs;

    public:
        /**
         * @brief Construct the difference expression from its operands.
         *
         * @param lhs The complex number expression of the left side operand.
         * @param rhs The complex number expression of the right side operand.
         */
        explicit constexpr tcomp_diff(Lhs lhs, Rhs rhs)
            : _lhs{std::move(lhs)}, _rhs{std::move(rhs)}
        {
        }

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The difference of the real parts of the left and right side
         * operands.
         */
        constexpr tcomp_elem_t<Lhs> real() const
        {
            return _lhs.real() - _rhs.real();
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The difference of the imaginary parts of the left and right
         * side operands.
         */
        constexpr tcomp_elem_t<Lhs> imag() const
        {
            return _lhs.imag() - _rhs.imag();
        }
    };

    template <typename Lhs, typename Rhs>
    class tcomp_info<tcomp_diff<Lhs, Rhs>>
    {
    public:
        using Elem = tcomp_elem_t<Lhs>;
    };

    /**
     * @brief Complex number expression for a scalar-complex sum.
     *
     * @tparam Comp The complex number expression type.
     */
    template <typename Comp>
    class tcomp_shiftr : public tcomp_expr<tcomp_shiftr<Comp>>
    {
    private:
        Comp _comp;
        tcomp_elem_t<Comp> _scalar;

    public:
        /**
         * @brief Construct the sum expression from its operands.
         *
         * @param comp The complex number expression.
         * @param scalar The scalar to add.
         */
        explicit constexpr tcomp_shiftr(Comp comp, tcomp_elem_t<Comp> scalar)
            : _comp{std::move(comp)}, _scalar{std::move(scalar)}
        {
        }

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The real part of the sum of the operands.
         */
        constexpr tcomp_elem_t<Comp> real() const
        {
            return _scalar + _comp.real();
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The imaginary part of the sum of the operands.
         */
        constexpr tcomp_elem_t<Comp> imag() const
        {
            return _comp.imag();
        }
    };

    template <typename Comp>
    class tcomp_info<tcomp_shiftr<Comp>>
    {
    public:
        using Elem = tcomp_elem_t<Comp>;
    };

    /**
     * @brief Complex number expression for a scalar-complex difference (scalar
     * subtracted from complex).
     *
     * @tparam Comp The complex number expression type.
     */
    template <typename Comp>
    class tcomp_shiftl : public tcomp_expr<tcomp_shiftl<Comp>>
    {
    private:
        Comp _comp;
        tcomp_elem_t<Comp> _scalar;

    public:
        /**
         * @brief Construct the difference expression from its operands.
         *
         * @param comp The complex number expression.
         * @param scalar The scalar to subtract.
         */
        explicit constexpr tcomp_shiftl(Comp comp, tcomp_elem_t<Comp> scalar)
            : _comp{std::move(comp)}, _scalar{std::move(scalar)}
        {
        }

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The real part of the difference of the operands.
         */
        constexpr tcomp_elem_t<Comp> real() const
        {
            return _comp.real() - _scalar;
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The imaginary part of the difference of the operands.
         */
        constexpr tcomp_elem_t<Comp> imag() const
        {
            return _comp.imag();
        }
    };

    template <typename Comp>
    class tcomp_shiftl<tcomp_shiftl<Comp>>
    {
    public:
        using Elem = tcomp_elem_t<Comp>;
    };

    /**
     * @brief Complex number expression for a complex-scalar difference (complex
     * subtracted from scalar).
     *
     * @tparam Comp The complex number expression type.
     */
    template <typename Comp>
    class tcomp_refl : public tcomp_expr<tcomp_refl<Comp>>
    {
    private:
        Comp _comp;
        tcomp_elem_t<Comp> _scalar;

    public:
        /**
         * @brief Construct the difference expression from its operands.
         *
         * @param comp The complex number expression.
         * @param scalar The scalar to subtract from.
         */
        explicit constexpr tcomp_refl(Comp comp, tcomp_elem_t<Comp> scalar)
            : _comp{std::move(comp)}, _scalar{std::move(scalar)}
        {
        }

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The real part of the difference of the operands.
         */
        constexpr tcomp_elem_t<Comp> real() const
        {
            return _scalar - _comp.real();
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The imaginary part of the difference of the operands.
         */
        constexpr tcomp_elem_t<Comp> imag() const
        {
            return _comp.imag();
        }
    };

    template <typename Comp>
    class tcomp_info<tcomp_refl<Comp>>
    {
    public:
        using Elem = tcomp_elem_t<Comp>;
    };

    /**
     * @brief Complex number expression for a scalar-complex product.
     *
     * @tparam Comp The complex number expression type.
     */
    template <typename Comp>
    class tcomp_scale : public tcomp_expr<tcomp_scale<Comp>>
    {
    private:
        Comp _comp;
        tcomp_elem_t<Comp> _scalar;

    public:
        /**
         * @brief Construct the product expression from its operands.
         *
         * @param comp The complex number expression.
         * @param scalar The scalar to multiply by.
         */
        explicit constexpr tcomp_scale(Comp comp, tcomp_elem_t<Comp> scalar)
            : _comp{std::move(comp)}, _scalar{std::move(scalar)}
        {
        }

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The real part of the product of the operands.
         */
        constexpr tcomp_elem_t<Comp> real() const
        {
            return _scalar * _comp.real();
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The imaginary part of the product of the operands.
         */
        constexpr tcomp_elem_t<Comp> imag() const
        {
            return _scalar * _comp.imag();
        }
    };

    template <typename Comp>
    class tcomp_info<tcomp_scale<Comp>>
    {
    public:
        using Elem = tcomp_elem_t<Comp>;
    };

    /**
     * @brief Complex number expression for a complex / scalar quotient.
     *
     * @tparam Comp The complex number expression type.
     */
    template <typename Comp>
    class tcomp_reduce : public tcomp_expr<tcomp_reduce<Comp>>
    {
    private:
        Comp _comp;
        tcomp_elem_t<Comp> _scalar;

    public:
        /**
         * @brief Construct the quotient expression from its operands.
         *
         * @param comp The complex number expression.
         * @param scalar The scalar to multiply by.
         */
        explicit constexpr tcomp_reduce(Comp comp, tcomp_elem_t<Comp> scalar)
            : _comp{std::move(comp)}, _scalar{std::move(scalar)}
        {
        }

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The real part of the quotient of the operands.
         */
        constexpr tcomp_elem_t<Comp> real() const
        {
            return _comp.real() / _scalar;
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The imaginary part of the quotient of the operands.
         */
        constexpr tcomp_elem_t<Comp> imag() const
        {
            return _comp.imag() / _scalar;
        }
    };

    template <typename Comp>
    class tcomp_info<tcomp_reduce<Comp>>
    {
    public:
        using Elem = tcomp_elem_t<Comp>;
    };

    /**
     * @brief Complex number expression for a product of two complex numbers.
     *
     * @tparam Lhs The complex number expression type of the left side operand.
     * @tparam Rhs The complex number expression type of the right side operand.
     */
    template <typename Lhs, typename Rhs>
    class tcomp_prod : public tcomp_expr<tcomp_prod<Lhs, Rhs>>
    {
    private:
        Lhs _lhs;
        Rhs _rhs;

    public:
        /**
         * @brief Construct the product expression from its operands.
         *
         * @param lhs The complex number expression of the left side operand.
         * @param rhs The complex number expression of the right side operand.
         */
        explicit constexpr tcomp_prod(Lhs lhs, Rhs rhs)
            : _lhs{std::move(lhs)}, _rhs{std::move(rhs)}
        {
        }

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The real part of the product of the operands.
         */
        constexpr tcomp_elem_t<Lhs> real() const
        {
            return _lhs.real() * _rhs.real() - _lhs.imag() * _rhs.imag();
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The imaginary part of the product of the operands.
         */
        constexpr tcomp_elem_t<Lhs> imag() const
        {
            return _lhs.imag() * _rhs.real() + _lhs.real() * _rhs.imag();
        }
    };

    template <typename Lhs, typename Rhs>
    class tcomp_info<tcomp_prod<Lhs, Rhs>>
    {
    public:
        using Elem = tcomp_elem_t<Lhs>;
    };

    /**
     * @brief Complex number expression for a quotient of two complex numbers.
     *
     * @tparam Lhs The complex number expression type of the left side operand.
     * @tparam Rhs The complex number expression type of the right side operand.
     */
    template <typename Lhs, typename Rhs>
    class tcomp_quot : public tcomp_expr<tcomp_quot<Lhs, Rhs>>
    {
    private:
        Lhs _lhs;
        Rhs _rhs;

    public:
        /**
         * @brief Construct the quotient expression from its operands.
         *
         * @param lhs The complex number expression of the left side operand.
         * @param rhs The complex number expression of the right side operand.
         */
        explicit constexpr tcomp_quot(Lhs lhs, Rhs rhs)
            : _lhs{std::move(lhs)}, _rhs{std::move(rhs)}
        {
        }

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The real part of the quotient of the operands.
         */
        constexpr tcomp_elem_t<Lhs> real() const
        {
            return (_lhs * _rhs.conj()).real() / _rhs.norm();
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The imaginary part of the quotient of the operands.
         */
        constexpr tcomp_elem_t<Lhs> imag() const
        {
            return (_lhs * _rhs.conj()).imag() / _rhs.norm();
        }
    };

    template <typename Lhs, typename Rhs>
    class tcomp_info<tcomp_quot<Lhs, Rhs>>
    {
    public:
        using Elem = tcomp_elem_t<Lhs>;
    };

    /**
     * @brief Complex number expression for casting to a new element type.
     *
     * @tparam T The element type to cast to.
     * @tparam Comp The complex number expression type of the expression being
     * cast.
     */
    template <typename T, typename Comp>
    class tcomp_cast : public tcomp_expr<tcomp_cast<T, Comp>>
    {
    private:
        Comp _target;

    public:
        /**
         * @brief Construct the complex number expression from the cast target
         * expression.
         *
         * @param target The expression to be cast.
         */
        explicit constexpr tcomp_cast(Comp target) : _target{std::move(target)}
        {
        }

        /**
         * @brief The real part accessor needed to define this as a complex
         * number expression.
         *
         * @return The real part of the target expression cast to type `T`.
         */
        constexpr T real() const
        {
            return static_cast<T>(_target.real());
        }

        /**
         * @brief The imaginary part accessor needed to define this as a complex
         * number expression.
         *
         * @return The imaginary part of the target expression cast to type `T`.
         */
        constexpr T imag() const
        {
            return static_cast<T>(_target.imag());
        }
    };

    template <typename T, typename Comp>
    class tcomp_info<tcomp_cast<T, Comp>>
    {
    public:
        using Elem = T;
    };

    /**
     * @brief The base complex number expression class, used for the CRTP
     * pattern.
     *
     * This base class contains almost all the operations on complex numbers,
     * such as conjugation and arithmetic operator overloads.
     *
     * @tparam Derived The type of the class deriving from this base for CRTP.
     */
    template <typename Derived>
    class tcomp_expr
    {
    public:
        using Elem = tcomp_elem_t<Derived>;

    protected:
        // CRTP safeguard
        tcomp_expr()  = default;
        ~tcomp_expr() = default;

    public:
        // Casts:

        /**
         * @brief Operator overload wrapping the `tcomp_cast` expression.
         *
         * @return A `tvec_cast` expression targeting this.
         */
        template <typename T>
        constexpr operator tcomp_cast<T, Derived>() const
        {
            Derived copy = static_cast<Derived const&>(*this);
            return tcomp_cast<T, Derived>{std::move(copy)};
        }

        /**
         * @brief Operator overload for casting to a concrete complex number of
         * different element type.
         *
         * @tparam T The element type to cast to.
         * @return A concrete evaluation of the respective `tcomp_cast`
         * expression.
         */
        template <
            typename T,
            typename =
                std::enable_if_t<!std::is_same_v<T, tcomp_elem_t<Derived>>>>
        constexpr operator tcomp<T>() const
        {
            return tcomp{static_cast<tcomp_cast<T, Derived>>(*this)};
        }

        // Component accessors:

        /**
         * @brief The real part accessor, delegating to derived classes by CRTP.
         *
         * @return The real part of the complex number result of this
         * expression.
         */
        constexpr Elem real() const
        {
            return static_cast<Derived const&>(*this).real();
        }

        /**
         * @brief The imaginary part accessor, delegating to derived classes by
         * CRTP.
         *
         * @return The imaginary part of the complex number result of this
         * expression.
         */
        constexpr Elem imag() const
        {
            return static_cast<Derived const&>(*this).imag();
        }

        // transforms / calculations:

        /**
         * @brief Create the conjugation of this expression.
         *
         * @return The complex conjugate of the result of this expression.
         */
        constexpr auto conj() const
        {
            Derived copy = static_cast<Derived const&>(*this);
            return tcomp_conj{std::move(copy)};
        }

        /**
         * @brief Calculate the square modulus of this expression. The result
         * stays in the type of the elements.
         *
         * @return The square modulus of the result of this expression.
         */
        constexpr Elem norm() const
        {
            auto r = real();
            auto i = imag();

            return r * r + i * i;
        }

        /**
         * @brief Calculate the modulus (or absolute value) of this expression.
         *
         * The calculation is done as a double for generality, directly
         * equivalent to `std::sqrt(static_cast<double>(norm()))`.
         *
         * @return The modulus of the result of this expression.
         */
        constexpr double abs() const
        {
            return std::sqrt(static_cast<double>(norm()));
        }

        /**
         * @brief Calculate the argument of this expression.
         *
         * The calculation is done as a double for generality, calling
         * `std::atan2` for doubles.
         *
         * @return The argument of the result of this expression.
         */
        constexpr double arg() const
        {
            return std::atan2(
                static_cast<double>(real()), static_cast<double>(imag()));
        }

        /**
         * @brief Calculate the reciprocal of this expression.
         */
        constexpr auto inv() const
        {
            return conj() / norm();
        }
    };

    enum class tcomp_tag
    {
        Dummy
    };

    // operator overloads:

    template <
        typename Lhs,
        typename Rhs,
        typename  = enable_if_comps_t<Lhs, Rhs>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator+(Lhs lhs, Rhs rhs)
    {
        return tcomp_sum{std::move(lhs), std::move(rhs)};
    }

    template <
        typename Comp,
        typename T,
        typename  = enable_if_comp_t<Comp>,
        typename  = std::enable_if_t<std::is_same_v<tcomp_elem_t<Comp>, T>>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator+(Comp lhs, T rhs)
    {
        return tcomp_shiftr{std::move(lhs), std::move(rhs)};
    }

    template <
        typename Comp,
        typename T,
        typename  = enable_if_comp_t<Comp>,
        typename  = std::enable_if_t<std::is_same_v<tcomp_elem_t<Comp>, T>>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator+(T lhs, Comp rhs)
    {
        return rhs + lhs;
    }

    template <
        typename Comp,
        typename T,
        typename  = enable_if_comp_t<Comp>,
        typename  = std::enable_if_t<std::is_same_v<tcomp_elem_t<Comp>, T>>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator-(Comp lhs, T rhs)
    {
        return tcomp_shiftl{std::move(lhs), std::move(rhs)};
    }

    template <
        typename Comp,
        typename T,
        typename  = enable_if_comp_t<Comp>,
        typename  = std::enable_if_t<std::is_same_v<tcomp_elem_t<Comp>, T>>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator-(T lhs, Comp rhs)
    {
        return tcomp_refl{std::move(lhs), std::move(rhs)};
    }

    template <
        typename Lhs,
        typename Rhs,
        typename  = enable_if_comps_t<Lhs, Rhs>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator-(Lhs lhs, Rhs rhs)
    {
        return tcomp_diff{std::move(lhs), std::move(rhs)};
    }

    template <
        typename Comp,
        typename  = enable_if_comp_t<Comp>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator-(Comp comp)
    {
        return tcomp_neg{std::move(comp)};
    }

    template <
        typename Lhs,
        typename Rhs,
        typename  = enable_if_comps_t<Lhs, Rhs>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator*(Lhs lhs, Rhs rhs)
    {
        return tcomp_prod{std::move(lhs), std::move(rhs)};
    }

    template <
        typename Comp,
        typename  = enable_if_comp_t<Comp>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator*(tcomp_elem_t<Comp> lhs, Comp rhs)
    {
        return tcomp_scale{std::move(rhs), std::move(lhs)};
    }
    template <
        typename Comp,
        typename  = enable_if_comp_t<Comp>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator*(Comp lhs, tcomp_elem_t<Comp> rhs)
    {
        return rhs * lhs;
    }

    template <
        typename Lhs,
        typename Rhs,
        typename  = enable_if_comps_t<Lhs, Rhs>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator/(Lhs lhs, Rhs rhs)
    {
        return tcomp_quot{std::move(lhs), std::move(rhs)};
    }

    template <
        typename Comp,
        typename  = enable_if_comp_t<Comp>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator/(Comp lhs, tcomp_elem_t<Comp> rhs)
    {
        return tcomp_reduce{std::move(lhs), std::move(rhs)};
    }

    template <
        typename Comp,
        typename  = enable_if_comp_t<Comp>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator/(tcomp_elem_t<Comp> lhs, Comp rhs)
    {
        return lhs * rhs.conj() / rhs.norm();
    }

    template <typename Elem>
    constexpr auto operator==(tcomp<Elem> lhs, tcomp<Elem> rhs)
    {
        return lhs.real() == rhs.real() && lhs.imag() == rhs.imag();
    }

    template <typename Elem>
    constexpr auto operator==(tcomp<Elem> lhs, Elem rhs)
    {
        return lhs.real() == rhs && iszero(lhs.imag());
    }

    template <typename Elem>
    constexpr auto operator==(Elem lhs, tcomp<Elem> rhs)
    {
        return rhs == lhs;
    }

    template <
        typename Lhs,
        typename Rhs,
        typename  = enable_if_comps_t<Lhs, Rhs>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator==(Lhs lhs, Rhs rhs)
    {
        return tcomp{lhs} == tcomp{rhs};
    }

    template <
        typename Comp,
        typename  = enable_if_comp_t<Comp>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator==(Comp lhs, tcomp_elem_t<Comp> rhs)
    {
        return tcomp{lhs} == rhs;
    }

    template <
        typename Comp,
        typename  = enable_if_comp_t<Comp>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator==(tcomp_elem_t<Comp> lhs, Comp rhs)
    {
        return rhs == lhs;
    }

    template <typename Elem>
    constexpr auto operator!=(tcomp<Elem> lhs, tcomp<Elem> rhs)
    {
        return !(lhs == rhs);
    }

    template <typename Elem>
    constexpr auto operator!=(tcomp<Elem> lhs, Elem rhs)
    {
        return !(lhs == rhs);
    }

    template <typename Elem>
    constexpr auto operator!=(Elem lhs, tcomp<Elem> rhs)
    {
        return !(lhs == rhs);
    }

    template <
        typename Lhs,
        typename Rhs,
        typename  = enable_if_comps_t<Lhs, Rhs>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator!=(Lhs lhs, Rhs rhs)
    {
        return !(lhs == rhs);
    }

    template <
        typename Comp,
        typename  = enable_if_comp_t<Comp>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator!=(Comp lhs, tcomp_elem_t<Comp> rhs)
    {
        return !(lhs == rhs);
    }

    template <
        typename Comp,
        typename  = enable_if_comp_t<Comp>,
        tcomp_tag = tcomp_tag::Dummy>
    constexpr auto operator!=(tcomp_elem_t<Comp> lhs, Comp rhs)
    {
        return !(lhs == rhs);
    }

    template <
        typename Comp,
        typename  = enable_if_comp_t<Comp>,
        tcomp_tag = tcomp_tag::Dummy>
    std::ostream& operator<<(std::ostream& ostream, Comp comp)
    {
        return ostream << comp.real() << " + i " << comp.imag();
    }

    // aliases:

    using fcomp = tcomp<float>;
    using dcomp = tcomp<double>;
    using icomp = tcomp<int>;
    using ucomp = tcomp<unsigned int>;

    using comp = fcomp;

    // Compound assignment operator implementations:

    template <typename T>
    template <typename Comp, typename, typename>
    constexpr tcomp<T>& tcomp<T>::operator+=(Comp other)
    {
        *this = *this + other;
        return *this;
    }

    template <typename T>
    template <typename Comp, typename, typename>
    constexpr tcomp<T>& tcomp<T>::operator-=(Comp other)
    {
        *this = *this - other;
        return *this;
    }

    template <typename T>
    template <typename Comp, typename, typename>
    constexpr tcomp<T>& tcomp<T>::operator*=(Comp other)
    {
        *this = *this * other;
        return *this;
    }

    template <typename T>
    template <typename Comp, typename, typename>
    constexpr tcomp<T>& tcomp<T>::operator/=(Comp other)
    {
        *this = *this / other;
        return *this;
    }

    template <typename T>
    constexpr tcomp<T>& tcomp<T>::operator+=(T scalar)
    {
        *this = *this + scalar;
        return *this;
    }

    template <typename T>
    constexpr tcomp<T>& tcomp<T>::operator-=(T scalar)
    {
        *this = *this - scalar;
        return *this;
    }

    template <typename T>
    constexpr tcomp<T>& tcomp<T>::operator*=(T scalar)
    {
        *this = *this * scalar;
        return *this;
    }

    template <typename T>
    constexpr tcomp<T>& tcomp<T>::operator/=(T scalar)
    {
        *this = *this / scalar;
        return *this;
    }
}

// cmath overloads
namespace std
{
    template <
        typename Comp,
        typename       = mth::enable_if_comp_t<Comp>,
        mth::tcomp_tag = mth::tcomp_tag::Dummy>
    mth::dcomp sqrt(Comp z)
    {
        auto arg  = 0.5 * z.arg();
        auto magn = std::sqrt(z.abs());

        return magn * mth::dcomp{std::cos(arg), std::sin(arg)};
    }

    template <
        typename Comp,
        typename       = mth::enable_if_comp_t<Comp>,
        mth::tcomp_tag = mth::tcomp_tag::Dummy>
    mth::dcomp exp(Comp z)
    {
        double a = z.real();
        double b = z.imag();

        return std::exp(a) * mth::dcomp{std::cos(b), std::sin(b)};
    }

    template <
        typename Comp,
        typename       = mth::enable_if_comp_t<Comp>,
        mth::tcomp_tag = mth::tcomp_tag::Dummy>
    mth::dcomp log(Comp z)
    {
        auto arg  = z.arg();
        auto magn = z.abs();

        return std::log(magn) + mth::dcomp{0, arg};
    }

    template <
        typename Comp,
        typename       = mth::enable_if_comp_t<Comp>,
        mth::tcomp_tag = mth::tcomp_tag::Dummy>
    mth::dcomp cos(Comp z)
    {
        mth::dcomp ix = mth::dcomp{0, 1} * z;
        return (std::exp(ix) + std::exp(-ix)) / 2.0;
    }

    template <
        typename Comp,
        typename       = mth::enable_if_comp_t<Comp>,
        mth::tcomp_tag = mth::tcomp_tag::Dummy>
    mth::dcomp sin(Comp z)
    {
        mth::dcomp ix = mth::dcomp{0, 1} * z;
        return (std::exp(ix) - std::exp(-ix)) / 2.0;
    }
}