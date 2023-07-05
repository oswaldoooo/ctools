#pragma once
#include <bits/types/FILE.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <ctime>
#include <stdexcept>
#include <string>
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
            // todo: time need format to string for add to prefix output
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

//new outputer,use outputer output to file or terminal.
class Outputer {
private:
    struct prefix* prf;
    int fid;

public:
    Outputer() { fid = STDOUT_FILENO; }
    Outputer(int tfid) { fid = tfid; }
    Outputer(const char* filepath)
    {
        fid = open(filepath, O_WRONLY | O_CREAT|O_APPEND, 0600);
        if (fid < 0) {
            throw std::logic_error("open output file failed");
        }
    }
    void output(const char* words)
    {
        int ok = write(fid, words, sizeof(char) * strlen(words));
        if (ok < 0) {
            throw std::logic_error("write to target file failed");
        }
    }
    void outputwithprefix(struct prefix* pf, const char* words) {
        std::string newans=pf->string();
        newans+=words;
        if (words[strlen(words)-1]!='\n'){
            newans+='\n';
        }
        int ok=write(fid, newans.c_str(), sizeof(char)*newans.length());
        if (ok<0){
            throw std::logic_error("write to target file failed");
        }
    }
};
