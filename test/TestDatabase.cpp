#include <gtest/gtest.h>

#include "Database/DuckDb.hpp"
#include "TestAppData.hpp"
#include "TestSqlResource.hpp"

class DatabaseTests : public ::testing::Test {
protected:
    DuckDb m_DuckDb{ TestAppData::DatabaseFilePath() };
};

TEST_F(DatabaseTests, TestQuery) {
    // TODO
}

TEST_F(DatabaseTests, InitializeSchema) {
    // TODO
    m_DuckDb.Query(TestSqlResource::InitializeSchema());
}
