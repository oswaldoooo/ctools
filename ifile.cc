// ifile create by Oswaldo Cho 10 july 2023
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <gflags/gflags.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#define docarechar "."
DEFINE_string(cmd, "", "--cmd");
// parendir is current file's parent directory's name
void dofile(const char* parendir, const char* filename)
{
    std::string fullpath;
    if (strlen(parendir) == 0) {
        fullpath = filename;
    } else {
        fullpath = parendir;
        fullpath += "/";
        fullpath += filename;
    }
    struct stat st;
    if (stat(fullpath.c_str(), &st) < 0) {
        printf("%s not exist\n", fullpath.c_str());
        return;
    }
    if (S_ISDIR(st.st_mode)) {
        DIR* dirinfo = opendir(filename);
        if (dirinfo == NULL) {
            printf("open dir %s failed\n", filename);
            return;
        }
        struct dirent* finfo;
        while (true) {
            finfo = readdir(dirinfo);
            if (finfo == NULL) break;
            if (strncmp(docarechar, finfo->d_name, sizeof(char) * strlen(docarechar)) != 0) dofile(fullpath.c_str(), finfo->d_name);
        }
    } else {
        char newcmd[100];
        memset(newcmd, 0, sizeof(char) * 100);
        snprintf(newcmd, sizeof(char) * 100, FLAGS_cmd.c_str(), fullpath.c_str());
        system(newcmd);
    }
}
int main(int argc, char** argv)
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    if (argc > 1) {
        if (FLAGS_cmd.length() < 1) {
            struct stat st;
            char cmdfile[] = "cmd.txt";
            if (stat(cmdfile, &st) >= 0) {
                int fid = open(cmdfile, O_RDONLY);
                if (fid > 0) {
                    char rdbuffer[st.st_size + 1];
                    memset(rdbuffer, 0, sizeof(char) * st.st_size);
                    if (read(fid, rdbuffer, sizeof(char) * st.st_size) > 0) {
                        FLAGS_cmd = rdbuffer;
                    } else {
                        return 1;
                    }
                } else {
                    return 1;
                }
            } else {
                return 1;
            }
        }
        char currentdir[] = "";
        for (size_t i = 1; i < argc; i++) {
            dofile(currentdir, argv[i]);
        }
    } else
        std::cerr << "not input any args" << std::endl;
    return 0;
}