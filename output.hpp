#pragma once
#include <cstddef>
#include <fcntl.h>
#include <memory>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <ctime>
#include <stdexcept>
#include <string>
#include <vector>
#define default_out_file "out.log"
inline int stdfd = STDOUT_FILENO;
#define default_template "[%s] %s %s"
#define default_template_two "[%s] %s"
#define default_template_three "[%s]"
#define logout_with_prefix(x, y) outputplus_(x, y, __FILE__, __LINE__, __func__)
// output log info to stdout
#define logout(x) logout_(x, STDOUT_FILENO, __FILE__, __LINE__, __func__)
// output log info to target file
#define logout2(x, y) logout_(x, y, __FILE__, __LINE__, __func__)
#define debugout() debugOut(__FILE__, __LINE__, __func__)
// need free ans
inline const char* debugOut(const char* file, const int line, const char* funcname)
{
    char* ans = new char[50];
    memset(ans, 0, 50);
    time_t ti = time(NULL);
    char* timestr = ctime(&ti);
    timestr[strlen(timestr) - 1] = 0;
    snprintf(ans, 50, "%s %s:%s:%d", timestr, file, funcname, line);
    return ans;
}
struct prefix {
    std::string prefixname;
    bool time;
    std::vector<std::string> tags;
    std::string string(const char* pfcontent = NULL) const
    {
        std::string ans;
        if (pfcontent != NULL) ans = pfcontent;
        if (!prefixname.empty()) {
            ans += "[" + prefixname + "] ";
        }
        if (time) {
            // todo: time need format to string for add to prefix output
            time_t ti = std::time(nullptr);
            const char* cti = ctime(&ti);
            ans += cti;
            ans += " ";
        }
        if (!tags.empty()) {
            std::string newans = "(";
            for (const std::string& value : tags) {
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

inline void set_release(const char* target)
{
    int fid = open(target, O_WRONLY);
    stdfd = fid;
}
inline void output(const char* data)
{
    write(stdfd, data, sizeof(char) * strlen(data));
}
inline void outputwithprefix(struct prefix* pf, const char* data)
{
    std::string finalans;
    finalans = pf->string();
    finalans += data;
    printf("prepare input data %s\n", pf->string().c_str());
    write(stdfd, finalans.c_str(), sizeof(char) * finalans.length());
}
inline int outputplus_(struct prefix* pf, const char* data, const char* filename, const int line, const char* funcname)
{
    char content[50];
    memset(content, 0, 50);
    snprintf(content, 50, "%s:%s:%d ", filename, funcname, line);
    std::string finalans;
    finalans = pf->string(content).c_str();
    finalans += data;
    write(stdfd, finalans.c_str(), finalans.length());
    return 0;
}
// new outputer,use outputer output to file or terminal.
class Outputer {
private:
    struct prefix* prf;
    int fid;
    std::string outpath;

public:
    Outputer() { fid = STDOUT_FILENO; }
    Outputer(int tfid) { fid = tfid; }
    ~Outputer()
    {
        if (fid != STDOUT_FILENO) {
            close(fid);
        }
    }
    Outputer(const char* filepath)
    {
        fid = open(filepath, O_WRONLY | O_CREAT | O_APPEND, 0600);
        if (fid < 0) {
            throw std::logic_error("open output file failed");
        }
        outpath = filepath;
    }
    void output(const char* words)
    {
        int ok = write(fid, words, sizeof(char) * strlen(words));
        if (ok < 0) {
            throw std::logic_error("write to target file failed");
        }
    }
    void outputwithprefix(const struct prefix* pf, const char* words)
    {
        std::string newans = pf->string();
        newans += words;
        if (words[strlen(words) - 1] != '\n') {
            newans += '\n';
        }
        int ok = write(fid, newans.c_str(), sizeof(char) * newans.length());
        if (ok < 0) {
            throw std::logic_error("write to target file failed");
        }
    }
    std::unique_ptr<char[]> getmod()
    {
        std::unique_ptr<char[]> ans = std::unique_ptr<char[]>(new char[100]);
        memset(ans.get(), 0, sizeof(char) * 100);
        std::string ansstr;
        if (fid == STDOUT_FILENO) {
            ansstr = "stdout";
        } else {
            ansstr = outpath.empty() ? "unknown path" : outpath;
        }
        strncpy(ans.get(), ansstr.c_str(), sizeof(char) * ansstr.length());
        return ans;
    }
};
// out to target file description,format time file_name:function_name:line_number
inline int logout_(const char* data, const int fid, const char* filename, const int line, const char* funname)
{
    char datafinal[strlen(data) + 70];
    memset(datafinal, 0, strlen(data) + 70);
    time_t ti = time(NULL);
    char* timstr = ctime(&ti);
    timstr[strlen(timstr) - 1] = 0;
    snprintf(datafinal, strlen(data) + 70, "%s %s:%s:%d %s\n", timstr, filename, funname, line, data);
    write(fid, datafinal, strlen(datafinal));
    return 0;
}