#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

constexpr char TERMINAL_ACTIVE[] = "\033[4m";
constexpr char TERMINAL_RESET[] = "\033[0m";

struct Rule {
    string CheckState;
    string NextState;
    string ReadTape;
    string WriteTape;
    int Move;
};

vector<Rule> read_rules(istream& stream)
{
    vector<Rule> rules;

    while (stream)
    {
        Rule rule;
        char move;

        stream
            >> rule.CheckState
            >> rule.ReadTape
            >> rule.WriteTape
            >> move
            >> rule.NextState;

        if (!stream.eof() && stream.fail())
        {
            throw runtime_error("failed reading rules-file.");
        }

        switch (move)
        {
            case '<': rule.Move = -1; break;
            case '>': rule.Move = 1; break;
            case '-': rule.Move = 0; break;

            default: throw runtime_error("failed reading rules-file (direction).");
        }

        rules.push_back(rule);
    }

    return rules;
}

vector<string> read_tape(istream& stream)
{
    vector<string> tape;
    string read;

    while (stream >> read)
    {
        tape.push_back(read);
    }

    if (!stream.eof() && stream.fail())
    {
        throw runtime_error("failed reading tape-file.");
    }

    return tape;
}

void output_tape(ostream& output, vector<string>& tape, int headPos)
{
    int size = tape.size();

    for (int i = 0; i < size; ++i)
    {
        if (i == headPos) output << TERMINAL_ACTIVE;
        output << tape[i];
        if (i == headPos) output << TERMINAL_RESET;
        output << ' ';
    }

    if (headPos == size) output << TERMINAL_ACTIVE;
    output << '?';
    if (headPos == size) output << TERMINAL_RESET;
}

void output_rule(ostream& output, Rule& rule)
{
    char move;

    switch (rule.Move)
    {
        case -1: move = '<'; break;
        case 0: move = '-'; break;
        case 1: move = '>'; break;
    }

    output
        << rule.CheckState << ' '
        << rule.ReadTape << ' '
        << rule.WriteTape << ' '
        << move << ' '
        << rule.NextState;
}

void run_turing(vector<Rule>& rules, vector<string>& tape)
{
    int headPos = 0;
    string state = rules[0].CheckState;
    bool found;

    cout << state << ":\t\t";
    output_tape(cout, tape, headPos);
    cout << "\n\n";

    do
    {
        found = false;
        bool validPos = headPos >= 0 && headPos < tape.size();

        string read = validPos ? tape[headPos] : "?";
        
        for (auto& rule : rules)
        {
            if (!found && state == rule.CheckState && read == rule.ReadTape)
            {
                if (validPos)
                {
                    tape[headPos] = rule.WriteTape;
                }
                else if (headPos == tape.size())
                {
                    tape.push_back(rule.WriteTape);
                }
                
                headPos += rule.Move;
                state = rule.NextState;
                found = true;

                output_rule(cout, rule);
                cout << ":\t";
                output_tape(cout, tape, headPos);
                cout << '\n';
            }
        }

    } while (found);

    cout << '\n' << state << ":\t\t";
    output_tape(cout, tape, headPos);
    cout << '\n';
}

int main(int argc, char** argv)
{
    string progName = argv[0];

    if (argc < 2)
    {
        cerr
            << "ERROR: too few arguments.\n"
            << "USE: " << progName << " <rules.txt> <tape.txt>\n";
        return 1;
    }

    string rulesPath = argv[1];
    string tapePath = argv[2];

    ifstream rulesFile(rulesPath);
    if (!rulesFile)
    {
        cerr << "ERROR: could not open rules-file '" << rulesPath << "'\n";
        return 1;
    }

    ifstream tapeFile(tapePath);
    if (!tapeFile)
    {
        cerr << "ERROR: could not open tape-file '" << tapePath << "'\n";
        return 1;
    }

    try
    {
        auto rules = read_rules(rulesFile);
        auto tape = read_tape(tapeFile);

        run_turing(rules, tape);
    }
    catch (runtime_error& error)
    {
        cerr << "ERROR: " << error.what() << '\n';
        return 1;
    }

    return 0;
}