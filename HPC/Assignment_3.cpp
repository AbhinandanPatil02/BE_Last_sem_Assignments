#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace chrono;

// ---------------- SEQUENTIAL ----------------
void sequential_ops(vector<int>& arr){
    int n = arr.size();

    int mn = INT_MAX;
    int mx = INT_MIN;
    long long sum = 0;

    for(int i = 0; i < n; i++){
        mn = min(mn, arr[i]);
        mx = max(mx, arr[i]);
        sum += arr[i];
    }

    double avg = (double)sum / n;

    cout << "Sequential:\n";
    cout << "Min: " << mn << "\n";
    cout << "Max: " << mx << "\n";
    cout << "Sum: " << sum << "\n";
    cout << "Avg: " << avg << "\n";
}

// ---------------- PARALLEL ----------------
void parallel_ops(vector<int>& arr){
    int n = arr.size();

    int mn = INT_MAX;
    int mx = INT_MIN;
    long long sum = 0;

    #pragma omp parallel for reduction(min:mn) reduction(max:mx) reduction(+:sum)
    for(int i = 0; i < n; i++){
        mn = min(mn, arr[i]);
        mx = max(mx, arr[i]);
        sum += arr[i];
    }

    double avg = (double)sum / n;

    cout << "Parallel:\n";
    cout << "Min: " << mn << "\n";
    cout << "Max: " << mx << "\n";
    cout << "Sum: " << sum << "\n";
    cout << "Avg: " << avg << "\n";
}

// ---------------- MAIN ----------------
int main(){
    srand(time(0)); 
    int n = 1000000;  // large input for performance
    vector<int> arr(n);
    // fill array
    for(int i = 0; i < n; i++){
        arr[i] = rand() % 1000;
    }
    // Sequential timing
    auto t1 = high_resolution_clock::now();
    sequential_ops(arr);
    auto t2 = high_resolution_clock::now();

    // Parallel timing
    auto t3 = high_resolution_clock::now();
    parallel_ops(arr);
    auto t4 = high_resolution_clock::now();

    cout << "\nTime Comparison:\n";
    cout << "Sequential Time: "
         << duration_cast<microseconds>(t2 - t1).count() << " microseconds\n";

    cout << "Parallel Time:   "
         << duration_cast<microseconds>(t4 - t3).count() << " microseconds\n";

    return 0;
}