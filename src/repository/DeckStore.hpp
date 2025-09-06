#pragma once

class DeckStore final {
public:
    DeckStore(const DeckStore&)            = delete;
    DeckStore& operator=(const DeckStore&) = delete;

    DeckStore(DeckStore&&) noexcept            = delete;
    DeckStore& operator=(DeckStore&&) noexcept = delete;
};
