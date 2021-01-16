#pragma once
#include <iostream>
#include <any>
#include "DataTransferObjects.h"
#include "Meta.h"
#include "Deps/sqliteWrapper/DBAdapter.h"
#include <sstream>

class Dispatcher
{
public:
    Dispatcher(DBAdapter& db);

    template<typename T>
    void execute(const T& command);

private:
    DBAdapter& m_db;
};

template<typename T>
inline void Dispatcher::execute(const T& command)
{
    std::cerr << "No execute method for type " << typeName<T>() << std::endl;
}

template<>
inline void Dispatcher::execute<WorktimeWriteEntryCommand>(const WorktimeWriteEntryCommand& command)
{
    for (const auto& entry: command.data)
    {
        std::stringstream ss;
        ss << "insert into Worktime (Username, Task, CU, Timestamp) values (";
        ss << "'" << entry.Username << "'";
        ss << ", ";
        ss << "'" << entry.Task << "'";
        ss << ", ";
        ss << entry.CU;
        ss << ", ";
        ss << entry.timestamp;
        ss << ");";
        m_db.execute(ss.str());
    }
}


class QueryHandler
{
public:
    QueryHandler(DBAdapter& db);

    template<typename T>
    typename T::resultType request(const T& query);

private:
    DBAdapter& m_db;
};

template<typename T>
inline typename T::resultType QueryHandler::request(const T& query)
{
    std::cerr << "No query request for type " << typeName<T>() << std::endl;
}

template<>
inline typename WorktimeTimeIntervalQuery::resultType QueryHandler::request<WorktimeTimeIntervalQuery>(const WorktimeTimeIntervalQuery& query)
{
    std::stringstream ss;
    ss << "select * from Worktime where ";
    ss << "Username='" << query.Username << "' ";
    ss << "and ";
    ss << "Timestamp ";
    ss << ">= ";
    ss << query.timestampFrom << " ";;
    ss << "and ";
    ss << "Timestamp ";
    ss << "<= ";
    ss << query.timestampTo;
    ss << ";";
    std::vector<Record> queryResult = m_db.execute(ss.str());
    WorktimeTimeIntervalQuery::resultType result;

    for (const auto& record: queryResult)
    {
        try
        {
            Worktime entry;
            entry.id = std::any_cast<int>(record.getColumn(0).getValue());
            entry.Username = std::any_cast<std::string>(record.getColumn(1).getValue());
            entry.Task = std::any_cast<std::string>(record.getColumn(2).getValue());
            entry.CU = std::any_cast<int>(record.getColumn(3).getValue());
            entry.timestamp = std::any_cast<int>(record.getColumn(4).getValue());
            result.push_back(entry);
        }
        catch (const std::bad_any_cast& ex)
        {
            std::cerr << "Bad any_cast!" << std::endl;
        }
    }
    return result;
}
