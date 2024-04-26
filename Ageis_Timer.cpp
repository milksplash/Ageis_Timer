#include <iostream>
#include <string>
#include <vector>
#include "clipboardxx.hpp" //https://github.com/Arian8j2/ClipboardXX (commit 7c9ca9ac06e5b72ae8c6bcc334ad771c6613eaed)
// cannot implement without xx when unknown MM
bool isValid(std::string str, long long len)
{
    const char SMALLX = 'x';
    const char BIGX = 'X';
    if (len > 4 || len < 2)
    {
        return 0;
    }
    for (long long i = len; i > -1; i--)
    {
        if (len - i < 2)
        {
            if (!isdigit(str[i]) && str[i] != SMALLX && str[i] != BIGX)
            {
                return 0;
            }
            if (len - i == 1 && str[i] != SMALLX && str[i] != BIGX && str[i] - '0' > 5)
            {
                return 0;
            }
        }
        else
        {
            if (!isdigit(str[i]))
            {
                return 0;
            }
        }
    }
    return 1;
}

std::string genResult(std::string rosTime, long long len, int EXPIRE, int INTERVAL1, int INTERVAL2)
{
    int MM = stoi(rosTime.substr(0, len - 1));
    std::string SS = rosTime.substr(len - 1, 2);
    for (int i = 0; i < SS.size(); i++)
    {
        if (SS[i] == 120) // 'x'
        {
            SS[i] = 88; // 'X'
        }
    }
    std::string result = "expire " + std::to_string(MM + EXPIRE) + ":" + SS + " respawn " +
        std::to_string(MM + INTERVAL1) + ":" + SS + "-" + std::to_string(MM + INTERVAL2) + ":" + SS;
    return result;
}

void copyToClipboard(std::string string)
{
    clipboardxx::clipboard clipboard;
    clipboard << string;
    std::cout << "Copied to clipboard...\n";
}

int main(int argc, char* argv[])    
{
    const int EXPIRE = 5;
    const int INTERVAL1 = 8;
    const int INTERVAL2 = 11;
    std::vector<std::string> arguments(argv + 1, argv + argc);
    std::string rosTime;
    std::string result;

    do {
        if (arguments.size() == 0)
        {
            std::cout << "Enter ros time (MSS/MMSS/MMMSS, use XX/xx to indicate unknown minute): ";
            std::cin >> rosTime;
        }
        else
        {
            rosTime = arguments[0];
        }
        long long len = rosTime.length() - 1;
        if (!isValid(rosTime, len)) {
            std::cout << rosTime;
            printf(" is not a valid input. Input time in the format of MSS/MMSS/MMMSS.\n");
        }
        else
        {
            result = genResult(rosTime, len, EXPIRE, INTERVAL1, INTERVAL2);
            std::cout << result << "\n";
            copyToClipboard(result);
        }
    } while (arguments.size() == 0);
}