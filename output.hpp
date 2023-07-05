#pragma once
#include <bits/types/FILE.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <vector>
#define default_out_file "out.log"
int stdfd = STDOUT_FILENO;
#define default_template "[%s] %s %s"
#define default_template_two "[%s] %s"
#define default_template_three "[%s]"
struct prefix {
    std::string prefixname;
    bool time;
    std::vector<std::string> tags;
    std::string string()
    {
        std::string ans;
        if (!prefixname.empty()) {
            ans += "[" + prefixname + "] ";
        }
        if (time) {
            time_t ti = std::time(nullptr);
            ans += std::to_string(ti) + " ";
        }
        if (!tags.empty()) {
            std::string newans = "(";
            for (std::string& value : tags) {
                newans += value + " ";
            }
            newans += ")";
            ans += newans;
        }
        ans += ": ";
        return ans;
    }
};
void set_release(const char* target = default_out_file);
void output(const char* data);
void outputwithprefix(struct prefix*, const char* data);

void set_release(const char* target)
{
    int fid = open(target, O_WRONLY);
    stdfd = fid;
}
void output(const char* data)
{
    FILE* fil = fdopen(stdfd, "w");
    fprintf(fil, data);
}
void outputwithprefix(struct prefix* pf, const char* data)
{
    std::string finalans;
    finalans = pf->string();
    finalans += data;
    printf("prepare input data %s\n", pf->string().c_str());
    FILE* fil = fdopen(stdfd, "w");
    fprintf(fil, finalans.c_str());
}
