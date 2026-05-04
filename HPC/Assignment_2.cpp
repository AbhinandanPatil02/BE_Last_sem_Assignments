#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace chrono;



void merge(vector<int>&arr,int l,int mid,int h){
    int left=l;
    int right=mid+1;
    vector<int>temp;
    while(left<=mid&&right<=h){
        if(arr[left]<=arr[right]){
            temp.push_back(arr[left++]);
        }
        else{
            temp.push_back(arr[right++]);
        }
    }
    while(left<=mid){
        temp.push_back(arr[left++]);
    }
    while(right<=h){
        temp.push_back(arr[right++]);
    }

    for(int i=l;i<=h;i++){
        arr[i]=temp[i-l];
    }
}



void merge_sort(vector<int>&arr,int l,int h){
    if(l<h){
        int mid=(l+h)/2;
        merge_sort(arr,l,mid);
        merge_sort(arr,mid+1,h);
        merge(arr,l,mid,h);
    }
}


void parallel_merge_sort(vector<int>&arr,int l,int h){
    if(l<h){
        if(h - l < 1000){
            merge_sort(arr, l, h);
            return;
        }
        int mid=(l+h)/2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallel_merge_sort(arr,l,mid);
            #pragma omp section
            parallel_merge_sort(arr,mid+1,h);
        }
        merge(arr,l,mid,h);
    }

}


void bubble_sort(vector<int>&arr){
    int n=arr.size();
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void parallel_bubble_sort(vector<int>&arr){
    int n=arr.size();
    for(int i=0;i<n;i++){
        #pragma omp parallel for
        for(int j=0;j<n-1;j+=2){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
        #pragma omp parallel for
        for(int j=1;j<n-1;j+=2){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}



void compare_merge(vector<int>arr){
    int runs=5;
    int l=0,h=arr.size()-1;
    double m_seq=0.0;
    for(int i=0;i<runs;i++){
        vector<int>temp=arr;
        auto  t1 =high_resolution_clock::now();
        merge_sort(temp,l,h);
        auto  t2 =high_resolution_clock::now();
        m_seq+= duration_cast<microseconds>(t2-t1).count();
    }

    double m_par=0.0;
    for(int i=0;i<runs;i++){
        vector<int>temp=arr;
        auto  t1 =high_resolution_clock::now();
        parallel_merge_sort(temp,l,h);
        auto  t2 =high_resolution_clock::now();
        m_par+= duration_cast<microseconds>(t2-t1).count();
    }
    m_seq/=runs;
    m_par/=runs;
    cout<<"Merge sequential : "<<m_seq/1e6<<" sec"<<endl;
    cout<<"Merge Parallel : "<<m_par/1e6<<" sec"<<endl;
}


void compare_bubble(vector<int>&arr){
    int runs=5;
    double b_seq=0.0;
    for(int i=0;i<runs;i++){
        vector<int>temp=arr;
        auto  t1 =high_resolution_clock::now();
        bubble_sort(temp);
        auto  t2 =high_resolution_clock::now();
        b_seq+= duration_cast<microseconds>(t2-t1).count();
    }

    double b_par=0.0;
    for(int i=0;i<runs;i++){
        vector<int>temp=arr;
        auto  t1 =high_resolution_clock::now();
        parallel_bubble_sort(temp);
        auto  t2 =high_resolution_clock::now();
        b_par+= duration_cast<microseconds>(t2-t1).count();
    }
    b_seq/=runs;
    b_par/=runs;
    cout<<"bubble sequential : "<<b_seq/1e6<<" sec"<<endl;
    cout<<"bubble Parallel : "<<b_par/1e6<<" sec"<<endl;

}




int main(){

    // vector<int>arr={8,1,5,4,9,63,55};

    int n = 1200;   // or 1e5 or 1e6
    vector<int> arr(n);
    for(int i = 0; i < n; i++){
        arr[i] = rand() % 1200;
    }
    int l=0,h=arr.size()-1;
    // parallel_bubble_sort(arr);
    // for(auto i:arr){
    //     cout<<i <<" ";
    // }


    compare_bubble(arr);
    cout<<endl;


    return 0;
}