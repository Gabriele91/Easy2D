
Mutex::Mutex(){
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex,&attr);
}
Mutex::~Mutex(){
	pthread_mutex_destroy(&mutex);
}
bool Mutex::lock(bool block){
	if(mutex){
		if(block)
			return pthread_mutex_lock(&mutex) == 0;
		else
			return pthread_mutex_trylock(&mutex) == 0;	
	}
	return false;	
}
void Mutex::unlock(){
	if(mutex)
		pthread_mutex_unlock(&mutex);
} 
