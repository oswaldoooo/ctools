#include "iomuti.h"
#include <pthread.h>
#include <sys/event.h>
static pthread_mutex_t ev_mux = PTHREAD_MUTEX_INITIALIZER;
static struct kevent ev;
int iomutil_create(unsigned int maxsize)
{
    int kid = kqueue();
    if (kid > 0) return kid;
    return -1;
}

int iomutil_add(int ioid, int fd, int flags)
{
    pthread_mutex_lock(&ev_mux);
    int rflag = 0;
    int status;
    if (flags & IOIN) {
        rflag |= EVFILT_READ;
    }
    if (flags & IOUT) {
        rflag |= EVFILT_WRITE;
    }
    if (flags & IOERR) {
        rflag |= EVFILT_EXCEPT;
    }
    EV_SET(&ev, fd, rflag, EV_ADD | EV_ENABLE, 0, 0, NULL);
    status = kevent(ioid, &ev, 1, NULL, 0, NULL);
    pthread_mutex_unlock(&ev_mux);
    return status;
}

int iomutil_del(int ioid, int fd)
{
    int status;
    pthread_mutex_lock(&ev_mux);
    EV_SET(&ev, fd, 0, EV_DELETE, 0, 0, NULL);
    status = kevent(ioid, &ev, 1, NULL, 0, NULL);
    pthread_mutex_unlock(&ev_mux);
    return status;
}

int iomuti_wait(int ioid, struct io_event* queue, size_t maxsize, int timeout)
{
    struct kevent evenqueue[maxsize];
    struct timespec tiout;
    struct timespec* tioutptr = NULL;
    if (timeout > 0) {
        tiout.tv_sec = timeout >> 9;
        tiout.tv_nsec = timeout % (1 << 9);
        tioutptr = &tiout;
    }
    int size = kevent(ioid, NULL, 0, evenqueue, maxsize, tioutptr);
    if (size > 0) {
        for (size_t i = 0; i < size; i++) {
            queue[i].fd = evenqueue[i].ident;
            if (evenqueue[i].fflags & EVFILT_READ) {
                queue[i].rflag |= IOIN;
            }
            if (evenqueue[i].fflags & EVFILT_WRITE) {
                queue[i].rflag |= IOUT;
            }
            if (evenqueue[i].fflags & EVFILT_EXCEPT) {
                queue[i].rflag |= IOERR;
            }
        }
    }
    return size;
}