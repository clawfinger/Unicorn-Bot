#pragma once
#include <string>
#include <vector>
#include "Meta.h"

struct Worktime
{
    int id;
    std::string Username;
    std::string Task;
    int CU;
    int timestamp;
};

struct WorktimeWriteEntryCommand
{
    std::vector<Worktime> data;
};
REGISTER_TYPENAME(WorktimeWriteEntryCommand)

struct WorktimeTimeIntervalQuery
{
    typedef std::vector<Worktime> resultType;
    resultType data;

    std::string Username;
    int timestampFrom;
    int timestampTo;
};
REGISTER_TYPENAME(WorktimeTimeIntervalQuery)
