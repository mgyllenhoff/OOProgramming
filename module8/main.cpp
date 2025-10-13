#include "ExpressionTree.h"
#include <iostream>

int main() {
    // Symbol table creation
    SymbolTable symTab;
    symTab["Xray"]  = 2.0;
    symTab["Yellow"]= 3.0;
    symTab["Zebra"] = 5.0;
    symTab["x"] = 4.0;
    symTab["y"] = 8.0;

    // Testing
    std::cout << "TEST 0 (Constant)" << std::endl;
    node_ptr node0 = std::make_shared<Constant>(10.0);
    std::cout << "Expression Tree: " << node0 << std::endl;
    std::cout << "Evaluation: " << node0->evaluate(symTab) << std::endl;
    node_ptr derivedNode0 = node0->derivative("x");
    std::cout << "Derivative: " << derivedNode0 << std::endl;
    std::cout << "Evaluation of derivative: " << derivedNode0->evaluate(symTab) << std::endl;
    std::cout << "\n";

    std::cout << "TEST 1 (Variable)" << std::endl;
    node_ptr node1 = std::make_shared<Variable>("Xray");
    std::cout << "Expression Tree: " << node1 << std::endl;
    std::cout << "Evaluation: " << node1->evaluate(symTab) << std::endl;
    node_ptr derivedNode1 = node1->derivative("Xray");
    std::cout << "Derivative: " << derivedNode1 << std::endl;
    std::cout << "Evaluation of derivative: " << derivedNode1->evaluate(symTab) << std::endl;
    std::cout << "\n";

    std::cout << "TEST 2 (Constant + Variable)" << std::endl;
    node_ptr node2 = std::make_shared<Add>(
                        std::make_shared<Constant>(2.0), 
                        std::make_shared<Variable>("x")
                    );
    std::cout << "Expression Tree: " << node2 << std::endl;
    std::cout << "Evaluation: " << node2->evaluate(symTab) << std::endl;
    node_ptr derivedNode2 = node2->derivative("x");
    std::cout << "Derivative: " << derivedNode2 << std::endl;
    std::cout << "Evaluation of derivative: " << derivedNode2->evaluate(symTab) << std::endl;
    std::cout << "\n";

    std::cout << "TEST 3 (Variable - Constant)" << std::endl;
    node_ptr node3 = std::make_shared<Sub>(
                        std::make_shared<Variable>("Xray"),
                        std::make_shared<Constant>(3.0)
                    );
    std::cout << "Expression Tree: " << node3 << std::endl;
    std::cout << "Evaluation: " << node3->evaluate(symTab) << std::endl;
    node_ptr derivedNode3 = node3->derivative("Xray");
    std::cout << "Derivative: " << derivedNode3 << std::endl;
    std::cout << "Evaluation of derivative: " << derivedNode3->evaluate(symTab) << std::endl;
    std::cout << "\n";

    std::cout << "TEST 4 (Variable * Variable)" << std::endl;
    node_ptr node4 = std::make_shared<Mul>(
                        std::make_shared<Variable>("Yellow"),
                        std::make_shared<Variable>("x")
                    );
    std::cout << "Expression Tree: " << node4 << std::endl;
    std::cout << "Evaluation: " << node4->evaluate(symTab) << std::endl;
    node_ptr derivedNode4 = node4->derivative("Yellow");
    std::cout << "Derivative: " << derivedNode4 << std::endl;
    std::cout << "Evaluation of derivative: " << derivedNode4->evaluate(symTab) << std::endl;
    std::cout << "\n";

    std::cout << "TEST 5 (Variable / Variable)" << std::endl;
    node_ptr node5 = std::make_shared<Div>(
                        std::make_shared<Variable>("y"),
                        std::make_shared<Variable>("Xray")
                    );
    std::cout << "Expression Tree: " << node5 << std::endl;
    std::cout << "Evaluation: " << node5->evaluate(symTab) << std::endl;
    node_ptr derivedNode5 = node5->derivative("y");
    std::cout << "Derivative: " << derivedNode5 << std::endl;
    std::cout << "Evaluation of derivative: " << derivedNode5->evaluate(symTab) << std::endl;
    std::cout << "\n";

    std::cout << "TEST 6 (Example from assignment)" << std::endl;
    node_ptr node6 = std::make_shared<Add>(
                        std::make_shared<Mul>(
                            std::make_shared<Constant>(2.3), 
                            std::make_shared<Variable>("Xray")
                        ), 
                        std::make_shared<Mul>(
                            std::make_shared<Variable>("Yellow"), 
                            std::make_shared<Sub>(
                                std::make_shared<Variable>("Zebra"),
                                std::make_shared<Variable>("Xray")
                            )
                        )
                    );
    std::cout << "Expression Tree: " << node6 << std::endl;
    std::cout << "Evaluation: " << node6->evaluate(symTab) << std::endl;
    node_ptr derivedNode6 = node6->derivative("Xray");
    std::cout << "Derivative: " << derivedNode6 << std::endl;
    std::cout << "Evaluation of derivative: " << derivedNode6->evaluate(symTab) << std::endl;
    std::cout << "\n";

    std::cout << "TEST 7 (Another complex case)" << std::endl;
    node_ptr node7 = std::make_shared<Div>(
                        std::make_shared<Mul>(
                            std::make_shared<Variable>("Xray"), 
                            std::make_shared<Variable>("Yellow")
                        ), 
                        std::make_shared<Add>(
                            std::make_shared<Variable>("Xray"), 
                            std::make_shared<Variable>("Yellow")
                        )
                    );
    std::cout << "Expression Tree: " << node7 << std::endl;
    std::cout << "Evaluation: " << node7->evaluate(symTab) << std::endl;
    node_ptr derivedNode7 = node7->derivative("Xray");
    std::cout << "Derivative: " << derivedNode7 << std::endl;
    std::cout << "Evaluation of derivative: " << derivedNode7->evaluate(symTab) << std::endl;
    std::cout << "\n";

    std::cout << "TEST 8 (Multipy same variable)" << std::endl;
    node_ptr node8 = std::make_shared<Mul>(
                            std::make_shared<Variable>("x"), 
                            std::make_shared<Variable>("x")
                    );
    std::cout << "Expression Tree: " << node8 << std::endl;
    std::cout << "Evaluation: " << node8->evaluate(symTab) << std::endl;
    node_ptr derivedNode8 = node8->derivative("x");
    std::cout << "Derivative: " << derivedNode8 << std::endl;
    std::cout << "Evaluation of derivative: " << derivedNode8->evaluate(symTab) << std::endl;
    std::cout << "\n";

    std::cout << "TEST 9 (Division by zero)" << std::endl;
    node_ptr node9 = std::make_shared<Div>(
                            std::make_shared<Variable>("Zebra"), 
                            std::make_shared<Constant>(0.0)
                    );
    std::cout << "Expression Tree: " << node9 << std::endl;
    std::cout << "Evaluation: " << node9->evaluate(symTab) << std::endl;
    node_ptr derivedNode9 = node9->derivative("Zebra");
    std::cout << "Derivative: " << derivedNode9 << std::endl;
    std::cout << "Evaluation of derivative: " << derivedNode9->evaluate(symTab) << std::endl;
    std::cout << "\n";

    return 0;
}
