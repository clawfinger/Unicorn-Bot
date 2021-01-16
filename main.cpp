#include "Bot.h"
#include <fstream>

int main() {

    std::ifstream dataFile;
    std::string fileName("data.txt");

    dataFile.open(fileName);
    if (!dataFile.is_open())
    {
        std::cout << "Failed to load " << fileName << ". Bot will not start!";
        return -1;
    }
    std::stringstream s_stream;

    s_stream << dataFile.rdbuf();

    std::string botToken;
    s_stream >> botToken;

    UnicornBot client(botToken, SleepyDiscord::USER_CONTROLED_THREADS);
    client.run();
    return 0;
}
