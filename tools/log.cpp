#include "log.h"
#include <cstdio>
#include <fstream>
#include <ios>
#include <ostream>
void to_log(const char *filename, const char *content){
	std::ofstream ofs;
	ofs.open(filename,std::ios::out|std::ios::app);
	ofs<<content<<std::endl;
	ofs.close();
}