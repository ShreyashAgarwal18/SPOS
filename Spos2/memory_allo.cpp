#include <bits/stdc++.h>
using namespace std;
vector<int> bestfit(vector<int> process, vector<int> blocks)
{
    vector<int> allocation(process.size(), -1);
    for (int i = 0; i < process.size(); i++)
    {
        int min_size = INT_MAX, ind = -1;
        for (int j = 0; j < blocks.size(); j++)
        {
            if (blocks[j] >= process[i] && blocks[j] < min_size)
            {
                min_size = blocks[j];
                ind = j;
            }
        }
        if (ind != -1)
        {
            blocks[ind] -= process[i];
            allocation[i] = ind;
        }
    }
    return allocation;
}
vector<int> firstfit(vector<int> process, vector<int> blocks)
{
    vector<int> allocation(process.size(), -1);
    for (int i = 0; i < process.size(); i++)
    {
        for (int j = 0; j < blocks.size(); j++)
        {
            if (blocks[j] >= process[i])
            {
                allocation[i] = j;
                blocks[j] -= process[i];
                break;
            }
        }
    }
    return allocation;
}
void print(vector<int> a)
{
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] == -1)
        {
            cout << "Process " << i + 1 << " not allocated" << endl;
            continue;
        }
        cout << "Process " << i + 1 << " allocated to block " << a[i] + 1 << endl;
    }
}
vector<int> nextfit(vector<int> process, vector<int> blocks)
{
    int last = 0;
    vector<int> allocation(process.size(), -1);
    for (int i = 0; i < process.size(); i++)
    {
        int cnt = 0, j = last;
        while (cnt < blocks.size())
        {
            if (blocks[j] >= process[i])
            {
                allocation[i] = j;
                blocks[j] -= process[i];
                last = j;
                last = (last + 1) % blocks.size();
                break;
            }
            j = (j + 1) % blocks.size();
            cnt++;
        }
    }
    return allocation;
}
vector<int> worstfit(vector<int> process, vector<int> blocks)
{
    vector<int> allocation(process.size(), -1);
    for (int i = 0; i < process.size(); i++)
    {
        int max_size = INT_MIN, ind = -1;
        for (int j = 0; j < blocks.size(); j++)
        {
            if (blocks[j] >= process[i] && blocks[j] > max_size)
            {
                max_size = blocks[j];
                ind = j;
            }
        }
        if (ind != -1)
        {
            blocks[ind] -= process[i];
            allocation[i] = ind;
        }
    }
    return allocation;
}
int main()
{
    vector<int> blocks;
    vector<int> process;
    int x, y, ip;
    cout << "Enter the number of memory blocks : ";
    cin >> x;
    cout << "Enter the sizes of memory blocks : ";
    for (int i = 0; i < x; i++)
    {
        cin >> ip;
        blocks.push_back(ip);
    }
    cout << "Enter the number of processes : ";
    cin >> y;
    cout << "Enter the sizes of processes : ";
    for (int j = 0; j < y; j++)
    {
        cin >> ip;
        process.push_back(ip);
    }
    cout << "Best Fit Algorithm" << endl;
    print(bestfit(process, blocks));
    cout << "First Fit algorithm" << endl;
    print(firstfit(process, blocks));
    cout << "Next Fit Algorithm" << endl;
    print(nextfit(process, blocks));
    cout << "Worst Fit Algorithm" << endl;
    print(worstfit(process, blocks));
    return 0;
}
// 5 100 500 200 300 600 4 212 417 112 426