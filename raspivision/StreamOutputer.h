//
//  StreamOutputer.h
//  raspivision
//
//  Created by Wang Han.SCU on 6/6/16.
//  Copyright © 2016 robotcloud. SCU. All rights reserved.
//

#ifndef StreamOutputer_h
#define StreamOutputer_h
#include "RaspiVision.h"
#include "CarHardware.h"
#include "dist/json/json.h"

#include <thread>
#include <vector>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>


#define IO_BUFFER 256
#define BUFFER_SIZE 1024
#define BOUNDARY "boundarydonotcross"
#define STD_HEADER "Connection: close\r\n" \
"Server: MJPG-Streamer/0.2\r\n" \
"Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\n" \
"Pragma: no-cache\r\n" \
"Expires: Mon, 3 Jan 2000 12:34:56 GMT\r\n"

#define _MIN(a, b) (((a) < (b)) ? (a) : (b))

/* the webserver determines between these values for an answer */
typedef enum { A_UNKNOWN, A_SNAPSHOT, A_STREAM, A_COMMAND, A_FILE } answer_t;

/*
 * the client sends information with each request
 * this structure is used to store the important parts
 */
typedef struct {
    answer_t type;
    char *parameter;
    char *client;
    char *credentials;
} request;

/* the iobuffer structure is used to read from the HTTP-client */
typedef struct {
    int level;              /* how full is the buffer */
    char buffer[IO_BUFFER]; /* the data */
} iobuffer;

//debug
//std::ostream& log_=std::cout;


class StreamOutputer{
private:
    rv::RaspiVision* raspivision;

    int port=8080;

    StreamOutputer();

    std::thread* serverThread;

    std::vector<std::thread*> clientThreads;

    static void run(StreamOutputer* that);

    static void runClient(StreamOutputer* that,int fd);

	void deal_dirctionCommand(int fd,char* parameter);

    void deal_ImageCommand(int fd,char* parameter);

    void send_stream(int fd);

    void init_iobuffer(iobuffer *iobuf);

    void init_request(request *req);

    void free_request(request *req);

    template<typename T>
    int _read(int fd, iobuffer *iobuf, T *buffer, size_t len, int timeout);

    int _readline(int fd, iobuffer *iobuf, void *buffer, size_t len, int timeout);



public:

    static StreamOutputer* getInstance();


};

#endif /* StreamOutputer_h */
