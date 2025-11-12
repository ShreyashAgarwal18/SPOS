#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

// =================== FIFO Function ===================
void fifoPageReplacement(vector<int> pages, int framesCount)
{
    vector<int> frames;
    queue<int> q;
    int pageFaults = 0;

    cout << "\n--- FIFO Page Replacement ---\n";
    for (int page : pages)
    {
        bool found = false;

        for (int f : frames)
        {
            if (f == page)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            if (frames.size() < framesCount)
            {
                frames.push_back(page);
                q.push(page);
            }
            else
            {
                int victim = q.front();
                q.pop();
                q.push(page);
                for (int j = 0; j < framesCount; j++)
                {
                    if (frames[j] == victim)
                    {
                        frames[j] = page;
                        break;
                    }
                }
            }
            pageFaults++;
        }

        cout << "Page " << page << " -> [ ";
        for (int f : frames)
            cout << f << " ";
        cout << "] " << (found ? "(Hit)" : "(Fault)") << endl;
    }
    cout << "\nTotal Page Faults (FIFO): " << pageFaults << endl;
}

// =================== LRU Function ===================
void lruPageReplacement(vector<int> pages, int framesCount)
{
    vector<int> frames;
    unordered_map<int, int> lastUsed;
    int pageFaults = 0;

    cout << "\n--- LRU Page Replacement ---\n";
    for (int i = 0; i < pages.size(); i++)
    {
        int page = pages[i];
        bool found = false;

        for (int f : frames)
        {
            if (f == page)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            if (frames.size() < framesCount)
            {
                frames.push_back(page);
            }
            else
            {
                int lruIndex = 0, minUse = i;
                for (int j = 0; j < frames.size(); j++)
                {
                    if (lastUsed[frames[j]] < minUse)
                    {
                        minUse = lastUsed[frames[j]];
                        lruIndex = j;
                    }
                }
                frames[lruIndex] = page;
            }
            pageFaults++;
        }

        lastUsed[page] = i;

        cout << "Page " << page << " -> [ ";
        for (int f : frames)
            cout << f << " ";
        cout << "] " << (found ? "(Hit)" : "(Fault)") << endl;
    }
    cout << "\nTotal Page Faults (LRU): " << pageFaults << endl;
}

// =================== Main Function ===================
int main()
{
    int n, framesCount, choice;
    cout << "Enter number of pages: ";
    cin >> n;

    vector<int> pages(n);
    cout << "Enter page reference string: ";
    for (int i = 0; i < n; i++)
        cin >> pages[i];

    cout << "Enter number of frames: ";
    cin >> framesCount;

    cout << "\nChoose Page Replacement Algorithm:\n";
    cout << "1. FIFO\n2. LRU\nEnter choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        fifoPageReplacement(pages, framesCount);
        break;
    case 2:
        lruPageReplacement(pages, framesCount);
        break;
    default:
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
