#include <bits/stdc++.h>
using namespace std;

// --------------------- MNT ENTRY ---------------------
class MNTEntry
{
public:
    string name;
    int index;
    MNTEntry(string name, int index)
    {
        this->name = name;
        this->index = index;
    }
};

// --------------------- PASS 1 ---------------------
class Pass1
{
public:
    vector<MNTEntry> MNT;
    vector<string> MDT;
    vector<string> intermediate;
    bool inmacro = false;

    void process()
    {
        cout << "Enter the filename : ";
        string fname;
        cin >> fname;
        ifstream fin(fname);
        if (!fin.is_open())
        {
            cout << "Error in opening file" << endl;
            return;
        }

        string line, mname;
        while (getline(fin, line))
        {
            if (line.empty())
                continue;

            string first;
            stringstream ss(line);
            ss >> first;

            if (first == "MACRO")
            {
                inmacro = true;
                getline(fin, line);
                stringstream name(line);
                name >> mname;
                MNT.push_back(MNTEntry(mname, MDT.size()));
                MDT.push_back(line);
                continue;
            }
            if (first == "MEND" && inmacro)
            {
                inmacro = false;
                MDT.push_back("MEND");
                continue;
            }
            if (inmacro)
            {
                MDT.push_back(line);
            }
            else
            {
                intermediate.push_back(line);
            }
        }
        fin.close();
        print();
    }

    void print()
    {
        cout << "\n==================== Intermediate Code ====================\n";
        for (auto &i : intermediate)
            cout << i << endl;

        cout << "\n==================== Macro Name Table (MNT) ====================\n";
        cout << "MacroName\tMDT index\n";
        for (auto &i : MNT)
            cout << i.name << "\t" << i.index << endl;

        cout << "\n==================== Macro Definition Table (MDT) ====================\n";
        for (auto &i : MDT)
            cout << i << endl;
    }
};

// --------------------- PASS 2 ---------------------
class Pass2
{
public:
    vector<MNTEntry> MNT;
    vector<string> MDT;
    vector<string> intermediate;

    Pass2(vector<MNTEntry> mnt, vector<string> mdt, vector<string> inter)
    {
        MNT = mnt;
        MDT = mdt;
        intermediate = inter;
    }

    void process()
    {
        cout << "\n==================== Pass-II: Expanded Code ====================\n";
        for (auto &line : intermediate)
        {
            string first;
            stringstream ss(line);
            ss >> first;

            int macroIndex = searchMNT(first);
            if (macroIndex != -1)
            {
                expandMacro(macroIndex, line);
            }
            else
            {
                cout << line << endl;
            }
        }
    }

    int searchMNT(string name)
    {
        for (int i = 0; i < (int)MNT.size(); i++)
        {
            if (MNT[i].name == name)
                return MNT[i].index;
        }
        return -1;
    }

    void expandMacro(int mdtIndex, string callLine)
    {
        vector<string> args;
        string macroName;
        stringstream ss(callLine);
        ss >> macroName;

        string temp;
        while (getline(ss, temp, ',')) // get args after commas
        {
            string arg = temp;
            arg.erase(remove_if(arg.begin(), arg.end(), ::isspace), arg.end());
            if (!arg.empty())
                args.push_back(arg);
        }

        unordered_map<string, string> argMap;

        // First line of MDT has macro header (e.g. ADDXY &X, &Y)
        string header = MDT[mdtIndex];
        stringstream head(header);
        string hname;
        head >> hname;

        string param;
        int idx = 0;
        while (getline(head, param, ','))
        {
            param.erase(remove_if(param.begin(), param.end(), ::isspace), param.end());
            if (param[0] == '&' && idx < (int)args.size())
            {
                argMap[param] = args[idx++];
            }
        }

        // Expand until MEND
        for (int i = mdtIndex + 1; i < (int)MDT.size() && MDT[i] != "MEND"; i++)
        {
            string expanded = MDT[i];
            for (auto &kv : argMap)
            {
                size_t pos = expanded.find(kv.first);
                if (pos != string::npos)
                    expanded.replace(pos, kv.first.length(), kv.second);
            }
            cout << expanded << endl;
        }
    }
};

// --------------------- MAIN ---------------------
int main()
{
    Pass1 p1;
    p1.process();

    Pass2 p2(p1.MNT, p1.MDT, p1.intermediate);
    p2.process();

    return 0;
}
// macro.asm