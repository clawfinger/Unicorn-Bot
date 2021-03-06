#include "Bot.h"
#include <sstream>

std::pair<time_t, time_t> getWeekInterval(const std::tm& timeData);


UnicornBot::UnicornBot(const std::string token, const char numOfThreads): SleepyDiscord::DiscordClient(token, numOfThreads), isDBready(true), disp(db), handler(db)
{
    db.open("database.db");
    int ec = db.lastResultCode();

    if (ec != 0) //Change magic 0 error code
        isDBready = false;
}

void UnicornBot::onMessage(SleepyDiscord::Message message)
{
    if (message.author.bot)
        return;

    auto channel = getChannel(message.channelID);
    if (channel.cast().type != SleepyDiscord::Channel::ChannelType::DM)
        return;

    if (!isDBready)
    {
        sendMessage(message.channelID, "Database not connected! Please, contact the administrator.");
        return;
    }
    std::stringstream ss;
    ss << message.content;

    std::string command;
    ss >> command;

    std::string reply;
    if (command == "help")
    {
        reply = handleHelpCommand();
    }
    else if (command == "add")
    {
        reply = handleAddCommand(ss, message);
    }
    else if (command == "status")
    {
        reply = handleStatusCommand(message);
    }
    else
    {
        return;
    }
    sendMessage(message.channelID, reply);
}

std::string UnicornBot::handleHelpCommand()
{
    std::stringstream answer;
    answer << "How to use bot:" << std::endl << std::endl;
    answer << "add <task> <CU>" << std::endl;
    answer << "Logging <CU> content units for the <task>" << std::endl << std::endl;
    answer << "status" << std::endl;
    answer << "Show all tracked CUs for the current week" << std::endl;
    return answer.str();
}

std::string UnicornBot::handleAddCommand(std::stringstream& ss, const SleepyDiscord::Message& message)
{
    std::time_t result = std::time(nullptr);
    std::tm time = *std::localtime(&result);

    std::string task;
    ss >> task;
    int CU;
    ss >> CU;

    Worktime entry;
    entry.Username = message.author.username;
    entry.Task = task;
    entry.CU = CU;
    entry.timestamp = std::mktime(&time);

    WorktimeWriteEntryCommand command;
    command.data.push_back(entry);
    disp.execute(command);

    std::stringstream answer;
    answer << "I've added " << CU << " CUs for the task " << task;
    return answer.str();
}

std::string UnicornBot::handleStatusCommand(const SleepyDiscord::Message& message)
{
    std::time_t result = std::time(nullptr);
    std::tm time = *std::localtime(&result);

    auto getWeekReport = [this, &message](const std::tm& time) -> std::string
    {
        std::pair<time_t, time_t> week = getWeekInterval(time);

        WorktimeTimeIntervalQuery query;
        query.Username = message.author.username;
        query.timestampFrom = week.first;
        query.timestampTo = week.second;

        std::vector<Worktime> res = handler.request(query);

        std::stringstream answer;
        int totalCU = 0;
        std::map<std::string, int> cuForTask;
        for (const auto& entry: res)
        {
            cuForTask[entry.Task] += entry.CU;
        }
        std::tm fromTime = *std::localtime(&week.first);
        std::tm toTime = *std::localtime(&week.second);

        answer << "Status on week from " << fromTime.tm_mday << "."
        << fromTime.tm_mon + 1 << "." << fromTime.tm_year + 1900 << " to "
        << toTime.tm_mday << "."
        << toTime.tm_mon + 1 << "." << toTime.tm_year + 1900 << std::endl;
        for (const auto& task: cuForTask)
        {
            totalCU += task.second;
            answer << task.first << " : " << task.second << std::endl;
        }
        answer << "Total CU for a week: " << totalCU << std::endl;
        return answer.str();
    };
    std::string currentWeekReport = getWeekReport(time);

    time.tm_mday -= 7;
    std::time_t prevWeekTimestamp = std::mktime(&time);
    std::tm prevWeekTime = *std::localtime(&prevWeekTimestamp);

    std::string prevWeekReport = getWeekReport(prevWeekTime);

    return prevWeekReport + currentWeekReport;
}

std::pair<time_t, time_t> getWeekInterval(const std::tm& timeData)
{
    int weekDay = timeData.tm_wday;
    int daysSinceMonday = 0;
    int daysToSunday = 0;
    if (weekDay != 0)
    {
        daysSinceMonday = weekDay - 1;
        daysToSunday = 7 - weekDay;
    }
    else
    {
        daysSinceMonday = 6;
        daysToSunday = 0;
    }

    std::tm monday = timeData;
    std::tm sunday = timeData;

    monday.tm_hour = 0;
    monday.tm_min = 0;
    monday.tm_sec = 1;
    monday.tm_mday -= daysSinceMonday;

    sunday.tm_hour = 23;
    sunday.tm_min = 59;
    sunday.tm_sec = 59;
    sunday.tm_sec = 59;
    sunday.tm_mday += daysToSunday;

    return std::pair<time_t, time_t>{std::mktime(&monday), std::mktime(&sunday)};
}
