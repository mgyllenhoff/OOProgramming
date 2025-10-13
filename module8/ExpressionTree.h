#pragma once
#include <memory>
#include <map>

class Node;
// Shared pointer allows sharing subtrees!
using node_ptr = std::shared_ptr<Node>;
using SymbolTable = std::map<std::string, double>;

// Non-member operator << for help with printing
std::ostream& operator<<(std::ostream& os, const node_ptr& node);

// Base class for all nodes in the expression tree
class Node {
    public:
        // Destructor
        virtual ~Node() = default;

        // Evaluate an expression tree
        virtual double evaluate(const SymbolTable& symbols) const = 0;

        // Return string representation
        virtual std::string toString() const = 0;

        // Create the derivative (new expression tree) with respect to a variable
        virtual node_ptr derivative(const std::string& var) const = 0;
};

// Declare Constant as a subclass of Node
class Constant : public Node {
    public:
        explicit Constant(double v) : value(v) {}
        double evaluate(const SymbolTable& symbols) const override;
        std::string toString() const override;
        node_ptr derivative(const std::string& var) const override;
    private:
        double value;
};

// Declare Variable as a subclass of Node
class Variable : public Node {
    public:
        explicit Variable(const std::string& n) : name(n) {}
        double evaluate(const SymbolTable& symbols) const override;
        std::string toString() const override;
        node_ptr derivative(const std::string& var) const override;
    private:
        std::string name;
};

// Binary operation abstract class (so I do not need to repeat for each binary operation)
class BinaryOp : public Node {
public:
    BinaryOp(node_ptr l, node_ptr r) : left(std::move(l)), right(std::move(r)) {}

protected:
    node_ptr left;
    node_ptr right;
};

// Addition of nodes
class Add : public BinaryOp {
    public:
        Add(node_ptr left, node_ptr right);
        double evaluate(const SymbolTable& symbols) const override;
        std::string toString() const override;
        node_ptr derivative(const std::string& var) const override;
};

// Subtraction of nodes
class Sub : public BinaryOp {
    public:
        Sub(node_ptr left, node_ptr right);
        double evaluate(const SymbolTable& symbols) const override;
        std::string toString() const override;
        node_ptr derivative(const std::string& var) const override;
};

// Multiplication of nodes
class Mul : public BinaryOp {
    public:
        Mul(node_ptr left, node_ptr right);
        double evaluate(const SymbolTable& symbols) const override;
        std::string toString() const override;
        node_ptr derivative(const std::string& var) const override;
};

// Division of nodes
class Div : public BinaryOp {
    public:
        Div(node_ptr left, node_ptr right);
        double evaluate(const SymbolTable& symbols) const override;
        std::string toString() const override;
        node_ptr derivative(const std::string& var) const override;
};
