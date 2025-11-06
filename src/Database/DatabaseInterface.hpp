#pragma once

#include <memory>

class QueryResultInterface {
public:
    virtual ~QueryResultInterface() = default;
};

class PreparedStatementInterface {
public:
    virtual ~PreparedStatementInterface() = default;

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
};
