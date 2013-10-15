#include "VQueue.h"

using namespace std;

template<class Token>
VQueue<Token>::VQueue(unsigned int size, char* file) :
    buffer(size), logfile(file), filled(0), in(0), out(0)
{
    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&nonEmpty, 0);
    pthread_cond_init(&nonFull, 0);
}

template<class Token>
VQueue<Token>::~VQueue() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&nonEmpty);
    pthread_cond_destroy(&nonFull);
}

template<class Token>
void VQueue<Token>::add(Token& token) {
    pthread_mutex_lock(&mutex);
    if(filled == buffer.size()) {
        pthread_cond_wait(&nonFull, &mutex);
    }

    assert(filled < buffer.size());
    buffer[in] = token;
    in = (in + 1) % buffer.size();
    ++filled;

    pthread_cond_signal(&nonEmpty);
    pthread_mutex_unlock(&mutex);
}

template<class Token>
Token VQueue<Token>::get() {
    Token token;

    pthread_mutex_lock(&mutex);
    if(filled == 0) {
        pthread_cond_wait(&nonEmpty, &mutex);
    }

    assert(filled > 0);
    token= buffer[out];
    out = (out + 1) % buffer.size();
    --filled;

    pthread_cond_signal(&nonFull);
    pthread_mutex_unlock(&mutex);

    return token;
}

template<class Token>
char* VQueue<Token>::file() {
    return logfile;
}


