#ifndef MANGLED_HPP_INCLUDED
#define MANGLED_HPP_INCLUDED

#include<string_view>
#include<array>

namespace mangled
{
    namespace detail
    {
        template<typename F>
        inline constexpr std::string_view suffix()
        {
            // clang-format off
            // given a function
            //     short_name frobnicate(long_struct_name)
            // __FUNCDNAME__ in the function and suffix<decltype(a_longer_unique_name)> will yield
            //
            // 0         1         2         3         4         5         6         7         8         9         0         1         2
            // 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
            // ?frobnicate@@YA?AUshort_name@@Ulong_struct_name@@@Z
            // ??$suffix@$$A6A?AUshort_name@@Ulong_struct_name@@@Z@detail@mangled@@YA?AV?$basic_string_view@DU?$char_traits@D@std@@@std@@XZ
            //
            // suffix returns
            //               A?AUshort_name@@Ulong_struct_name@@@Z
            //
            // clang-format on
            auto str = std::string_view(__FUNCDNAME__);
            return str.substr(14, str.size() - 87);
        }

        // ?frobnicate@detail@mangled@@YA?AUshort_name@@Ulong_struct_name@@@Z

        template<typename L>
        struct constexpr_string
        {
            constexpr constexpr_string(L) {}
            static constexpr std::string_view data = L{}();
        };

        template<typename Name>
        inline constexpr int qualifiers()
        {
            int i = -2, count = -1;
            while(i != std::string_view::npos)
            {
                i = Name::data.find("::", i + 2);
                count++;
            }
            return count;
        }

        template<typename Name>
        inline constexpr auto split()
        {
            std::array<std::string_view, qualifiers<Name>() + 1> arr = {};
            int prev = -2;
            for(int i = arr.size() - 1; i > 0; i--)
            {
                int cur = Name::data.find("::", prev + 2);
                arr[i] = Name::data.substr(prev + 2, cur - prev - 2);
                prev = cur;
            }
            arr[0] = Name::data.substr(prev + 2);
            return arr;
        }

        template<typename F, typename Name>
        struct name_builder
        {
            static constexpr auto suf = detail::suffix<F>();
            static constexpr auto toks = split<Name>();
            static constexpr auto len = Name::data.size() + suf.size() - toks.size() + 5;
            static constexpr auto str = [] {
                std::array<char, len> arr = {};
                arr[0] = '?';
                int i = 1;
                for(int t = 0; t < toks.size(); t++)
                {
                    if(t > 0)
                        arr[i++] = '@';
                    for(auto c : toks[t])
                        arr[i++] = c;
                }

                arr[i++] = '@';
                arr[i++] = '@';
                arr[i++] = 'Y';

                for(auto c : suf)
                    arr[i++] = c;

                return arr;
            }();
        };
    }

    template<typename F, typename LambdaString>
    inline constexpr std::string_view name(LambdaString)
    {
        using Cs = detail::constexpr_string<LambdaString>;
        using N = detail::name_builder<F, Cs>;
        return {N::str.data(), N::len};
    }
}



#endif
