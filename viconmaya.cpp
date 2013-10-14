/**
    C++ client example using sockets
    http://www.binarytides.com/code-a-simple-socket-client-class-in-c/

    Modified - GBG 9/2013 - CpSc6120
**/

#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <unistd.h>
#include <pthread.h>

#include "tcp.h"
#include "ViconData.h"
#include "VQueue.cpp"

#define MAX_TOKENS_PER_LINE     50
#define MAX_LINE_BUFFER         10000

using namespace std;

typedef VQueue<ViconData> Buffer;

extern "C" int getBoolean(char *tf) {
    if(strcmp(tf, "True") == 0) return 1;
    return 0;
}

extern "C" int parseData(int maxtokens, char *buf, char **token, ViconData *t) {
    int n = 0;
    int i = 0;

    token[0] = strtok(buf, ",");

    // trim out the dummy frames we seem to get
    if(token[0] && atoi(token[0]) < 1000000) {
        for( n = 1; n < maxtokens; n++) {
            token[n] = strtok(0, ",");
            if(!token[n]) break;
        }

        if(i < n) {
            // parse data as best as possible.
            (*t).frameNumber = atoi(token[i++]);
            (*t).frameRate = atoi(token[i++]);

            if(i < n) {
                if(strcmp(token[i],"Head") == 0 && strcmp(token[i+1], "Eye-R") != 0) {
                    (*t).headPosition[0] = atof(token[++i]);
                    (*t).headPosition[1] = atof(token[++i]);
                    (*t).headPosition[2] = atof(token[++i]);

                    (*t).headPOccluded = getBoolean(token[++i]);

                    (*t).headRotation[0] = atof(token[++i]);
                    (*t).headRotation[1] = atof(token[++i]);
                    (*t).headRotation[2] = atof(token[++i]);

                    (*t).headROccluded = getBoolean(token[++i]);
                }

                if(++i < n) {
                    if(strcmp(token[i],"Eye-R") == 0 && strcmp(token[i+1], "Eye-L") != 0) {
                        (*t).eyeRXY[0] = atof(token[++i]);
                        (*t).eyeRXYOccluded[0] = getBoolean(token[++i]);
                        (*t).eyeRXY[1] = atof(token[++i]);
                        (*t).eyeRXYOccluded[1] = getBoolean(token[++i]);

                        (*t).eyeRXYZ[0] = atof(token[++i]);
                        (*t).eyeRXYZOccluded[0] = getBoolean(token[++i]);
                        (*t).eyeRXYZ[1] = atof(token[++i]);
                        (*t).eyeRXYZOccluded[1] = getBoolean(token[++i]);
                        (*t).eyeRXYZ[2] = atof(token[++i]);
                        (*t).eyeRXYZOccluded[2] = getBoolean(token[++i]);
                    } else { (*t).blink = true; }
                    if(++i < n) {
                        if(strcmp(token[i],"Eye-L") == 0 && strcmp(token[i+1], "EyePG") != 0) {
                            (*t).eyeLXY[0] = atof(token[++i]);
                            (*t).eyeLXYOccluded[0] = getBoolean(token[++i]);
                            (*t).eyeLXY[1] = atof(token[++i]);
                            (*t).eyeLXYOccluded[1] = getBoolean(token[++i]);

                            (*t).eyeLXYZ[0] = atof(token[++i]);
                            (*t).eyeLXYZOccluded[0] = getBoolean(token[++i]);
                            (*t).eyeLXYZ[1] = atof(token[++i]);
                            (*t).eyeLXYZOccluded[1] = getBoolean(token[++i]);
                            (*t).eyeLXYZ[2] = atof(token[++i]);
                            (*t).eyeLXYZOccluded[2] = getBoolean(token[++i]);
                        } else { (*t).blink = true; }

                        if(++i < n) {
                            if(strcmp(token[i],"EyePG") == 0 && token[i+1]) {
                                (*t).eyeRPosition[0] = atof(token[++i]);
                                (*t).eyeRPosition[1] = atof(token[++i]);
                                (*t).eyeRPosition[2] = atof(token[++i]);

                                (*t).eyeRPOccluded = getBoolean(token[++i]);

                                (*t).eyeRGaze[0] = atof(token[++i]);
                                (*t).eyeRGaze[1] = atof(token[++i]);
                                (*t).eyeRGaze[2] = atof(token[++i]);

                                (*t).eyeRGazeOccluded = getBoolean(token[++i]);

                                (*t).eyeLPosition[0] = atof(token[++i]);
                                (*t).eyeLPosition[1] = atof(token[++i]);
                                (*t).eyeLPosition[2] = atof(token[++i]);

                                (*t).eyeLPOccluded = getBoolean(token[++i]);

                                (*t).eyeLGaze[0] = atof(token[++i]);
                                (*t).eyeLGaze[1] = atof(token[++i]);
                                (*t).eyeLGaze[2] = atof(token[++i]);

                                (*t).eyeLGazeOccluded = getBoolean(token[++i]);
                            }
                        }
                    }
                }
            }
        }
    }

    (*t).ConvertEyeData();

    return n;
}

extern "C" void* readFrames(void *arg) {
    Buffer* buffer = (Buffer*) arg;

    ifstream log;
    string line;
    ViconData temp, nullvd;
    int n;

    log.open(buffer->file());

    if(!log.good()) {
        fprintf(stderr, "error opening parameter file %s\n", buffer->file());
        exit(1);
    }

    while(!log.eof()) {
        temp.Reset();

        char buf[MAX_LINE_BUFFER];
        log.getline(buf, MAX_LINE_BUFFER);

        char *token[MAX_TOKENS_PER_LINE];

        if(log.eof()) break;

        n = parseData(MAX_TOKENS_PER_LINE, buf, token, &temp);

        if(!(temp==nullvd)) {
            cout << "PRODUCER: " << endl;
            temp.PrintData();
            buffer->add(temp);
        }
        //sleep(1);
    }

    buffer->complete();

    return 0;
}

extern "C" void* sendFrames(void *arg) {
    Buffer* buffer = (Buffer*) arg;

    ViconData frame;

    frame = buffer->get();
    while(!frame.IsEmpty()) {
        cout << "CONSUMER: " << endl;
        frame.PrintData();

        frame.Reset();
        frame = buffer->get();

        frame.PrintData();
    }

    cout << "I GOT HERE" << endl;
    return 0;
}


int main(int argc , char *argv[]) {
    if(argc != 3) { cout << "no file to read in/write to" << endl; exit(1); }

    VQueue<ViconData> buffer(10000, argv[1]);

    pthread_t prod;
    pthread_t cons;


    pthread_create(&prod, 0, readFrames, &buffer);
    pthread_create(&cons, 0, sendFrames, &buffer);

    void* resultp;
    pthread_join(prod, &resultp);
    pthread_join(cons, &resultp);


    /**
    tcp_client c;
    string host;

    cout<<"Enter hostname : ";
    cin>>host;

    //connect to host
    c.conn(host , 80);

    //send some data
    c.send_data("GET / HTTP/1.1\r\n\r\n");

    //receive and echo reply
    cout<<"----------------------------\n\n";
    cout<<c.receive(1024);
    cout<<"\n\n----------------------------\n\n";

    //done
    **/
    return 0;
}
