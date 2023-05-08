#pragma once
#ifndef cinsecure_h
#define cinsecure_h
#define BACKSPACE 0x08
void getInput(char *);
void getLine(char *buffer,bool &is_input,bool &force_end);
bool getLineUltra(char *,unsigned);
#endif