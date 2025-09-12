#include "repository/DeckStore.hpp"

[[nodiscard]] Types::IDVector DeckStore::GetDeckIDs() const {
    Types::IDVector idv;
    idv.reserve(m_DeckRecords.size());
    for (auto it = m_DeckRecords.begin(); it != m_DeckRecords.end(); it++) {
        idv.push_back(it->second.DeckRecordContext.ID());
    }
    return idv;
}
