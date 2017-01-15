#pragma once

#include <map>
#include <string>
#include <vector>


class Equation
{
private:
    std::string original;
    std::string canonical;
    std::vector<char> rpn;
    std::string rpn_str;
    std::map<char, bool> variables;

    std::string canonicalize(std::string text);
    std::vector<char> to_rpn(std::string text);
    std::map<char, bool> extract_variables(std::string text);

    int token_precedence(char token);
    bool is_operator(char token);
    bool is_variable(char token);
    bool is_literal(char token);
    bool is_value(char token);
public:
    Equation(std::string text);
    const std::string &get_original();
    const std::string &get_canonical();
    const std::string &get_rpn();
    const std::map<char, bool> &get_variables();
    bool evaluate(std::map<char, bool> vars);
};