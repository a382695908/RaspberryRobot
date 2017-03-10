//
//  main.cpp
//  dachuang_car_xcode
//
//  Created by 黄正跃 on 5/12/16.
//  Copyright © 2016 黄正跃. All rights reserved.
//


#include <csignal>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <fcntl.h>

#include "RaspiVision.h"
#include "StreamOutputer.h"
#include "ImageProcess.h"

void StopSignalHandler(int sig)
{
	//Release all resource
	rv::RaspiVision::Release();
}

int main(int argc, char* argv[])
{
	//Ignore SIGPIPE (send by OS if transmitting to closed TCP sockets)
	//Don't ask me why. I don't know, either.
	std::signal(SIGPIPE, SIG_IGN);
	//Set handler for interupt signal
	std::signal(SIGINT, StopSignalHandler);

    StreamOutputer* streamOutputer=StreamOutputer::getInstance();
	rv::RaspiVision& rsv = rv::RaspiVision::GetInstance();
	pause();

	return 0;
}
