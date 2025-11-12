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

class AssemblerPass1
{
private:
    map<string, string> OPTAB;
    vector<Symbol> SYMTAB;
    vector<Literal> LITTAB;
    vector<int> POOLTAB;
    vector<Instruction> INTERMEDIATE;
    int LC;

public:
    AssemblerPass1()
    {
        LC = 0;
        POOLTAB.push_back(0);
        OPTAB = {
            {"MOV", "IS"},
            {"ADD", "IS"},
            {"SUB", "IS"},
            {"MUL", "IS"},
            {"DIV", "IS"},
            {"DC", "DL"},
            {"DS", "DL"},
            {"START", "AD"},
            {"END", "AD"},
            {"LTORG", "AD"}};
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

        // START directive
        if (opcode == "START")
        {
            LC = stoi(operand);
            INTERMEDIATE.push_back(Instruction(label, opcode, operand, LC));
            return;
        }

        // LTORG directive
        if (opcode == "LTORG")
        {
            assignLiterals();
            INTERMEDIATE.push_back(Instruction(label, opcode, operand, LC));
            return;
        }

        // END directive
        if (opcode == "END")
        {
            assignLiterals();
            INTERMEDIATE.push_back(Instruction(label, opcode, operand, LC));
            return;
        }

        // Label handling
        if (!label.empty() && !symbolExists(label))
            SYMTAB.push_back(Symbol(label, LC));

        // Declarative instructions
        if (opcode == "DS")
        {
            LC += stoi(operand);
        }
        else if (opcode == "DC")
        {
            LC += 1;
        }
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
        {
            LITTAB[i].address = LC++;
        }
        POOLTAB.push_back(LITTAB.size());
    }

    void displayTables()
    {
        cout << "\nINTERMEDIATE FILE:\n";
        cout << "LC\tLABEL\tOPCODE\tOPERAND\n";
        for (auto &i : INTERMEDIATE)
            cout << i.lc << "\t" << i.label << "\t" << i.opcode << "\t" << i.operand << "\n";

        cout << "\nSYMBOL TABLE:\n";
        cout << "NAME\tADDRESS\n";
        for (auto &s : SYMTAB)
            cout << s.name << "\t" << s.address << "\n";

        cout << "\nLITERAL TABLE:\n";
        cout << "LITERAL\tADDRESS\n";
        for (auto &l : LITTAB)
            cout << l.value << "\t" << l.address << "\n";

        cout << "\nPOOLTAB:\n";
        for (int i = 0; i < (int)POOLTAB.size(); i++)
            cout << "Pool " << i << " starts at literal index " << POOLTAB[i] << "\n";
    }
};

// ===================== MAIN FUNCTION =====================
int main()
{
    string filename;
    cout << "Enter assembly file name (with extension): ";
    cin >> filename;

    ifstream fin(filename);
    if (!fin.is_open())
    {
        cerr << "Error: Could not open file '" << filename << "'\n";
        return 1;
    }

    AssemblerPass1 assembler;
    string line;
    while (getline(fin, line))
    {
        assembler.processLine(line);
        if (line.find("END") != string::npos)
            break;
    }
    fin.close();

    assembler.displayTables();
    cout << "\nPass-I completed successfully.\n";
    return 0;
}
































/*
Topic: Two-Pass Assembler — Pass I
----------------------------------
Theory:
A two-pass assembler translates assembly language into machine code in two stages.
In Pass-I, the assembler scans the source program to:
1. Build the Symbol Table (labels with their addresses).
2. Build the Literal Table (constants used in the program).
3. Maintain a Pool Table (to manage literal groups).
4. Generate the Intermediate Code (IC) that records instructions with location counters.
Pass-I helps handle forward references — symbols used before being defined.

Code Explanation:
→ The program reads an assembly source file line by line.
→ It maintains data structures: SYMTAB (symbols), LITTAB (literals), POOLTAB (literal pools), and OPTAB (opcodes).
→ LC (Location Counter) keeps track of instruction addresses.
→ Directives like START, LTORG, and END are recognized and processed.
→ For labels, their names and addresses are stored in the symbol table.
→ For literals (like =5), entries are added to the literal table and assigned addresses at LTORG or END.
→ Finally, the program displays all tables: Intermediate Code, Symbol Table, Literal Table, and Pool Table.

In short, Pass-I prepares all necessary information for Pass-II to generate the final machine code.

more info
In computer systems, programs written in assembly language (which uses mnemonics like MOV, ADD, SUB, etc.) 
cannot be directly executed by the machine.
They must first be translated into machine code (binary form) that the CPU understands.
This translation is done by a program called an Assembler.
Assemblers are usually divided into two passes:
Pass-I:
Scans the assembly program.
Builds essential tables (Symbol Table, Literal Table, Pool Table).
Generates an Intermediate Code (IC) with symbolic references.
Calculates addresses using a Location Counter (LC).
Handles assembler directives like START, LTORG, and END.

Pass-II:
Uses the tables created in Pass-I.
Replaces symbols and literals with their actual addresses.
Produces the final machine code or object code.
The given code implements Pass-I, which is the foundation of the
 assembling process — it analyzes the structure of assembly code and prepares
 all the necessary data for Pass-II.








*/















































































