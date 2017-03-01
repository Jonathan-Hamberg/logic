#include "Equation.h"

#include <iostream>

void truth_table(std::vector<Equation> equations);


bool cmd_debug = false;
bool cmd_compare = false;
bool cmd_help = false;

/**
* -d debug Prints the canonical and rpn notation.
*/
int main(int argc, char *argv[])
{
    std::vector<std::vector<char>> eqns;
    std::vector<std::string> original_eqns;
    std::map<char, bool> variables;
    std::vector<Equation> equations;

    // Canonicalize and extract variables.
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-d", strlen("-d")) == 0) {
            cmd_debug = true;
        }
        else if (strncmp(argv[i], "-c", strlen("-c")) == 0) {
            cmd_compare = true;
        }
        else {
            equations.push_back(Equation(argv[i]));
        }
    }

    // Print the debug information if necessary.
    if (cmd_debug) {
        for (unsigned i = 0; i < equations.size(); i++) {
            std::cout << i + 1 << ". " << equations[i].get_original() << std::endl;
            std::cout << "   " << equations[i].get_canonical() << std::endl;
            std::cout << "   " << equations[i].get_rpn() << std::endl;
        }
    }

    truth_table(equations);
}

void truth_table(std::vector<Equation> equations){

    // Make a map of the variables used in all the equations.
    std::map<char, bool> vars;
    for (auto eqn : equations) {
        vars.insert(eqn.get_variables().begin(), eqn.get_variables().end());
    }

    std::string header = "";
    // Add the variable names to the header.
    for (auto var : vars) {
        header.append(1, var.first);
        header.append(" ");
    }

    // Add the seperator between the variables and the equations.
    header += "| ";

    // This vector stores the number of columns until the equation in the truth table.
    std::vector<int> equation_position;
    equation_position.push_back(0);

    // Add the equations to the header.
    for (auto &eqn : equations) {
        header += eqn.get_original() + " ";
        equation_position.push_back(eqn.get_original().length());
    }

    // Remove the extra space at the end of the header.
    header = header.substr(0, header.length() - 1);

    // Print the header.
    std::cout << header << std::endl;

    // Print seperating line between header and body of the table.
    std::cout << std::string(header.length(), '-') << std::endl;

    // Used to determine if the results are equal.
    bool is_equal = true;

    // Print the rows of the truth table.
    int numRows = (int)pow(2, vars.size());
    for (int i = 0; i < numRows; i++) {

        // Print the variable inputs.
        int idx = vars.size() - 1;
        for (auto &var : vars) {
            bool value = ((i & (1 << idx)) > 0);
            var.second = value;
            if (value) {
                std::cout << "1 ";
            }
            else {
                std::cout << "0 ";
            }

            // Decrement the index.
            idx--;
        }

        // Print the seperator between the variables and the equations.
        std::cout << "| ";

        // Print the results of the equations.
        std::vector<char> results;
        for (auto eqn : equations) {
            results.push_back(eqn.evaluate(vars) ? '1' : '0');
        }

        // Print the results of the equations to the truth table.
        auto iter = equation_position.begin();
        for (auto res : results) {
            // Print the space seperators between the equations.
            std::cout << std::string(*(iter++), ' ');

            // Print the relsult of the equations.
            std::cout << res;
        }

        // Check to see if all the equations equal each other.
        for (unsigned i = 0; i < results.size() - 1; i++) {
            if (results[i] != results[i + 1]) {
                is_equal = false;
            }
        }

        std::cout << std::endl;
    }

    // Print whether or not the equations are equal.
    if (cmd_compare) {
        if (is_equal) {
            std::cout << "The equation(s) are equal." << std::endl;
        }
        else {
            std::cout << "The equation(s) are not equal." << std::endl;
        }
    }
    
}   