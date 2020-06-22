#include<bits/stdc++.h>
#include "ThreadPool.cpp"
using namespace std;
void mytest(int po)
{
    cout<<po<<endl;
}
int main()
{
    ThreadPool pool1;
    pool1.start();
    int a = 100;
    while(a--)
    {
        int po = rand()%5+1;
        pool1.addTask(bind(&mytest, po),po);
    }
    pool1.stop();
}
