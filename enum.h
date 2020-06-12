#include <array>
#include <string_view>

// C++ 17 enum with std::to_string(enum class) method and Name::End
// Designed to be quick to compile and to run.
// Enums are literally just C++ `enum class` es... no nonsense!
//
// Example usage:
//
//    Enum(Name,
//        A, 
//        B,
//        C
//    );
//
// You cannot have a trailing comma (you'd get a compiler error).
//
// With -O3, GCC compiles to_string to:
//
//    std::to_string(Name):
//    movsx rdi, edi
//    sal rdi, 4
//    mov rax, QWORD PTR std::to_string(Name)::strs[rdi]
//    mov rdx, QWORD PTR std::to_string(Name)::strs[rdi+8]
//    ret
//
// ... and it's still O(1) without any -O
#define EnumWithUnderlying(Name, NTy, ... )\
enum class Name : NTy {__VA_ARGS__, End };\
namespace std {\
    std::string_view to_string(enum Name enumIn) noexcept {\
        constexpr static string_view fullstr = #__VA_ARGS__ ", End ";\
        struct BeginLen {\
            std::size_t begin;\
            std::size_t len;\
        };\
        using ArrIdxTy = std::array<BeginLen, NTy(Name::End)+1>;\
        constexpr static ArrIdxTy begins = [&]() {\
            ArrIdxTy ret = {};\
            std::size_t jbegin = 0;\
            std::size_t jend = 0;\
            enum ParserState {Before,In,After};\
            ParserState state = Before;\
            for (std::size_t i = 0; i < fullstr.size(); ++i) {\
                auto isSpace = [](char ch) -> bool {\
                    return ch == ' ' || ch == '\t' || ch == '\n';\
                };\
                switch (state) {\
                    case Before:\
                        if (!isSpace(fullstr[i])) {\
                            ret[jbegin].begin = i;\
                            jbegin++;\
                            state = In;\
                        } else {\
                            break;\
                        }\
                    case In:\
                        if (isSpace(fullstr[i]) || fullstr[i] == ',') {\
                            ret[jend].len = i - ret[jend].begin;\
                            jend++;\
                            state = fullstr[i] == ',' ? Before : After;\
                        } else {\
                            break;\
                        }\
                        break;\
                    case After:\
                        if (fullstr[i] == ',') {\
                            state = Before;\
                        }\
                        break;\
                }\
            }\
            return ret;\
        }();\
        using ArrStrTy = std::array<std::string_view, NTy(Name::End)+1>;\
        constexpr static ArrStrTy strs = [&]() {\
            ArrStrTy ret = {};\
            for (std::size_t i = 0; i < begins.size(); ++i) {\
                ret[i] = std::string_view(fullstr.begin() + begins[i].begin, begins[i].len);\
            }\
            return ret;\
        }();\
        return strs[NTy(enumIn)];\
    };\
}
#define Enum(Name,...) EnumWithUnderlying(Name, int, __VA_ARGS__)

