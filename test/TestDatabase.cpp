#include <gtest/gtest.h>

#include "Database/DuckDb.hpp"
#include "TestAppSqlResource.hpp"
#include "TestAppSupportData.hpp"

class DatabaseTests : public ::testing::Test {
protected:
    DuckDb m_DuckDb{ TestAppSupportData::DatabaseFilePath() };
};

TEST_F(DatabaseTests, TestQuery) {
    // TODO
}

TEST_F(DatabaseTests, InitializeSchema) {
    // TODO
    m_DuckDb.Query(TestAppSqlResource::InitializeSchemaSql());
}
