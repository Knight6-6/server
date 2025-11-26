#include "thread_poll.hpp"

threadpoll::threadpoll(unsigned poll)
{
    for(unsigned i=0 ; i<poll ; i++)
    {
       workthread.emplace_back([this](){this->work();});
    }
}

threadpoll::~threadpoll()
{
   stop=true;
   cv.notify_all();
   for(auto &i: workthread)
   {
      if(i.joinable())
      {
         i.join();
      }
   } 
}

 void threadpoll::work()
{
    while(!stop)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> l(lock);
            cv.wait(l,[this](){return stop || !tasks.empty();});
            if(stop)
            {
                return;
            }
            task=tasks.front();
            tasks.pop();
        }
        task();
    }
}