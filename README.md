# Enum.h

C++ 17 enum with std::to_string(enum class) method and Name::End
Designed to be quick to compile and to run.
Enums are literally just C++ `enum class` es... no nonsense!
You can use any number of items!  It will still be quick!  I've tested up to about 1000 items.

# Example

    Enum(Letter,
        A, B, C 
        D, E, F
    );

... just like you'd expect! See example.cpp for more usage.

# std::to_string

This returns a string_view rather than a string.  You could easily change that if you wanted to.

With `-O3`, GCC compiles to_string to:

       std::to_string(Name):
       movsx rdi, edi
       sal rdi, 4
       mov rax, QWORD PTR std::to_string(Name)::strs[rdi]
       mov rdx, QWORD PTR std::to_string(Name)::strs[rdi+8]
       ret

... and it's still O(1) when optimizations are turned off (but not as pretty).

# Compilation speed

Running Clang with `-ftime-trace`, you can see this is super quick! <6ms and most of that time is spent including `<array>` (which could be removed but not worth it to me).

The example file also includes `<iostream>` which takes 710ms.  More importantly, there are no large templates to instantiate or macros to exapnd that would grow in proportion to the number of things in the enum. 

![This image shows that enum.h is super quick to compile!](/ftime-trace.png)

See also ftime-trace.json and ftime-trace.png.
