#ifndef CURRY_H_INCLUDED
#define CURRY_H_INCLUDED

#include <functional>
#include <type_traits>


template <typename T>
class CurriedFunction;

template <typename R, typename... Args>
class CurriedFunction<R(Args...)>
{
private:
    std::function<R(Args...)> func;

    template <size_t N, typename... Arguments>
    struct DropArgumentsHelper;

    template <size_t N, typename T, typename... Arguments>
    struct DropArgumentsHelper<N, T, Arguments...>
    {
        using type = std::conditional_t<
            N <= 0,
            CurriedFunction<R(T, Arguments...)>,
            typename DropArgumentsHelper<N - 1, Arguments...>::type
        >;
    };

    template <size_t N>
    struct DropArgumentsHelper<N>
    {
        using type = void;
    };

    template <size_t N>
    using DropArguments = typename DropArgumentsHelper<N, Args...>::type;

    enum class Enabler {};
    static constexpr Enabler dummyEnabler = {};


public:
    CurriedFunction() = default;

    template <typename F>
    CurriedFunction(const F& f): func{f} {}

    template <typename F>
    CurriedFunction& operator=(const F& f)
    {
        func = f;
        return *this;
    }

    template <typename... CallArgs, std::enable_if_t<(sizeof...(CallArgs) == sizeof...(Args)), Enabler> = dummyEnabler>
    auto operator()(CallArgs &&... args)
    {
        return func(std::forward<CallArgs>(args)...);
    }

    template <typename... CallArgs, std::enable_if_t<(sizeof...(CallArgs) < sizeof...(Args)), Enabler> = dummyEnabler>
    auto operator()(CallArgs &&... args)
    {
        return DropArguments<sizeof...(CallArgs)> {
            [funcCopy = func, &args...](auto &&... rest) mutable {
                return funcCopy(std::forward<CallArgs>(args)..., std::forward<decltype(rest)>(rest)...);
            }
        };
    }
};

#endif // CURRY_H_INCLUDED
