#include "Equation.h"

#include <iostream>
#include <regex>
#include <stack>

std::string Equation::canonicalize(std::string text)
{
    // Capatalize all the variables in the expression.
    for (auto &c : text) c = toupper(c);

    // Remove unsupported tokens.
    text = std::regex_replace(text, std::regex("[^A-Z*+!()01]"), "");

    // Add the implicit 'and' operators.
    for (unsigned i = 0; i < text.length() - 1; i++) {
        char a = text[i];
        char b = text[i + 1];

        if ((is_value(a) || a == ')') && (is_value(b) || b == '!' || b == '(')) {
            text.insert(text.begin() + i + 1, '*'); // Insert the 'and' operator.
            i++; // Skip the new 'and' operator.
        }
    }

    return text;
}

/**
* Calculate the reverse polish notation of the equation using the shunting yard algorithm.
*/
std::vector<char> Equation::to_rpn(std::string text)
{
    std::vector<char> output;
    std::stack<char> stack;

    // Iterate over the canonical string to calculate the rpn.
    for (char token : text) {
        // If the token is a variable or literal then queue it to the output.
        if (is_value(token)) {
            output.push_back(token);
        }
        // If the token is an operator.
        else if (is_operator(token)) {
            while (!stack.empty() && token_precedence(stack.top()) > token_precedence(token)) {
                output.push_back(stack.top());
                stack.pop();
            }
            stack.push(token);
        }
        // If the token is a left parenthesis then push it to the stack.
        else if (token == '(') {
            stack.push(token);
        }
        // If the token is a right parenthesis then pop the stack to the output until the left parenthesis is found.
        else if (token == ')') {
            // Pop the stack to the output.
            while (!stack.empty() && stack.top() != '(') {
                output.push_back(stack.top());
                stack.pop();
            }

            // Remove '(' from the stack.
            if (!stack.empty() && stack.top() == '(') {
                stack.pop();
            }
            else { // If the '(' was not found then there are unequal or mis-matched parenthesis.
                std::cout << "Error: Unbalanced parenthesis." << std::endl;
            }
        }
    }

    // Pop the rest of the stack to the output.
    while (!stack.empty()) {
        output.push_back(stack.top());
        stack.pop();
    }

    // Return the output.
    return output;
}

std::map<char, bool> Equation::extract_variables(std::string text)
{
    // Map to store the variables.
    std::map<char, bool> vars;

    // Every token that is a variable is added to the vars map.
    for (auto token : text) {
        if (is_variable(token)) {
            vars[token] = false;
        }
    }

    return vars;
}

int Equation::token_precedence(char token)
{
    switch (token) {
    case '!':
        return 3;
    case '*':
        return 2;
    case '+':
        return 1;
    default:
        return 0;
    }
}

bool Equation::is_operator(char token)
{
    return token == '*' || token == '+' || token == '!';
}

bool Equation::is_variable(char token)
{
    return token >= 'A' && token <= 'Z';
}

bool Equation::is_literal(char token)
{
    return token == '0' || token == '1';
}

bool Equation::is_value(char token)
{
    return is_variable(token) || is_literal(token);
}

Equation::Equation(std::string text)
{
    // Store the original equation to display later.
    this->original = text;

    // Canonicalize the input text.
    this->canonical = canonicalize(text);

    // Extract the variables from the equation.
    this->variables = extract_variables(this->canonical);

    // Calculate the rpn notation of the equation.
    this->rpn = to_rpn(this->canonical);

    // Generate the string representation of the rpn vector.
    this->rpn_str = std::string(this->rpn.data(), this->rpn.size());
}

const std::string &Equation::get_original()
{
    return this->original;
}

const std::string &Equation::get_canonical()
{
    return this->canonical;
}

const std::string &Equation::get_rpn()
{
    return this->rpn_str;
}

const std::map<char, bool>& Equation::get_variables()
{
    return this->variables;
}

bool Equation::evaluate(std::map<char, bool> vars)
{
    std::stack<bool> stack;
    bool value = false;

    // Evaluate the equations using the rpn notation.
    for (auto token : this->rpn) {
        // If the token is a variable then push the value to the stack.
        if (is_variable(token)) {
            stack.push(vars[token]);
        }
        // If the token is a literal then push the value to the stack.
        else if (is_literal(token)) {
            stack.push(token == '1');
        }
        // If the token is a operaton then perform the calculation.
        else if (is_operator(token)) {
            switch (token) {
            case '!':
                value = stack.top();
                stack.pop();
                stack.push(!value);
                break;
            case '*':
                value = stack.top();
                stack.pop();
                value = value && stack.top();
                stack.pop();
                stack.push(value);
                break;
            case '+':
                value = stack.top();
                stack.pop();
                value = value || stack.top();
                stack.pop();
                stack.push(value);
                break;
            }
        }
    }

    return stack.top();
}
