#include "Bot.h"

int main() {
    UnicornBot client(botToken, SleepyDiscord::USER_CONTROLED_THREADS);
    client.run();
    return 0;
}
