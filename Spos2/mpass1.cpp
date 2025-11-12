#include <bits/stdc++.h>
using namespace std;

class MNTEntry
{
public:
    string name;
    int mdtIndex;
    MNTEntry(string n, int idx) : name(n), mdtIndex(idx) {}
};

class MacroProcessorPass1
{
    vector<MNTEntry> MNT;
    vector<string> MDT;
    vector<string> intermediate;
    bool inMacro = false;

public:
    void processFile(const string &filename)
    {
        ifstream fin(filename);  
        if (!fin)
        {
            cout << "Error opening file: " << filename << endl;
            return;
        }

        string line;
        string currentMacro;

        while (getline(fin, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string first;
            ss >> first;

            if (first == "MACRO")
            {
                inMacro = true;
                getline(fin, line); // read macro header (next line)
                stringstream macroHeader(line);
                macroHeader >> currentMacro;
                MNT.push_back(MNTEntry(currentMacro, MDT.size()));
                MDT.push_back(line); // add header line
                continue;
            }

            if (first == "MEND" && inMacro)
            {
                MDT.push_back("MEND");
                inMacro = false;
                continue;
            }

            if (inMacro)
            {
                MDT.push_back(line);
            }
            else
            {
                intermediate.push_back(line);
            }
        }

        fin.close();
        printOutput();
    }

    void printOutput()
    {
        cout << "\n==============================\n";
        cout << "MNT (Macro Name Table)\n";
        cout << "==============================\n";
        for (int i = 0; i < (int)MNT.size(); i++)
        {
            cout << i << "\t" << MNT[i].name << "\t" << MNT[i].mdtIndex << "\n";
        }

        cout << "\n==============================\n";
        cout << "MDT (Macro Definition Table)\n";
        cout << "==============================\n";
        for (int i = 0; i < (int)MDT.size(); i++)
        {
            cout << i << "\t" << MDT[i] << "\n";
        }

        cout << "\n==============================\n";
        cout << "Intermediate Code (Without Macros)\n";
        cout << "==============================\n";
        for (auto &line : intermediate)
        {
            cout << line << "\n";
        }
        cout << "\nPass-I completed successfully.\n";
    }
};

int main()
{
    string filename;
    cout << "Enter the input assembly filename: ";
    cin >> filename;

    MacroProcessorPass1 pass1;
    pass1.processFile(filename);

    return 0;
}
