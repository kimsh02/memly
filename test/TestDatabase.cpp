#include <gtest/gtest.h>

#include <duckdb.hpp>

#include "TestApp/SqlResource.hpp"
#include "TestApp/SupportData.hpp"

class DatabaseTests : public ::testing::Test {
protected:
    duckdb::DuckDB m_Database{ TestApp::SupportData::DatabaseFilePath() };
    duckdb::Connection m_Connection{ m_Database };
};

TEST_F(DatabaseTests, TestQuery) {
    // TODO
}

TEST_F(DatabaseTests, InitializeSchema) {
    // TODO
    m_Connection.Query(TestApp::SqlResource::InitializeSchemaSql());
}
