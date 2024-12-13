#ifndef NUMCEPTS_HPP
#define NUMCEPTS_HPP

#include <concepts>
#include <type_traits>
#include <complex>

namespace numcepts
{
  template <typename T>
  struct is_real : std::false_type {};

  template <std::floating_point T>
  struct is_real<T> : std::true_type {};

  template <typename T>
  struct is_real<const T> : is_real<T> {};

  template <typename T>
  struct is_real<T &> : is_real<T> {};

  template <typename T>
  struct is_real<T &&> : is_real<T> {};

  template <typename T>
  inline constexpr bool is_real_v = is_real<T>::value;

  /**
   * @brief Any real number type. For example, float, double, etc.
   */
  template <typename T>
  concept RealType = is_real_v<T>;

  template <typename T>
  struct is_complex : std::false_type {};

  template <RealType T>
  struct is_complex<std::complex<T>> : std::true_type {};

  template <typename T>
  struct is_complex<const T> : is_complex<T> {};

  template <typename T>
  struct is_complex<T &> : is_complex<T> {};

  template <typename T>
  struct is_complex<T &&> : is_complex<T> {};

  template <typename T>
  inline constexpr bool is_complex_v = is_complex<T>::value;

  /**
   * @brief Any complex number type.
   */
  template <typename T>
  concept ComplexType = is_complex_v<T>;

  /**
   * @brief Any scalar type, real or complex.
   */
  template <typename T>
  concept ScalarType = is_real_v<T> || is_complex_v<T>;

  template <typename T>
  struct get_precision;

  template <RealType T>
  struct get_precision<T>
  {
    using type = T;
  };

  template <RealType T>
  struct get_precision<std::complex<T>>
  {
    using type = T;
  };

  template <typename T>
  struct get_precision<const T> : get_precision<T> {};

  template <typename T>
  struct get_precision<T &> : get_precision<T> {};

  template <typename T>
  struct get_precision<T &&> : get_precision<T> {};

  template <typename T>
  using precision_t = get_precision<T>::type;

  template <typename T>
  concept ArithmeticType = std::is_arithmetic_v<T>;

  template <typename T>
  struct get_value_type
  {
    using type = typename T::value_type;
  };

  template <typename T>
  using value_t = get_value_type<T>::type;

  template <std::ranges::range T>
  struct get_precision<T>
  {
    using type = get_precision<value_t<T>>::type;
  };

  /**
   * @brief Vector type.
   */
  template <typename T>
  concept VectorType = requires(T a, T b, precision_t<T> c, value_t<T> d) {
    { a + b } -> std::convertible_to<T>; // It should support addition
    { c * a } -> std::convertible_to<T>;  // It should support scalar multiplication with a real type
    { d * a } -> std::convertible_to<T>;  // It should support scalar multiplication with a scalar type
  };
} // namespace numcepts

#endif
