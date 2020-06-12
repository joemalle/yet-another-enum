// Shows usage of Enum macro
//
// $ g++ example.cpp -std=c++17
// $ ./a.out
// UP DOWN LEFT RIGHT 4 A B 
//

#include <iostream>
#include "enum.h"

EnumWithUnderlying(Move, char,
  UP,
  DOWN,
  LEFT,
  RIGHT
);

Enum(Letter, A, B);

int main() {
  std::cout << 
    std::to_string(Move::UP) << " " <<
    std::to_string(Move::DOWN) << " " <<
    std::to_string(Move::LEFT) << " " <<
    std::to_string(Move::RIGHT) << " " <<
    int(Move::End) << " " <<
    std::to_string(Letter::A) << " " <<
    std::to_string(Letter::B) << " " <<
    std::endl;
}
