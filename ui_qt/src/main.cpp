#include <print>

#include "repetition_ai/flashcard.hpp"

int main() {
    FlashCard test;
    test.SetFrontText("hello");
    test.SetBackText("hallo");
    std::println("{}", test.GetBackText());
    return 0;
}
