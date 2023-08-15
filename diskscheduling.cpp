#include <bits/stdc++.h>
using namespace std;

#define ll long long int

#define maxd 4999
#define mind 0

void fcfs(vector<int> &v,int ip)
{
    ll total = 0;
    for(int i=0;i<v.size();i++)
    {
        total += abs(ip-v[i]);
        ip=v[i];
    }
    /*
    cout<<"fcfs sequence: "<<endl;
    for(int i=0;i<v.size();i++)
    {
        cout<<v[i]<<" ";
    }
    */
    //cout<<endl;
    cout<<"total head movement: "<<total<<endl;
    cout<<"average head movement: "<<(double)total/v.size()<<endl;
}

void sstf(vector<int> &v,int ip)
{
    ll total = 0;
    int n = v.size();
    //cout<<ip<<endl;
    while(v.size())
    {
        int min = INT_MAX;
        int index = -1;
        for(int i=0;i<v.size();i++)
        {
            if(abs(ip-v[i])<min)
            {
                min = abs(ip-v[i]);
                index = i;
            }
        }
        total += min;
        //cout<<v[index]<<" "<<endl;
        ip = v[index];
        v.erase(v.begin()+index);
    }
    cout<<"total head movement: "<<total<<endl;
    cout<<"average head movement: "<<(double)total/n<<endl;
}

void scan(vector<int> &v,int ip)
{
    ll total = 0;
    int n = v.size();
    sort(v.begin(),v.end());
    int index = -1;
    for(int i=0;i<v.size();i++)
    {
        if(v[i]>=ip)
        {
            index = i;
            break;
        }
    }
    if(index==-1)
    {
        index = v.size();
    }
    //cout<<ip<<endl;
    for(int i=index-1;i>=0;i--)
    {
        total += abs(ip-v[i]);
        //cout<<v[i]<<endl;
        ip = v[i];
    }
    total += abs(ip-mind);
    //cout<<mind<<endl;
    ip = mind;
    for(int i=index;i<v.size();i++)
    {
        total += abs(ip-v[i]);
        //cout<<v[i]<<endl;
        ip = v[i];
    }
    //cout<<endl;
    cout<<"total head movement: "<<total<<endl;
    cout<<"average head movement: "<<(double)total/n<<endl;
}

void cscan(vector<int> &v,int ip)
{
    ll total = 0;
    int n = v.size();
    sort(v.begin(),v.end());
    int index = -1;
    for(int i=0;i<v.size();i++)
    {
        if(v[i]>=ip)
        {
            index = i;
            break;
        }
    }
    if(index==-1)
    {
        index = v.size();
    }
    //cout<<ip<<endl;
    for(int i=index;i<v.size();i++)
    {
        total += abs(ip-v[i]);
        //cout<<v[i]<<endl;
        ip = v[i];
    }
    total += abs(ip-maxd);
    //cout<<maxd<<endl;
    ip = maxd;
    //cout<<mind<<endl;
    ip = mind;
    total+=(maxd-mind);
    for(int i=0;i<index;i++)
    {
        total += abs(ip-v[i]);
        //cout<<v[i]<<endl;
        ip = v[i];
    }
    //cout<<endl;
    cout<<"total head movement: "<<total<<endl;
    cout<<"average head movement: "<<(double)total/n<<endl;
}

void look(vector<int> &v,int ip)
{
    ll total = 0;
    int n = v.size();
    sort(v.begin(),v.end());
    int index = -1;
    for(int i=0;i<v.size();i++)
    {
        if(v[i]>=ip)
        {
            index = i;
            break;
        }
    }
    if(index==-1)
    {
        index = v.size();
    }
    //cout<<ip<<endl;
    for(int i=index-1;i>=0;i--)
    {
        total += abs(ip-v[i]);
        //cout<<v[i]<<endl;
        ip = v[i];
    }    
    for(int i=index;i<v.size();i++)
    {
        total += abs(ip-v[i]);
        //cout<<v[i]<<endl;
        ip = v[i];
    }
    //cout<<endl;
    cout<<"total head movement: "<<total<<endl;
    cout<<"average head movement: "<<(double)total/n<<endl;
}

void clook(vector<int> &v,int ip)
{
    ll total = 0;
    int n = v.size();
    sort(v.begin(),v.end());
    int index = -1;
    for(int i=0;i<v.size();i++)
    {
        if(v[i]>=ip)
        {
            index = i;
            break;
        }
    }
    if(index==-1)
    {
        index = v.size();
    }
    //cout<<ip<<endl;
    for(int i=index;i<v.size();i++)
    {
        total += abs(ip-v[i]);
        //cout<<v[i]<<endl;
        ip = v[i];
    }
    for(int i=0;i<index;i++)
    {
        total += abs(ip-v[i]);
        //cout<<v[i]<<endl;
        ip = v[i];
    }
    //cout<<endl;
    cout<<"total head movement: "<<total<<endl;
    cout<<"average head movement: "<<(double)total/n<<endl;
}

int main(int argc, char*argv[])
{
    int ip = 2000,op;
    if(argc > 2)
    {
        ip = atoi(argv[1]);
        op = atoi(argv[2]);
    }
    else
    {
        op = atoi(argv[1]);
        ip = 2000;
    }
    vector<int> v;
    srand(time(0));
    v.clear();
    for(int i=0;i<1000;i++)
    {
        int x = rand()%5000;
        v.push_back(x);
    }

    /*
    int a[] = {98,183,37,122,14,124,65,67};
    for(int i=0;i<8;i++)
    {
         //int x;
         //cin>>x;
         v.push_back(a[i]);
    }
    */
    if(op == 1)
    {
        fcfs(v,ip);
    }
    else if (op == 2)
    {
        sstf(v,ip);
    }
    else if(op == 3)
    {
        scan(v,ip);
    }
    else if(op == 4)
    {
        cscan(v,ip);
    }
    else if(op == 5)
    {
        look(v,ip);
    }
    else if(op == 6)
    {
        clook(v,ip);
    }
}