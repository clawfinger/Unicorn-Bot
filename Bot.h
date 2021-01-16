#pragma once
#include <ctime>
#include "sleepy_discord/sleepy_discord.h"
#include "Deps/sqliteWrapper/DBAdapter.h"
#include "Dispatcher.h"
#include <map>

static std::string botToken("Nzk2NjcxODkzODY4MjQ5MTIw.X_bUhA.TMEzBpupvl9mZ9cxiqOCS0VZgIw");

class UnicornBot : public SleepyDiscord::DiscordClient {
public:
    using SleepyDiscord::DiscordClient::DiscordClient;
    UnicornBot(const std::string token, const char numOfThreads = SleepyDiscord::DEFAULT_THREADS);
    void onMessage(SleepyDiscord::Message message) override;

private:
    std::string handleHelpCommand();
    std::string handleAddCommand(std::stringstream& ss, const SleepyDiscord::Message& message);
    std::string handleStatusCommand(const SleepyDiscord::Message& message);

private:
    bool isDBready;
    DBAdapter db;
    Dispatcher disp;
    QueryHandler handler;
};
