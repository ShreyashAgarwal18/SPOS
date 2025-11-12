#include <bits/stdc++.h>
using namespace std;

// ======================= CLASS DEFINITIONS =======================
class process
{
public:
    string name;
    int arrival;
    int burst;
    int remaining;
    int waiting;
    int turnaround;
    int completion;
    int priority;

    process(string name, int arrival, int burst, int priority = -1)
    {
        this->name = name;
        this->arrival = arrival;
        this->burst = burst;
        this->remaining = burst;
        this->priority = priority;
        this->waiting = 0;
        this->turnaround = 0;
        this->completion = 0;
    }
};

// Comparator for sorting by arrival time
class comp
{
public:
    bool operator()(process p1, process p2)
    {
        return p1.arrival < p2.arrival;
    }
};

// ======================= FCFS =======================
void fcfs(vector<process> v)
{
    sort(v.begin(), v.end(), comp());
    int current = 0;
    for (int i = 0; i < v.size(); i++)
    {
        if (current < v[i].arrival)
            current = v[i].arrival;
        v[i].waiting = current - v[i].arrival;
        current += v[i].burst;
        v[i].completion = current;
        v[i].turnaround = v[i].waiting + v[i].burst;
    }

    cout << "\n--- FCFS Scheduling ---\n";
    cout << "Process\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &p : v)
        cout << p.name << "\t" << p.arrival << "\t" << p.burst << "\t"
             << p.completion << "\t" << p.turnaround << "\t" << p.waiting << "\n";
}

// ======================= SJF (Preemptive) =======================
void sjf_preemptive(vector<process> v)
{
    cout << "\n--- SJF (Preemptive) Scheduling ---\n";
    int n = v.size();
    int completed = 0, time = 0;
    sort(v.begin(), v.end(), comp());

    while (completed < n)
    {
        int idx = -1, minBurst = INT_MAX;
        for (int i = 0; i < n; i++)
            if (v[i].arrival <= time && v[i].remaining > 0 && v[i].remaining < minBurst)
                minBurst = v[i].remaining, idx = i;

        if (idx == -1)
        {
            time++;
            continue;
        }

        v[idx].remaining--;
        time++;

        if (v[idx].remaining == 0)
        {
            completed++;
            v[idx].completion = time;
            v[idx].turnaround = v[idx].completion - v[idx].arrival;
            v[idx].waiting = v[idx].turnaround - v[idx].burst;
        }
    }

    cout << "Process\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &p : v)
        cout << p.name << "\t" << p.arrival << "\t" << p.burst << "\t"
             << p.completion << "\t" << p.turnaround << "\t" << p.waiting << "\n";
}

// ======================= PRIORITY (Non-Preemptive) =======================
void priority_non_preemptive(vector<process> v)
{
    cout << "\n--- Priority (Non-Preemptive) Scheduling ---\n";
    int n = v.size();
    int time = 0, completed = 0;
    vector<int> done(n, 0);
    sort(v.begin(), v.end(), comp());

    while (completed < n)
    {
        int idx = -1, best = INT_MAX;
        for (int i = 0; i < n; i++)
            if (!done[i] && v[i].arrival <= time && v[i].priority < best)
                best = v[i].priority, idx = i;

        if (idx == -1)
        {
            time++;
            continue;
        }

        time += v[idx].burst;
        v[idx].completion = time;
        v[idx].turnaround = v[idx].completion - v[idx].arrival;
        v[idx].waiting = v[idx].turnaround - v[idx].burst;
        done[idx] = 1;
        completed++;
    }

    cout << "Process\tAT\tBT\tPR\tCT\tTAT\tWT\n";
    for (auto &p : v)
        cout << p.name << "\t" << p.arrival << "\t" << p.burst << "\t"
             << p.priority << "\t" << p.completion << "\t" << p.turnaround
             << "\t" << p.waiting << "\n";
}

void round_robin(vector<process> v, int tq)
{
    cout << "\n--- Round Robin Scheduling (TQ=" << tq << ") ---\n";
    int n = v.size();
    sort(v.begin(), v.end(), comp()); // Sort by arrival time

    queue<int> q;
    vector<int> visited(n, 0);
    int time = 0, completed = 0;

    // Initialize remaining time in each process
    for (auto &p : v)
        p.remaining = p.burst;

    // Push first process into the queue
    q.push(0);
    visited[0] = 1;
    time = v[0].arrival;

    while (!q.empty())
    {
        int i = q.front();
        q.pop();

        // Execute current process for time quantum or less
        if (v[i].remaining > tq)
        {
            v[i].remaining -= tq;
            time += tq;
        }
        else
        {
            time += v[i].remaining;
            v[i].remaining = 0;
            completed++;
            v[i].completion = time;
            v[i].turnaround = v[i].completion - v[i].arrival;
            v[i].waiting = v[i].turnaround - v[i].burst;
        }

        // Add any processes that have arrived till now
        for (int j = 0; j < n; j++)
        {
            if (!visited[j] && v[j].arrival <= time)
            {
                q.push(j);
                visited[j] = 1;
            }
        }

        // If process not yet finished, requeue it
        if (v[i].remaining > 0)
            q.push(i);

        // If queue is empty but processes remain, jump to next arrival
        if (q.empty() && completed < n)
        {
            for (int j = 0; j < n; j++)
            {
                if (v[j].remaining > 0)
                {
                    q.push(j);
                    visited[j] = 1;
                    time = max(time, v[j].arrival);
                    break;
                }
            }
        }
    }

    cout << "Process\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &p : v)
        cout << p.name << "\t" << p.arrival << "\t" << p.burst << "\t"
             << p.completion << "\t" << p.turnaround << "\t" << p.waiting << "\n";
}

// ======================= MAIN FUNCTION =======================
int main()
{
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<process> v;

    for (int i = 0; i < n; i++)
    {
        string name = "P" + to_string(i + 1);
        int at, bt, pr;
        cout << "Enter Arrival, Burst and Priority for " << name << ": ";
        cin >> at >> bt >> pr;
        v.emplace_back(name, at, bt, pr);
    }

    fcfs(v);
    sjf_preemptive(v);
    priority_non_preemptive(v);
    int tq;
    cout << "\nEnter Time Quantum for Round Robin: ";
    cin >> tq;
    round_robin(v, tq);

    return 0;
}
