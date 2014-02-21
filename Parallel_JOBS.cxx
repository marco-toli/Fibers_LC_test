#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "TFile.h"
#include <cstdlib> 
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <threadpool.hpp>


using namespace std;
using boost::mutex;
using boost::thread;

//TO COMPILE "from6to5Root" 
//g++ -Wall -ansi rise_parallel.cxx -o rise_parallel `root-config --cflags --glibs` -I /usr/include/boost -lboost_thread-mt
//g++ -Wall -ansi rise_parallel.cxx -o rise_parallel `root-config --cflags --glibs` -lboost_thread-mt
void myThread(int number, string conf, string name) 
{	
	ostringstream temp1;
	temp1 << number;
	string command;	
	
	string rootFile = name + temp1.str();
	command = "$G4WORKDIR/bin/Linux-g++/Fibers_LC_test Fibers.cfg" + conf + " " + rootFile + " ";
	cout << command << endl;
	system(command.c_str());
};



int main(int argc, char** argv) 
{

    int n_cpu          = atoi(argv[1]);
    int n_processi     = atoi(argv[2]);
    string conf	       = argv[3];
    string filename    = argv[4];
    	
    boost::threadpool::thread_pool<> tpool(n_cpu);
    for(int j=0; j<n_processi; j++)
    {
      tpool.schedule(boost::bind(myThread, j, conf, filename));
    }
    tpool.wait();
    cout << "Done" << endl;

    return 0;
}
