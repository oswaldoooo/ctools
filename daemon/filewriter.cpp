#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <future>
#include <liburing.h>
#include <liburing/io_uring.h>
#include <mutex>
#include <unistd.h>
enum { READ = 1,
    WRITE };
typedef struct {
    int fd;
    unsigned short type;
    unsigned short other;
} info;
// async file writer
class filewriter {
private:
    int mission = 0;
    struct io_uring ring;
    struct io_uring_params iup;
    unsigned int queuesize = 10;
    std::mutex mu;
    std::thread* th = NULL;
    void init()
    {
        memset(&iup, 0, sizeof(iup));
        io_uring_queue_init_params(queuesize, &ring, &iup);
    }

public:
    filewriter()
    {
        init();
    }
    void post(int fd, const void* v, size_t size, __u64 off)
    {
        struct io_uring_sqe* sq = io_uring_get_sqe(&ring);
        if (sq == NULL)
            fprintf(stderr, "sqe is null\n");
        io_uring_prep_write(sq, fd, v, size, off);
        info in = { .fd = fd, .type = WRITE };
        memmove(&sq->user_data, &in, 8);
        mu.lock();
        mission++;
        printf("mission update %d\n", mission);
        mu.unlock();
    }
    int getevent(info* infos, unsigned int maxsize)
    {
        struct io_uring_cqe* cqequeue[maxsize];
        int count = io_uring_peek_batch_cqe(&ring, cqequeue, maxsize);
        if (count > 0) {
            for (size_t i = 0; i < count; i++) {
                memmove(&infos[i], &cqequeue[i]->user_data, 8);
            }
        }
        return count;
    }
    void start()
    {
        while (1) {
            if (mission > 0) {
                mu.lock();
                io_uring_submit(&ring);
                mission = 0;
                mu.unlock();
            }
        }
    }
    static void _start_(filewriter* fw)
    {
        printf("start ...\n");
        while (1) {
            if (fw->mission > 0) {
                fw->mu.lock();
                printf("submit mission %d\n", fw->mission);
                io_uring_submit(&fw->ring);
                fw->mission = 0;
                fw->mu.unlock();
            }
        }
    }
    void async_start()
    {
        this->th = new std::thread(_start_, this);
        this->th->detach();
    }
    ~filewriter()
    {
        if (th != NULL) {
        }
        io_uring_queue_exit(&ring);
    }
};

int main()
{
    int fd = open("test.txt", O_RDWR | O_CREAT, 0644);
    if (fd < 0) return -1;
    filewriter fl;
    fl.async_start();
    printf("start fwriter\n");
    fl.post(fd, "hello jim", 9, 0);
    info queue[10];
    sleep(1);
    int count = fl.getevent(queue, 10);
    printf("get count %d\n", count);
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            printf("event %d %d\n", queue[i].fd, queue[i].type);
        }
    }
    return 0;
}
