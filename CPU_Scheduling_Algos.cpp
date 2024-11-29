#include<bits/stdc++.h>
using namespace std;

// Find the burst time of a process
int findProcess(vector<int> &burstTime,string s,vector<string> &Pid){
    for(int i=0;i<Pid.size();i++){
        if(Pid[i]==s)return burstTime[i];
    }
    // flow will never reach here
    return -1;
}


// find arrival time of a process
int findProcessStart(vector<int> & arrivalTime,string s,vector<string> &Pid){
    for(int i=0;i<Pid.size();i++){
        if(Pid[i]==s)return arrivalTime[i];
    }
    return -1;
}

// First come first serve
void FCFS(vector<int> &arrivalTime,vector<int> &burstTime,vector<string> &processID){
    int n=processID.size();
    priority_queue<pair<int,string>,vector<pair<int,string>>,greater<pair<int,string>>> readyQueue;
    // Adding the process in readyQueue cuz its non pre emptive
    for(int i=0;i<n;i++){
        readyQueue.push({arrivalTime[i],processID[i]});
    }
    int curtime=0;
    while(!readyQueue.empty()){
        pair<int,string> curProcess=readyQueue.top();
        readyQueue.pop();
        if(curProcess.first>=curtime){
            int endtime=findProcess(burstTime,curProcess.second,processID);
            curtime=curProcess.first;
            cout<<curProcess.second<<" ran from "<<curtime<<" to "<<endtime+curtime<<endl;
            curtime=endtime+curtime;
        }
        else{
            int endtime=findProcess(burstTime,curProcess.second,processID);
            cout<<curProcess.second<<" ran from "<<curtime<<" to "<<endtime+curtime<<endl;
            curtime=endtime+curtime;
        }
    }
    return;
}


// round robin
void RR(vector<int> &arrivalTime,vector<int> &burstTime,vector<string> &processID){
    int timequanta=5;
    int n=processID.size();
    priority_queue<pair<int,string>,vector<pair<int,string>>,greater<pair<int,string>>> readyQueue;
    // Adding the process in readyQueue cuz its non pre emptive
    int maxArrivalTime=0;
    for(int i=0;i<n;i++){
        maxArrivalTime=max(maxArrivalTime,arrivalTime[i]);
        readyQueue.push({arrivalTime[i],processID[i]});
    }
    maxArrivalTime++;
    int curtime=0;
    vector<int> tempBurstTime=burstTime;

    while(!readyQueue.empty()){
        pair<int,string> curProcess=readyQueue.top();
        readyQueue.pop();
        if(curProcess.first>=curtime){
            int endtime=findProcess(tempBurstTime,curProcess.second,processID);
            curtime=curProcess.first;
            if(endtime<=timequanta){
                cout<<curProcess.second<<" ended at "<<curtime+endtime<<endl;
                curtime=endtime+curtime;
            }
            else{
                int ind=-1;
                for(int i=0;i<n;i++){
                    if(curProcess.second==processID[i]){
                        ind=i;
                    }
                }
                tempBurstTime[ind]-=timequanta;
                readyQueue.push({maxArrivalTime,curProcess.second});
                maxArrivalTime++;
                curtime+=timequanta;
            }
        }
        else{
            int endtime=findProcess(tempBurstTime,curProcess.second,processID);
            if(endtime<=timequanta){
                cout<<curProcess.second<<" ended at "<<endtime+curtime<<endl;
                curtime=endtime+curtime;
            }
            else{
                int ind=-1;
                for(int i=0;i<n;i++){
                    if(curProcess.second==processID[i]){
                        ind=i;
                    }
                }
                tempBurstTime[ind]-=timequanta;
                readyQueue.push({maxArrivalTime,curProcess.second});
                maxArrivalTime++;
                curtime+=timequanta;
            }
        }
    }
}


// Shortest process next
void SPN(vector<int> &arrivalTime,vector<int> &burstTime,vector<string> &processID){
    int n=processID.size();
    priority_queue<pair<int,string>,vector<pair<int,string>>,greater<pair<int,string>>> readyQueue;
    // Adding the process in readyQueue cuz its non pre emptive
    for(int i=0;i<n;i++){
        readyQueue.push({burstTime[i],processID[i]});
    }
    int curtime=0;
    while(!readyQueue.empty()){
        pair<int,string> curProcess=readyQueue.top();
        readyQueue.pop();
        int startime=findProcessStart(arrivalTime,curProcess.second,processID);
        if(startime>=curtime){
            int endtime=findProcess(burstTime,curProcess.second,processID);
            curtime=startime;
            cout<<curProcess.second<<" ran from "<<curtime<<" to "<<endtime+curtime<<endl;
            curtime=endtime+curtime;
        }
        else{
            int endtime=findProcess(burstTime,curProcess.second,processID);
            cout<<curProcess.second<<" ran from "<<curtime<<" to "<<endtime+curtime<<endl;
            curtime=endtime+curtime;
        }
    }
    
    return;
}


// shortest remaining time
void SRT(vector<int> &arrivalTime,vector<int> &burstTime,vector<string> &processID){
    int n=processID.size();
    int curtime=0;
    vector<bool> vis(n,0);// currently in ready queue or exit
    priority_queue<pair<pair<int,int>,string>,vector<pair<pair<int,int>,string>>,greater<pair<pair<int,int>,string>>> readyQueue;
    // I will simulate the scheduling algo for 500 time units since all process will be gone by then
    int position=1;
    while(curtime<=500 && position!=n+1){
        // Simulated entire 500 time units
        for(int i=0;i<n;i++){
            if(!vis[i]){
                if(arrivalTime[i]<=curtime){
                    vis[i]=1;
                    readyQueue.push({{burstTime[i],arrivalTime[i]},processID[i]});
                }
            }
        }
        int duration=1;
        auto it=readyQueue.top();
        readyQueue.pop();
        if(duration>=it.first.first){
            cout<<it.second<<" ended at "<<position<<" position."<<endl;
            position++;
        }
        else{
            it.first.first--;
            readyQueue.push({it});
        }
        curtime++;
    }   
}


// highest response ratio next

void HRRN(vector<int> &arrivalTime,vector<int> &burstTime,vector<string> &processID){
    int n=processID.size();
    int curtime=0;
    int position=1;
    vector<bool> vis(n,0);
    set<int> st;
    while(curtime<=500 && position!=6){
        for(int i=0;i<n;i++){
            if(!vis[i]){
                if(arrivalTime[i]<=curtime){
                    st.insert(i);
                    vis[i]=1;   
                }
            }
        }
        if(st.size()>0){
            int ind=-1;
            int mx=-1e9;
            for(auto &val:st){
                int WT=curtime-arrivalTime[val];
                int BT=burstTime[val];
                // Take the process with max Response Ratio
                double RR=((double)0 + WT + BT)/BT;
                if(mx<RR){
                    mx=RR;
                    ind=val;
                }
            }
            cout<<processID[ind]<<" ended at "<<position<<" position."<<endl;
            position++;
            st.erase(ind);
            curtime+=burstTime[ind];
        }
        else{
            curtime++;
        }
    }
}


int main(){
    int n=5;
    // Assuming only 5 processes for this project
    // Every algo was run separately so maybe answers of differnt algo dont match
    // I tried to implement logic and focused on that part primarily.
    vector<string> processID(n);
    for(int i=0;i<n;i++){
        string s = "P" + to_string(i+1);
        processID[i]=s;
    }
    vector<int> arrivalTime(n),burstTime(n);
    // Assuming max arrival time of 10 sec and max burst time of 50 seconds
    for(int  i=0;i<n;i++){
        int mn=0,mx=10;
        int x=rand();
        x = (x%(mx-mn))+mn;
        arrivalTime[i]=x;
    }
    for(int  i=0;i<n;i++){
        int mn=1,mx=50;
        int x=rand();
        x = (x%(mx-mn))+mn;
        burstTime[i]=x;
    }

    FCFS(arrivalTime,burstTime,processID);
    // result of FCFS during test - 
    // P4 ran from 0 to 13
    // P1 ran from 13 to 58
    // P3 ran from 58 to 66
    // P2 ran from 66 to 79
    // P5 ran from 79 to 93

    RR(arrivalTime,burstTime,processID);
    // result of RR during test -
    // P3 ended at 38
    // P4 ended at 51
    // P2 ended at 59
    // P5 ended at 63
    // P1 ended at 93

    SPN(arrivalTime,burstTime,processID);
    // result of SPN during test - 
    // P3 ran from 4 to 12
    // P2 ran from 12 to 25
    // P4 ran from 25 to 38
    // P5 ran from 38 to 52
    // P1 ran from 52 to 97

    SRT(arrivalTime,burstTime,processID);
    // result of SRT during test - 
    // P3 ended at 1 position.
    // P4 ended at 2 position.
    // P2 ended at 3 position.
    // P5 ended at 4 position.
    // P1 ended at 5 position.

    HRRN(arrivalTime,burstTime,processID);
    // result of HRRN during test - 
    // P4 ended at 1 position.
    // P3 ended at 2 position.
    // P2 ended at 3 position.
    // P5 ended at 4 position.
    // P1 ended at 5 position.



}