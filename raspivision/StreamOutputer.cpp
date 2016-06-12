//
//  StreamOutputer.cpp
//  raspivision
//
//  Created by WangHan.SCU on 5/1/16.
//  Copyright © 2016 SCU. All rights reserved.
//

#include "StreamOutputer.h"

StreamOutputer::StreamOutputer(){
    this->port=htons(this->port);
    this->raspivision=&(rv::RaspiVision::GetInstance());
    serverThread=new std::thread(run,this);
    serverThread->detach();
}

void StreamOutputer::run(StreamOutputer* that){

    log_<<"server thread run \n";

    int temp_fd=0;//temp file descriptor for every client
    int sd=0;//server socket file descriptor
    std::thread* tempClientThread=nullptr;
    struct sockaddr_in addr, client_addr;


    socklen_t addr_len = sizeof(struct sockaddr_in);
    sd=socket(PF_INET, SOCK_STREAM, 0);

    if(sd<0){
        std::cerr<<"error to create server_socket"<<std::endl;
        //***
        exit(0);
    }

    log_<<"server_socket created!\n";

    int on=1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        std::cerr<<"error to set serversocket opt"<<std::endl;
        //***
        exit(0);
    }

    /* configure server address to listen to all local IPs */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = that->port; /* is already in right byteorder */
    addr.sin_addr.s_addr = htonl(INADDR_ANY);


    if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 ) {
        std::cerr<<"error to bind serversocket"<<std::endl;
        //***
        exit(0);

    }

    log_<<"server_socket binded\n";


    /* start listening on socket */
    //10 means the length of request queue
    if ( listen(sd, 10) != 0 ) {
        std::cerr<<"error to bind serversocket"<<std::endl;
        //***
        exit(0);

    }


    /* create a child for every client that connects */
    while(!that->raspivision->isStop()){

        log_<<"server listenning \n";
        temp_fd = accept(sd, (struct sockaddr *)&client_addr, &addr_len);

        tempClientThread=new std::thread(runClient,that,temp_fd);
        that->clientThreads.push_back(tempClientThread);
        //???
        tempClientThread->detach();
    }

    log_<<"server stop\n";

}



void StreamOutputer::runClient(StreamOutputer* that,int fd){

    log_<<"client run on "<<fd<<std::endl;
    int cnt;
    char buffer[BUFFER_SIZE]={0}, *pb=buffer;
    iobuffer iobuf;
    request req;

    /* initializes the structures */
    that->init_iobuffer(&iobuf);
    that->init_request(&req);


    /* What does the client want to receive? Read the request. */
    memset(buffer, 0, sizeof(buffer));
    if ( (cnt = that->_readline(fd, &iobuf, buffer, sizeof(buffer)-1, 5)) == -1 ) {
        close(fd);
        return;
    }
    /* determine what to deliver */
    if ( strstr(buffer, "GET /?action=stream") != NULL ) {
        req.type = A_STREAM;
		that->send_stream(fd);
		close(fd);
    }
	else if((pb = strstr(buffer, "GET /?action=directionCommand")) !=NULL){
        //size of command
		pb += strlen("GET /?action=directionCommand");
        int len=0;
        while(pb[len]!=' '){
            ++len;
        }

		//get the command
        char *parameter=new char[len+2];
        strncpy(parameter,pb,len);
		parameter[len]='\0';
        //deubg
        //std::cout<<"old parameter: "<<parameter<<"\n";

        std::string mParameter = parameter;
        std::string old_value="%22";
        std::string new_value="\"";

        for(std::string::size_type pos(0); pos!=std::string::npos; pos+=new_value.length())   {
            if( (pos=mParameter.find(old_value,pos))!=std::string::npos )
                mParameter.replace(pos,old_value.length(),new_value);
            else
                break;
       }

       old_value="&";
       new_value="";
       for(std::string::size_type pos(0);   pos!=std::string::npos;   pos+=new_value.length())   {
           if(   (pos=mParameter.find(old_value,pos))!=std::string::npos   )
              mParameter.replace(pos,old_value.length(),new_value);
           else
              break;
       }
        //deubg
        std::cout<<"new parameter: "<<mParameter<<"\n";

        //determine what to deal with  command
        that->deal_dirctionCommand(fd,mParameter.c_str());

        //debug
        //std::cout<<"deal successful!"<<std::endl;
        delete[] parameter;
        //debug
        //std::cout<<"delete successful!"<<std::endl;
        close(fd);
        //debug
        //std::cout<<"close successful!"<<std::endl;
    }

    else if((pb = strstr(buffer, "GET /?action=imageCommand")) !=NULL){
        /*size of command*/
		pb += strlen("GET /?action=imageCommand");
        int len=0;
        while(pb[len]!=' '){
            ++len;
        }
        //debug
        //log_<<"len: "<<len<<"\n";

		/*get the command*/
        char *parameter=new char[len+2];
        strncpy(parameter,pb,len);
		parameter[len]='\0';
        //deubg
        //std::cout<<"old parameter: "<<parameter<<"\n";

        std::string mParameter = parameter;
	    std::string old_value="%22";
	    std::string new_value="\"";

	    for(std::string::size_type   pos(0);   pos!=std::string::npos;   pos+=new_value.length())   {
		    if( (pos=mParameter.find(old_value,pos))!=std::string::npos   )
			    mParameter.replace(pos,old_value.length(),new_value);
		    else
                break;
	   }

	    old_value="&";
	    new_value="";
	    for(std::string::size_type   pos(0);   pos!=std::string::npos;   pos+=new_value.length())   {
		    if(   (pos=mParameter.find(old_value,pos))!=std::string::npos   )
			   mParameter.replace(pos,old_value.length(),new_value);
		    else
			   break;
	    }

        //deubg
        std::cout<<"new parameter: "<<mParameter<<"\n";

		/*determine what to deal with  command*/
        that->deal_ImageCommand(fd,mParameter.c_str());

        delete[] parameter;
        close(fd);
    }
    else{
        std::cerr<<"wrong parameter!\n";

		char buf[256]={0};
        buf[255]='\0';
        sprintf(buf, "HTTP/1.0 200 OK\r\n" \
                  "Content-type: text/plain\r\n" \
                  STD_HEADER \
                  "\r\n%s","wrong command!\n");
        if(write(fd, buf, strlen(buf))<0){
            std::cerr<<"wrong to send response!\n";
        }

        close(fd);
		return;
    }
}


/****************************
*	deal with dirction command
*
*****************************/
void StreamOutputer::deal_dirctionCommand(int fd, const char* parameter){
    //debug
    log_<<"client "<<fd<<" deal_dirctionCommand\n";

    Json::Reader reader;
    Json::Value root;

	int leftWheelSpeed = 0;
	int rightWheelSpeed = 0;


    if(reader.parse(parameter,root))
    {
        leftWheelSpeed = root["LeftWheelSpeed"].asInt();
        rightWheelSpeed = root["RightWheelSpeed"].asInt();
    }

	//debug
	log_<<"leftWheelSpeed: "<<leftWheelSpeed<<"\n";
    log_<<"rightWheelSpeed: "<<rightWheelSpeed<<"\n";

    //CarHardware* car=CarHardware::getInstance();
    /*
    *
    *
    *未完成
    *
    */
    char buf[256]={0};
    buf[255]='\0';
    sprintf(buf, "HTTP/1.0 200 OK\r\n" \
              "Content-type: text/plain\r\n" \
              STD_HEADER \
              "\r\n","direction parameter changed!\n");
    if(write(fd, buf, strlen(buf))<0){
        std::cerr<<"wrong to send direction info!\n";
    }

    //debug
    log_<<"write  successful!\n";
}


/****************************
*	deal with image command
*
*****************************/
void StreamOutputer::deal_ImageCommand(int fd, const char* parameter){
    //debug
    log_<<"client "<<fd<<" deal_ImageCommand\n";

    Json::Reader reader;
    Json::Value root;

    int mBrightness = 55;
    int mContrast = 50;
    int mSaturation = 50;
    int mWidth = 320;
    int mHeight = 240;

    if(reader.parse(parameter,root))
    {
        mBrightness = root["BRIGHTNESS"].asInt();
        mContrast = root["CONTRAST"].asInt();
        mSaturation = root["SATURATION"].asInt();
        mWidth = root["WIDTH"].asInt();
        mHeight = root["HEIGHT"].asInt();

    }

	//debug
	std::cout<<"brightness: "<<mBrightness <<std::endl;
	std::cout<<"contrast: "<<mContrast <<std::endl;
	std::cout<<"saturation: "<<mSaturation <<std::endl;
	std::cout<<"width: "<<mWidth <<std::endl;
	std::cout<<"height: "<<mHeight <<std::endl;

    rv::RaspiVision& raspi = rv::RaspiVision::GetInstance();

    if (raspi.getImageBrightness() != mBrightness)
        raspi.setImageBrightness(mBrightness);
    if (raspi.getImageContrast() != mContrast)
        raspi.setImageContrast(mContrast);
    if (raspi.getImageSaturation() != mSaturation)
        raspi.setImageSaturation(mSaturation);
    if (raspi.getImageWidth() !=mWidth || raspi.getImageHeight() !=mHeight)
        raspi.setImageSize(mHeight,mWidth);


    char buf[256]={0};
    buf[255]='\0';
    sprintf(buf, "HTTP/1.0 200 OK\r\n" \
              "Content-type: text/plain\r\n" \
              STD_HEADER \
              "\r\n","image parameter changed!\n");
    if(write(fd, buf, strlen(buf))<0){
        std::cerr<<"wrong to send image info!\n";
    }

    //debug
    log_<<"write successful!\n";
}

/************************************
*	send image to web brower
* 	@para fd : cliend id
*
***********************************/

void StreamOutputer::send_stream(int fd){
    //debug
    bool testFlag=true;

    log_<<"start to send stream "<<fd<<std::endl;
    std::vector<uchar> imageBuf;
    cv::Mat tempImage;

    char buffer[BUFFER_SIZE] = {0};

    sprintf(buffer, "HTTP/1.0 200 OK\r\n" \
            STD_HEADER \
            "Content-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n" \
            "\r\n" \
            "--" BOUNDARY "\r\n");

    if ( write(fd, buffer, strlen(buffer)) < 0 ) {
        std::cerr<<"client write error"<<std::endl;
        return;
    }

    while(!this->raspivision->isStop()){


        tempImage = this->raspivision->dupImage();



        sprintf(buffer, "Content-Type: image/jpeg\r\n" \
                "Content-Length: %d\r\n" \
                "\r\n", tempImage.cols * tempImage.rows);

        if ( write(fd, buffer, strlen(buffer)) < 0 ) break;

        cv::imencode(".jpg", tempImage, imageBuf, std::vector<int>());
        //debug
        if(testFlag){
            testFlag=false;
            cv::imwrite ("imageTest.jpg",tempImage );
            log_<<"tempImage saved!\n";
        }


        if( write(fd, imageBuf.data(), imageBuf.size()) < 0 ) break;

        sprintf(buffer, "\r\n--" BOUNDARY "\r\n");
        if ( write(fd, buffer, strlen(buffer)) < 0 ) break;


    }



}


/*******************************************
*	get singl instance
*
********************************************/
StreamOutputer* StreamOutputer::getInstance(){

    static StreamOutputer* streamOutputer=nullptr;
    if(nullptr!=streamOutputer){
        return streamOutputer;
    }
    else{
        streamOutputer=new StreamOutputer();
        return streamOutputer;
    }
}

/******************************************************************************
 Description.: initializes the iobuffer structure properly
 Input Value.: pointer to already allocated iobuffer
 Return Value: iobuf
 ******************************************************************************/
void StreamOutputer::init_iobuffer(iobuffer *iobuf) {
    memset(iobuf->buffer, 0, sizeof(iobuf->buffer));
    iobuf->level = 0;
}

/******************************************************************************
 Description.: initializes the request structure properly
 Input Value.: pointer to already allocated req
 Return Value: req
 ******************************************************************************/
void StreamOutputer::init_request(request *req) {
    req->type        = A_UNKNOWN;
    req->parameter   = NULL;
    req->client      = NULL;
    req->credentials = NULL;
}

/******************************************************************************
 Description.: If strings were assigned to the different members free them
 This will fail if strings are static, so always use strdup().
 Input Value.: req: pointer to request structure
 Return Value: -
 ******************************************************************************/
void StreamOutputer::free_request(request *req) {
    if ( req->parameter != NULL ) free(req->parameter);
    if ( req->client != NULL ) free(req->client);
    if ( req->credentials != NULL ) free(req->credentials);
}


/******************************************************************************
 Description.: read with timeout, implemented without using signals
 tries to read len bytes and returns if enough bytes were read
 or the timeout was triggered. In case of timeout the return
 value may differ from the requested bytes "len".
 Input Value.: * fd.....: fildescriptor to read from
 * iobuf..: iobuffer that allows to use this functions from multiple
 threads because the complete context is the iobuffer.
 * buffer.: The buffer to store values at, will be set to zero
 before storing values.
 * len....: the length of buffer
 * timeout: seconds to wait for an answer
 Return Value: * buffer.: will become filled with bytes read
 * iobuf..: May get altered to save the context for future calls.
 * func().: bytes copied to buffer or -1 in case of error
 ******************************************************************************/
template<typename T>
int StreamOutputer::_read(int fd, iobuffer *iobuf, T *buffer, size_t len, int timeout) {
    int copied=0, rc, i;
    fd_set fds;
    struct timeval tv;

    memset(buffer, 0, len);

    while ( (copied < len) ) {
        i = _MIN(iobuf->level, len-copied);
        memcpy(buffer+copied, iobuf->buffer+IO_BUFFER-iobuf->level, i);

        iobuf->level -= i;
        copied += i;
        if ( copied >= len )
            return copied;

        /* select will return in case of timeout or new data arrived */
        tv.tv_sec = timeout;
        tv.tv_usec = 0;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        if ( (rc = select(fd+1, &fds, NULL, NULL, &tv)) <= 0 ) {
            if ( rc < 0)
                exit(EXIT_FAILURE);

            /* this must be a timeout */
            return copied;
        }

        init_iobuffer(iobuf);

        /*
         * there should be at least one byte, because select signalled it.
         * But: It may happen (very seldomly), that the socket gets closed remotly between
         * the select() and the following read. That is the reason for not relying
         * on reading at least one byte.
         */
        if ( (iobuf->level = read(fd, &iobuf->buffer, IO_BUFFER)) <= 0 ) {
            /* an error occured */
            return -1;
        }

        /* align data to the end of the buffer if less than IO_BUFFER bytes were read */
        memmove(iobuf->buffer+(IO_BUFFER-iobuf->level), iobuf->buffer, iobuf->level);
    }

    return 0;
}

/******************************************************************************
 Description.: Read a single line from the provided fildescriptor.
 This funtion will return under two conditions:
 * line end was reached
 * timeout occured
 Input Value.: * fd.....: fildescriptor to read from
 * iobuf..: iobuffer that allows to use this functions from multiple
 threads because the complete context is the iobuffer.
 * buffer.: The buffer to store values at, will be set to zero
 before storing values.
 * len....: the length of buffer
 * timeout: seconds to wait for an answer
 Return Value: * buffer.: will become filled with bytes read
 * iobuf..: May get altered to save the context for future calls.
 * func().: bytes copied to buffer or -1 in case of error
 ******************************************************************************/
/* read just a single line or timeout */
int StreamOutputer::_readline(int fd, iobuffer *iobuf, void *buffer, size_t len, int timeout) {

    char c='\0', *out=static_cast<char*>(buffer);
    int i;

    memset(buffer, 0, len);

    for ( i=0; i<len && c != '\n'; i++ ) {
        if ( _read(fd, iobuf, &c, 1, timeout) <= 0 ) {
            /* timeout or error occured */
            return -1;
        }
        *out++ = c;
    }

    return i;
}
