#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <unistd.h>
#ifndef __def
#define __def
class defer {
public:
  defer(std::function<void()> fe) : f(fe) {}
  template <class Function, class... Args>
  defer(Function &fe, Args &&...args) : f(std::bind(fe, args...)) {}
  ~defer() { f(); }

private:
  std::function<void()> f;
};
void greet(int age) {
  // return 0;
  printf("age %d\n", age);
}
struct taskruntimeinfo {
  int *status;
  int *result;
};
#endif
// prerun 命令为启动任务，启动后状态会变更为running
enum { DROP = -2, UNUSED, PRERUN, RUNNING };
//任务容器，延时触发
class taskcontainer {
public:
  taskcontainer() : th(backend, this) {
    th.detach();
    std::unique_lock<std::mutex> ul(resource_mutex);
    cond.wait(ul);
  }
  //增加任务
  template <class Function, class... Args>
  void task_add(int *status, int *result, Function &fun, Args &&...args) {
    resource_mutex.lock();
    taskqueue.push_back(std::bind(fun, args...));
    statusqueue.push_back({status, result});
    resource_mutex.unlock();
  }
  void notify() { cond.notify_one(); }

protected:
  //后台系统
  static void backend(taskcontainer *tc) {
    std::mutex mu;
    std::unique_lock<std::mutex> ul(mu);
    size_t queuesize;
    printf("start backend\n");
    tc->cond.notify_one();
    while (1) {
      tc->cond.wait(ul);
      printf("range queue\n");
      tc->resource_mutex.lock();
      queuesize = tc->statusqueue.size();
      if (queuesize > 0) {
        for (size_t i = 0; i < queuesize; i++) {
          if (*tc->statusqueue[i].status == PRERUN) {
            *tc->statusqueue[i].status = RUNNING;
            std::async([tc, i]() {
              *tc->statusqueue[i].result = tc->taskqueue[i]();
              *tc->statusqueue[i].status = UNUSED;
            });
          } else if (*tc->statusqueue[i].status ==
                     DROP) { //设置状态前需判断状态是否在使用中
            tc->statusqueue.erase(tc->statusqueue.begin() + i);
            tc->taskqueue.erase(tc->taskqueue.begin() + i);
            queuesize--;
            i--;
          }
        }
      }
      tc->resource_mutex.unlock();
    }
  }

private:
  std::thread th;
  std::mutex resource_mutex;
  std::condition_variable cond;
  // std::map<int *, std::future<int>> resultmap;
  std::vector<struct taskruntimeinfo> statusqueue;
  std::vector<std::function<int()>> taskqueue;
};
int sayhello(std::string name) {
  printf("into sayhello %s\n", name.c_str());

  return 0;
}
int main() {
  taskcontainer tc;
  int st = UNUSED;
  int ans = -1;
  tc.task_add(&st, &ans, sayhello, "jesko");
  printf("add to task container\n");
  st = PRERUN;
  tc.notify();

  printf("process finished %d\n", ans);
  return 0;
}
