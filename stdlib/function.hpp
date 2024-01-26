#include <cstdio>
#include <functional>
#include <iostream>
#include <string>
class defer {
public:
  defer(std::function<void()> fe) : f(fe) {}
  template <class Function, class... Args>
  defer(Function &fe, Args &&...args) : f(std::bind(fe, args...)) {}
  ~defer() { f(); }

private:
  std::function<void()> f;
};
