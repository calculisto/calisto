#pragma once

    template <class>
    struct
is_a_uncertain_value_t
    : std::false_type
{};

    template <class T>
    struct
is_a_uncertain_value_t <uncertain_value_t <T>>
    : std::true_type
{};

    template <class T>
    constexpr auto
is_a_uncertain_value_v = is_a_uncertain_value_t <T>::value;


// Remove ambiguous overload for operator*
    template <
          class T
        , dimension_t D
        , class U
        , class V = decltype (std::declval <T> () * std::declval <U> ())
        , class = std::enable_if_t <!is_a_uncertain_value_v <U>>
    >
    constexpr auto
operator * (
      uncertain_value_t <T> u
    , quantity_t <D, U> const& q
)
    -> quantity_t <D, uncertain_value_t <V>>
{
    return quantity_t <D, uncertain_value_t <V>> { u * q.magnitude };
}

    template <
          class T
        , dimension_t D
        , class U
        , class V = decltype (std::declval <T> () * std::declval <U> ())
        , class = std::enable_if_t <!is_a_uncertain_value_v <U>>
    >
    constexpr auto
operator * (
      uncertain_value_t <T> u
    , quantity_t <D, uncertain_value_t <U>> const& q
)
    -> quantity_t <D, uncertain_value_t <V>>
{
    return quantity_t <D, uncertain_value_t <V>> { u * q.magnitude };
}


    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () * std::declval <U> ())
        , class = std::enable_if_t <!is_a_uncertain_value_v <U>>
    >
    constexpr auto
operator * (
      uncertain_value_t <T> u
    , any_quantity_t <U> const& q
)
    -> any_quantity_t <uncertain_value_t <V>>
{
    return any_quantity_t <uncertain_value_t <V>> { u * q.magnitude, q.dimension };
}

    template <
          class T
        , class U
        , class V = decltype (std::declval <T> () * std::declval <U> ())
        , class = std::enable_if_t <!is_a_uncertain_value_v <U>>
    >
    constexpr auto
operator * (
      uncertain_value_t <T> u
    , any_quantity_t <uncertain_value_t <U>> const& q
)
    -> any_quantity_t <uncertain_value_t <V>>
{
    return any_quantity_t <uncertain_value_t <V>> { u * q.magnitude, q.dimension };
}


    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () * std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator * (
      array_t <uncertain_value_t <T>, N> const& a
    , uncertain_value_t <U> const& u
){
        array_t <uncertain_value_t <V>, N>
    r { a };
    std::transform (
          r.begin ()
        , r.end   ()
        , r.begin ()
        , [&](auto x){ return x * u; }
    );
    return r;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () * std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator * (
      array_t <T, N> const& a
    , uncertain_value_t <U> const& u
){
    return array_t <uncertain_value_t <V>, N> { a } * u;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () * std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator * (
      uncertain_value_t <T> const& u
    , array_t <uncertain_value_t <U>, N> const& a
){
        array_t <uncertain_value_t <V>, N>
    r { a };
    std::transform (
          r.begin ()
        , r.end   ()
        , r.begin ()
        , [&](auto x){ return u * x; }
    );
    return r;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () * std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator * (
      uncertain_value_t <U> const& u
    , array_t <T, N> const& a
){
    return u * array_t <uncertain_value_t <V>, N> { a };
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () / std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator / (
      array_t <uncertain_value_t <T>, N> const& a
    , uncertain_value_t <U> const& u
){
        array_t <uncertain_value_t <V>, N>
    r { a };
    std::transform (
          r.begin ()
        , r.end   ()
        , r.begin ()
        , [&](auto x){ return x / u; }
    );
    return r;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () / std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator / (array_t <T, N> const& a, uncertain_value_t <U> const& u)
{
    return array_t <uncertain_value_t <V>, N> { a } / u;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () / std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator / (
      uncertain_value_t <T> const& u
    , array_t <uncertain_value_t <U>, N> const& a
){
        array_t <uncertain_value_t <V>, N>
    r { a };
    std::transform (
          r.begin ()
        , r.end   ()
        , r.begin ()
        , [&](auto x){ return u / x; }
    );
    return r;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () / std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator / (uncertain_value_t <U> const& u, array_t <T, N> const& a)
{
    return u / array_t <uncertain_value_t <V>, N> { a };
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator + (
      array_t <uncertain_value_t <T>, N> const& a
    , uncertain_value_t <U> const& u
){
        array_t <uncertain_value_t <V>, N>
    r { a };
    std::transform (
          r.begin ()
        , r.end   ()
        , r.begin ()
        , [&](auto x){ return x + u; }
    );
    return r;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator + (array_t <T, N> const& a, uncertain_value_t <U> const& u)
{
    return array_t <uncertain_value_t <V>, N> { a } + u;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator + (
      uncertain_value_t <T> const& u
    , array_t <uncertain_value_t <U>, N> const& a
){
        array_t <uncertain_value_t <V>, N>
    r { a };
    std::transform (
          r.begin ()
        , r.end   ()
        , r.begin ()
        , [&](auto x){ return u + x; }
    );
    return r;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () + std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator + (uncertain_value_t <U> const& u, array_t <T, N> const& a)
{
    return u + array_t <uncertain_value_t <V>, N> { a };
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () - std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator - (
      array_t <uncertain_value_t <T>, N> const& a
    , uncertain_value_t <U> const& u
){
        array_t <uncertain_value_t <V>, N>
    r { a };
    std::transform (
          r.begin ()
        , r.end   ()
        , r.begin ()
        , [&](auto x){ return x - u; }
    );
    return r;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () - std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator - (array_t <T, N> const& a, uncertain_value_t <U> const& u)
{
    return array_t <uncertain_value_t <V>, N> { a } - u;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () - std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator - (
      uncertain_value_t <T> const& u
    , array_t <uncertain_value_t <U>, N> const& a
){
        array_t <uncertain_value_t <V>, N>
    r { a };
    std::transform (
          r.begin ()
        , r.end   ()
        , r.begin ()
        , [&](auto x){ return u - x; }
    );
    return r;
}
    template <
          class T
        , std::size_t N
        , class U
        , class V = decltype (std::declval <T> () - std::declval <U> ())
    >
    constexpr array_t <uncertain_value_t <V>, N>
operator - (uncertain_value_t <U> const& u, array_t <T, N> const& a)
{
    return u - array_t <uncertain_value_t <V>, N> { a };
}


    namespace
isto::array 
{
    template <
          class T
        , class U, std::size_t N
        , class V = decltype (pow (std::declval <T> (), std::declval <U> ()))
    >
    constexpr array_t <uncertain_value_t <V>, N>
pow (const array_t <T, N>& p, uncertain_value_t <U> const& exp)
{
        array_t <uncertain_value_t <V>, N>
    r;
    std::transform (
            p.begin (),
            p.end   (),
            r.begin (),
            [=](auto e){ return pow (e, exp); }
    );
    return r;
}

    template <
          class T
        , class U
        , std::size_t N
        , class V = decltype (pow (std::declval <T> (), std::declval <U> ()))
    >
    constexpr array_t <uncertain_value_t <V>, N>
pow (uncertain_value_t <T> const& t, const array_t <U, N>& exp)
{
        array_t <uncertain_value_t <V>, N>
    r;
    std::transform (
            exp.begin (),
            exp.end   (),
            r.begin (),
            [=](auto ee){ return pow (t, ee); }
    );
    return r;
}

} // namespace isto::array
