#pragma once
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <sys/types.h>
#include <unistd.h>
class prototransfer {
public:
  //notify object after update data
  virtual ssize_t transfer(void *src, size_t size) = 0;
};
class metadata {
public:
  metadata(void *_data, size_t _size, std::condition_variable *_cond) {}
  //release buffer which inused
  std::condition_variable *set(void *_data, size_t _size,
                               std::condition_variable *_cond) {
    return &selfcond;
  }
  void release() { selfcond.notify_one(); }

private:
  void *rawdata;
  std::condition_variable selfcond;
};
class temptransfer : public prototransfer {
public:
  ssize_t transfer(void *src, size_t size) {
    // transfer data
    ssize_t ansize;
    // notify waite object
    // std::condition_variable *mcond = md.set(src, ansize, &cond);
    cond.notify_one();
    //wait front object end
    std::unique_lock<std::mutex> ul(mu);
    mcond->wait(ul);
    return ansize;
  }
  ssize_t read(void *src, size_t size) {}

protected:
  char errno_[108];

private:
  std::condition_variable *mcond;
  metadata md;
  std::mutex mu;
  std::condition_variable cond;
};
class netio {
public:
  netio(int _fd) : fd(_fd) {}
  ssize_t write(void *data, size_t size) {
    data = (char *)data - 2;
    *(uint16_t *)data = size;
    return ::write(fd, data, size + 2);
  }
  ssize_t read(void *data, size_t size) {
    if (size < 2) {
      return 0;
    }
    ssize_t _size = ::read(fd, data, 2);
    if (_size == 2) {
      _size = ::read(fd, data, *(uint16_t *)data);
      return _size;
    }
    return 0;
  }
  ~netio() {
    if (fd > 0) {
      close(fd);
    }
  }
  static int bind(netio &ni, prototransfer &pt, size_t buffsize) {
    if (ni.fd <= 0)
      return -1;
    ssize_t size = 1;
    char buffer[buffsize];
    while (size) {
      size = ni.read(buffer + 2, buffsize);
      if (size > 0) {
        size = pt.transfer(buffer, size);
      }
    }
    return -1;
  }

private:
  int fd;
};
