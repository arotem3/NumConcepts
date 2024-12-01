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
  struct is_real : std::true_type {};

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

  template <ScalarType T>
  using precision_t = get_precision<T>::type;

  template <typename T>
  concept ArithmeticType = std::is_arithmetic_v<T>;

  /**
   * @brief Vector type supporting scalar multiplication by real numbers.
   */
  template <typename T>
  concept RealVectorType = requires(T a, T b, RealType auto d) {
      // The type should be default-constructible
      { T() } -> std::same_as<T>;

      // It should support addition
      { a + b } -> std::same_as<T>;

      // It should support scalar multiplication with any real type
      { d * a } -> std::same_as<T>;
  };

  /**
   * @brief Vector type supporting scalar multiplication by complex numbers.
   * 
   * Note any VectorType is a RealVectorType.
   */
  template <typename T>
  concept VectorType = requires(T a, T b, ScalarType auto d) {
    { T() } -> std::same_as<T>;

    { a + b } -> std::same_as<T>;

    { d * a } -> std::same_as<T>;
  };
} // namespace numcepts

#endif
