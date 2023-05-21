#pragma once
#ifndef cinsecure_h
#define cinsecure_h
#define BACKSPACE 0x08
void getInput(char *);
//if sec is -1,it will always listen
void ListenKey(void(*funcp)(int),unsigned);
void getLine(char *buffer,bool &is_input,bool &force_end);
bool getLineUltra(char *,unsigned);
#endif