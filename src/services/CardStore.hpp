#pragma once

#include <boost/uuid/uuid.hpp>

#include <functional>
#include <optional>
#include <unordered_map>

#include "database/FlashCardRecord.hpp"

class CardStore final {
public:
    using ID = boost::uuids::uuid;

    [[nodiscard]] ID Create(FlashCard&& card);

    [[nodiscard]] std::optional<std::reference_wrapper<const FlashCard>>
    Read(const ID& cardID) const noexcept {
        if (auto it = m_Cards.find(cardID); it != m_Cards.end()) {
            return std::cref(it->second.Card);
        }
        return std::nullopt;
    }

    template <typename Func>
    [[nodiscard]] bool Update(const ID& cardID, Func&& updateFunc) {
        if (auto it = m_Cards.find(cardID); it != m_Cards.end()) {
            updateFunc(it->second.Card);
            return true;
        }
        return false;
    }

    [[nodiscard]] bool Delete(const ID& cardID);

private:
    std::unordered_map<ID, FlashCardRecord> m_Cards;
};
