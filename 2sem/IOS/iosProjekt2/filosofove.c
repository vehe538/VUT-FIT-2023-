#include <stdio.h>
#include<unistd.h>
#include<thread>
#include<queue>
#include<mutex>

#define NUM 6


std::mutex zamky[NUM];




void f(int ID){
	printf("Filosof %i started\n",ID);
	//lock(zamky[ID],zamky[(ID+1)%NUM]);
	zamky[ID].lock();
	usleep(100);
	zamky[(ID+1)%NUM].lock();
	/*if (ID==(NUM-1)) {
		zamky[0].lock();
		zamky[ID].lock();
	} else {
		zamky[ID].lock();
		usleep(5);
		zamky[(ID+1)].lock();
	}*/
	printf("%i eating\n",ID);
	zamky[ID].unlock();
	zamky[(ID+1)%NUM].unlock();
	printf("%i done\n",ID);

}


int main() {
	std::thread * t[NUM];
	for(int i=0;i<NUM;i++){	
		t[i] = new std::thread (f,i);
	}
	for(int i=0;i<NUM;i++){
		(*(t[i])).join();
		delete t[i];
	}

	printf("everything finished\n");
	
}
