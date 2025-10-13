#include "ExpressionTree.h"
#include <iostream>
#include <sstream>

// To be used with printing
std::ostream& operator<<(std::ostream& os, const node_ptr& node) {
    if (!node) {
        os << "(null)";
    } else {
        os << node->toString();
    }
    return os;
}

/*
CONSTANT FUNCTIONS
*/
double Constant::evaluate(const SymbolTable& symbols) const {
    return value;
}

std::string Constant::toString() const {
    std::ostringstream ss;
    // keep some decimal info; ostream formatting is simple here
    ss << value;
    return ss.str();
}

node_ptr Constant::derivative(const std::string& var) const {
    // derivative of a constant is 0.0
    return std::make_shared<Constant>(0.0);
}

/*
VARIABLE FUNCTIONS
*/
double Variable::evaluate(const SymbolTable& symbols) const {
    // Find variable iterator in symbol table
    auto itr = symbols.find(name);

    // Check if symbol is not found
    if (itr == symbols.end()) {
        throw std::runtime_error("Variable value not found for: " + name);
    }
    return itr->second;
}

std::string Variable::toString() const {
    return name;
}

node_ptr Variable::derivative(const std::string& var) const {
    // Derivative of constant is 1.0 when variable matches, otherwise 0.0
    if (name == var) {
        return std::make_shared<Constant>(1.0);
    }
    return std::make_shared<Constant>(0.0);
}

/*
ADDITION FUNCTIONS
*/
Add::Add(node_ptr left, node_ptr right) : BinaryOp(left, right) {}

double Add::evaluate(const SymbolTable& symbols) const {
    return left->evaluate(symbols) + right->evaluate(symbols);
}

std::string Add::toString() const {
    return "(" + left->toString() + " + " + right->toString() + ")";
}

node_ptr Add::derivative(const std::string& var) const {
    // d(u+v) = du + dv
    return std::make_shared<Add>(left->derivative(var), right->derivative(var));
}

/*
SUBSTITUTION FUNCTIONS
*/
Sub::Sub(node_ptr left, node_ptr right) : BinaryOp(left, right) {}

double Sub::evaluate(const SymbolTable& symbols) const {
    return left->evaluate(symbols) - right->evaluate(symbols);
}

std::string Sub::toString() const {
    return "(" + left->toString() + " - " + right->toString() + ")";
}

node_ptr Sub::derivative(const std::string& var) const {
    // d(u-v) = du - dv
    return std::make_shared<Sub>(left->derivative(var), right->derivative(var));
}

/*
MULTIPLICATION FUNCTIONS
*/
Mul::Mul(node_ptr left, node_ptr right) : BinaryOp(left, right) {}

double Mul::evaluate(const SymbolTable& symbols) const {
    return left->evaluate(symbols) * right->evaluate(symbols);
}

std::string Mul::toString() const {
    return "(" + left->toString() + " * " + right->toString() + ")";
}

node_ptr Mul::derivative(const std::string& var) const {
    // d(u*v) = u*dv + v*du
    node_ptr du = left->derivative(var);
    node_ptr dv = right->derivative(var);

    // Sum left and right sides
    return std::make_shared<Add>(
        std::make_shared<Mul>(left, dv), 
        std::make_shared<Mul>(right, du)
    );
}

/*
DIVISION FUNCTIONS
*/
Div::Div(node_ptr left, node_ptr right) : BinaryOp(left, right) {}

double Div::evaluate(const SymbolTable& symbols) const {
    if (right->evaluate(symbols) == 0.0) {
        throw std::runtime_error("Cannot divide by 0!");
    }
    return left->evaluate(symbols) / right->evaluate(symbols);
}

std::string Div::toString() const {
    return "(" + left->toString() + " / " + right->toString() + ")";
}

node_ptr Div::derivative(const std::string& var) const {
    // d(u/v) = (v*du - u*dv) / (v*v)
    node_ptr du = left->derivative(var);
    node_ptr dv = right->derivative(var);

    // v*du - u*dv -> v*du - u*dv
    node_ptr leftTerm = std::make_shared<Sub>(
        std::make_shared<Mul>(right, du), 
        std::make_shared<Mul>(left, dv));

    // v * v
    node_ptr rightTerm = std::make_shared<Mul>(right, right);

    // Divide left by right side
    return std::make_shared<Div>(leftTerm, rightTerm);
}