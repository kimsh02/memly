#include <gtest/gtest.h>

#include "Database/DuckDb.hpp"
#include "TestApp/SqlResource.hpp"
#include "TestApp/SupportData.hpp"

class DatabaseTests : public ::testing::Test {
protected:
    DuckDb m_DuckDb{ TestApp::SupportData::DatabaseFilePath() };
};

TEST_F(DatabaseTests, TestQuery) {
    // TODO
}

TEST_F(DatabaseTests, InitializeSchema) {
    // TODO
    m_DuckDb.Query(TestApp::SqlResource::InitializeSchemaSql());
}
