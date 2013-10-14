#ifndef _TEMPLATEQ_H
#define _TEMPLATEQ_H

#include <vector>
#include <pthread.h>
#include <assert.h>
#include <time.h>

template <class Token>
class VQueue {

public:
    // constructors and destructors
    VQueue(unsigned int size, char* file);
    ~VQueue();

    // mutators
    void add(Token& token);
    Token get();
    char* file();

private:
    std::vector<Token> buffer;
    unsigned int filled;    // # of objects in buffer
    unsigned int in;        // next frame
    unsigned int out;       // where to take from
    char *logfile;          //
    int prodcomplete;
    pthread_mutex_t mutex;
    pthread_cond_t nonEmpty;
    pthread_cond_t nonFull;
};

#endif
