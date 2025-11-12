#include <bits/stdc++.h>
using namespace std;

// ===================== CLASS DEFINITIONS =====================
class Symbol
{
public:
    string name;
    int address;
    Symbol(string n, int a) : name(n), address(a) {}
};

class Literal
{
public:
    string value;
    int address;
    Literal(string v, int a = -1) : value(v), address(a) {}
};

class Instruction
{
public:
    string label, opcode, operand;
    int lc;
    Instruction(string l, string o, string op, int loc)
        : label(l), opcode(o), operand(op), lc(loc) {}
};

// ===================== PASS 2 ASSEMBLER =====================
class AssemblerPass2
{
private:
    map<string, pair<string, int>> MOT; // Mnemonic opcode table
    vector<Symbol> SYMTAB;
    vector<Literal> LITTAB;
    vector<Instruction> INTERMEDIATE;

public:
    AssemblerPass2(vector<Symbol> s, vector<Literal> l, vector<Instruction> i)
    {
        SYMTAB = s;
        LITTAB = l;
        INTERMEDIATE = i;

        // Mnemonic opcode table (Opcode + number of operands)
        MOT = {
            {"MOV", {"01", 1}},
            {"ADD", {"02", 1}},
            {"SUB", {"03", 1}},
            {"MUL", {"04", 1}},
            {"DIV", {"05", 1}},
            {"DC", {"--", 1}},
            {"DS", {"--", 1}},
            {"START", {"--", 0}},
            {"END", {"--", 0}},
            {"LTORG", {"--", 0}}};
    }

    int getSymbolAddress(string sym)
    {
        for (auto &s : SYMTAB)
            if (s.name == sym)
                return s.address;
        return -1;
    }

    int getLiteralAddress(string lit)
    {
        for (auto &l : LITTAB)
            if (l.value == lit)
                return l.address;
        return -1;
    }

    void generateMachineCode()
    {
        cout << "\nMACHINE CODE (Pass-II Output):\n";
        cout << "LC\tINSTRUCTION\n";

        for (auto &inst : INTERMEDIATE)
        {
            string opcode = inst.opcode;
            string operand = inst.operand;
            string machineLine = "";

            // START or END directive
            if (opcode == "START" || opcode == "END" || opcode == "LTORG")
            {
                cout << inst.lc << "\t(No Machine Code)\n";
                continue;
            }

            // DS / DC handling
            if (opcode == "DS")
            {
                cout << inst.lc << "\t(Reserve " << operand << " words)\n";
                continue;
            }
            if (opcode == "DC")
            {
                cout << inst.lc << "\t(" << operand << ")\n";
                continue;
            }

            // For Imperative statements
            if (MOT.find(opcode) != MOT.end())
            {
                machineLine += MOT[opcode].first + " ";

                // Find operand address
                int addr = -1;
                if (!operand.empty())
                {
                    if (operand[0] == '=')
                        addr = getLiteralAddress(operand);
                    else
                        addr = getSymbolAddress(operand);
                }

                if (addr == -1 && !operand.empty())
                    machineLine += "??"; // unknown
                else if (!operand.empty())
                    machineLine += to_string(addr);
                else
                    machineLine += "00";

                cout << inst.lc << "\t" << machineLine << "\n";
            }
            else
            {
                cout << inst.lc << "\t(Unknown instruction)\n";
            }
        }
    }
};

// ===================== MAIN FUNCTION =====================
int main()
{
    string filename;
    cout << "Enter assembly file name (same used in Pass-I): ";
    cin >> filename;

    // Run Pass-I first
    ifstream fin(filename);
    if (!fin.is_open())
    {
        cerr << "Error: Could not open file.\n";
        return 1;
    }

    // ---------- PASS I (Simplified) ----------
    class AssemblerPass1
    {
    public:
        map<string, string> OPTAB;
        vector<Symbol> SYMTAB;
        vector<Literal> LITTAB;
        vector<int> POOLTAB;
        vector<Instruction> INTERMEDIATE;
        int LC;

        AssemblerPass1()
        {
            LC = 0;
            POOLTAB.push_back(0);
            OPTAB = {
                {"MOV", "IS"}, {"ADD", "IS"}, {"SUB", "IS"}, {"MUL", "IS"}, {"DIV", "IS"}, {"DC", "DL"}, {"DS", "DL"}, {"START", "AD"}, {"END", "AD"}, {"LTORG", "AD"}};
        }

        void processLine(string line)
        {
            if (line.empty())
                return;
            string label = "", opcode = "", operand = "";
            stringstream ss(line);
            ss >> label;

            if (OPTAB.find(label) != OPTAB.end())
            {
                opcode = label;
                label = "";
            }
            else
            {
                ss >> opcode;
            }
            ss >> operand;

            if (opcode == "START")
            {
                LC = stoi(operand);
                INTERMEDIATE.push_back(Instruction(label, opcode, operand, LC));
                return;
            }
            if (opcode == "LTORG" || opcode == "END")
            {
                assignLiterals();
                INTERMEDIATE.push_back(Instruction(label, opcode, operand, LC));
                return;
            }

            if (!label.empty() && !symbolExists(label))
                SYMTAB.push_back(Symbol(label, LC));

            if (opcode == "DS")
                LC += stoi(operand);
            else if (opcode == "DC")
                LC += 1;
            else
            {
                if (!operand.empty() && operand[0] == '=')
                    LITTAB.push_back(Literal(operand));
                LC += 1;
            }

            INTERMEDIATE.push_back(Instruction(label, opcode, operand, LC));
        }

        bool symbolExists(const string &name)
        {
            for (auto &s : SYMTAB)
                if (s.name == name)
                    return true;
            return false;
        }

        void assignLiterals()
        {
            for (int i = POOLTAB.back(); i < (int)LITTAB.size(); i++)
                LITTAB[i].address = LC++;
            POOLTAB.push_back(LITTAB.size());
        }
    };

    // ---------- Run Pass-I ----------
    AssemblerPass1 pass1;
    string line;
    while (getline(fin, line))
    {
        pass1.processLine(line);
        if (line.find("END") != string::npos)
            break;
    }
    fin.close();

    // ---------- Run Pass-II ----------
    AssemblerPass2 pass2(pass1.SYMTAB, pass1.LITTAB, pass1.INTERMEDIATE);
    pass2.generateMachineCode();

    cout << "\nPass-II completed successfully.\n";
    return 0;
}
