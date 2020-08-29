#include <abstractSyntaxTree.h>
#include <iostream>

using namespace PythonCore::Runtime;

int main(int argc, char *argv[])
{
    std::cout << "POC - PythonCore Parser" << std::endl;
    auto node = std::make_shared<ASTBinaryExpressionNode>(0, 5, ASTNode::NodeKind::NK_MUL, nullptr, nullptr, nullptr);
    if (node->isKind(ASTNode::NodeKind::NK_MUL)) std::cout << " [EXPRESSION => '*']" << std::endl;
    if (node->isExpressionNode()) std::cout << " Yep! Expression Node is here!" << std::endl;
    if (node->isStatementNode()) std::cout << " Yep! Statement Node is here!" << std::endl;

    return 0;
}