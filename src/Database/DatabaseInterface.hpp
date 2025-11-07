#pragma once

#include <memory>

class QueryResultInterface {
public:
    virtual ~QueryResultInterface() = default;

    QueryResultInterface(const QueryResultInterface&)            = delete;
    QueryResultInterface& operator=(const QueryResultInterface&) = delete;
    QueryResultInterface(QueryResultInterface&&)                 = delete;
    QueryResultInterface& operator=(QueryResultInterface&&)      = delete;
};

class PreparedStatementInterface {
public:
    virtual ~PreparedStatementInterface() = default;

    PreparedStatementInterface(const PreparedStatementInterface&) = delete;
    PreparedStatementInterface&
    operator=(const PreparedStatementInterface&)             = delete;
    PreparedStatementInterface(PreparedStatementInterface&&) = delete;
    PreparedStatementInterface&
    operator=(PreparedStatementInterface&&) = delete;

    template <typename... Params>
    std::unique_ptr<QueryResultInterface> Execute(Params&&... Args) {
        return ExecuteImplementation(std::forward<Params>(Args)...);
    }

private:
    virtual std::unique_ptr<QueryResultInterface> ExecuteImplementation() = 0;
};

class DatabaseInterface {
public:
    virtual ~DatabaseInterface() = default;

    DatabaseInterface(const DatabaseInterface&)            = delete;
    DatabaseInterface& operator=(const DatabaseInterface&) = delete;
    DatabaseInterface(DatabaseInterface&&)                 = delete;
    DatabaseInterface& operator=(DatabaseInterface&&)      = delete;
};
