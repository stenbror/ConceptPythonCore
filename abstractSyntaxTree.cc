#include <abstractSyntaxTree.h>

using namespace PythonCore::Runtime;


// ASTNode - base node ////////////////////////////////////////////////////////////////////////////////////////////////
ASTNode::ASTNode(unsigned int start, unsigned int end, NodeKind kind) : m_StartPos(start), m_EndPos(end), m_Kind(kind) {};

unsigned int ASTNode::getStartPosition() { return m_StartPos; }
unsigned int ASTNode::getEndPosition() { return m_EndPos; }
ASTNode::NodeKind ASTNode::kind() { return m_Kind; }
bool ASTNode::isKind(NodeKind kind) { return m_Kind == kind; }
bool ASTNode::isExpressionNode() 
{ 
    switch (m_Kind)
    {
        case NodeKind::NK_NAMED_EXPR:
        case NodeKind::NK_TEST:
        case NodeKind::NK_LAMBDA:
        case NodeKind::NK_OR_TEST:
        case NodeKind::NK_AND_TEST: 
        case NodeKind::NK_NOT_TEST:
        case NodeKind::NK_LESS:
        case NodeKind::NK_LESS_EQUAL:
        case NodeKind::NK_EQUAL:
        case NodeKind::NK_GREATER_EQUAL:
        case NodeKind::NK_GREATER: 
        case NodeKind::NK_NOT_EQUAL: 
        case NodeKind::NK_IN:
        case NodeKind::NK_NOT_IN:
        case NodeKind::NK_IS:
        case NodeKind::NK_IS_NOT:
        case NodeKind::NK_STAR_EXPR:
        case NodeKind::NK_OR:
        case NodeKind::NK_XOR: 
        case NodeKind::NK_AND:
        case NodeKind::NK_SHIFT_LEFT:
        case NodeKind::NK_SHIFT_RIGHT:
        case NodeKind::NK_PLUS:
        case NodeKind::NK_MINUS:
        case NodeKind::NK_MUL:
        case NodeKind::NK_DIV:
        case NodeKind::NK_FLOOR_DIV:
        case NodeKind::NK_MODULO:
        case NodeKind::NK_MATRICE:
        case NodeKind::NK_POWER: 
        case NodeKind::NK_ATOM_EXPR:
        case NodeKind::NK_NAME:
        case NodeKind::NK_NUMBER:
        case NodeKind::NK_STRING:
        case NodeKind::NK_LIST:
        case NodeKind::NK_TUPLE:
        case NodeKind::NK_DICTIONARY:
        case NodeKind::NK_SET:
        case NodeKind::NK_TRUE:
        case NodeKind::NK_FALSE:
        case NodeKind::NK_NONE: 
        case NodeKind::NK_ELIPSIS:
        case NodeKind::NK_TEST_LIST_COMP:
        case NodeKind::NK_TRAILER:
        case NodeKind::NK_SUBSCRIPTION_LIST:
        case NodeKind::NK_SUBSCRIPTION:
        case NodeKind::NK_EXPR_LIST:
        case NodeKind::NK_TEST_LIST:
        case NodeKind::NK_SYNC_COMP_FOR:
        case NodeKind::NK_COMP_FOR:
        case NodeKind::NK_COMP_IF:
        case NodeKind::NK_YIELD:
        case NodeKind::NK_YIELD_FROM:
            return true;
        default:
            return false;
    } 
};

bool ASTNode::isStatementNode() 
{ 
    switch(m_Kind)
    {
        case NodeKind::NK_ASYNC_STMT:
        case NodeKind::NK_IF_STMT:
        case NodeKind::NK_ELIF_STMT:
        case NodeKind::NK_ELSE_STMT: 
        case NodeKind::NK_WHILE_STMT: 
        case NodeKind::NK_FOR_STMT:
        case NodeKind::NK_TRY_STMT: 
        case NodeKind::NK_EXCEPT_STMT:
        case NodeKind::NK_FINALLY_STMT:
        case NodeKind::NK_WITH_STMT:
        case NodeKind::NK_WITH_ITEM:
        case NodeKind::NK_SUITE:
        case NodeKind::NK_PLUS_ASSIGN:
        case NodeKind::NK_MINUS_ASSIGN: 
        case NodeKind::NK_MUL_ASSIGN:
        case NodeKind::NK_DIV_ASSIGN:
        case NodeKind::NK_FLOOR_DIV_ASSIGN: 
        case NodeKind::NK_POWER_ASSIGN:
        case NodeKind::NK_MATRICE_ASSIGN:
        case NodeKind::NK_MODULO_ASSIGN:
        case NodeKind::NK_SHIFT_LEFT_ASSIGN:
        case NodeKind::NK_SHIFT_RIGHT_ASSIGN:
        case NodeKind::NK_BIT_AND_ASSIGN:
        case NodeKind::NK_BIT_XOR_ASSIGN:
        case NodeKind::NK_BIT_OR_ASSIGN:
        case NodeKind::NK_ASSIGN:
        case NodeKind::NK_ANN_ASSIGN:
        case NodeKind::NK_SIMPLE_STMT_LIST:
        case NodeKind::NK_DEL:
        case NodeKind::NK_PASS:
        case NodeKind::NK_BREAK:
        case NodeKind::NK_CONTINUE:
        case NodeKind::NK_RETURN:
        case NodeKind::NK_RAISE:
        case NodeKind::NK_YIELD_STMT:
        case NodeKind::NK_IMPORT_NAME:
        case NodeKind::NK_IMPORT_FROM:
        case NodeKind::NK_DOTTED_NAME:
        case NodeKind::NK_DOTTED_AS_NAME:
        case NodeKind::NK_DOTTED_AS_NAMES:
        case NodeKind::NK_IMPORT_AS_NAME:
        case NodeKind::NK_IMPORT_AS_NAMES:
        case NodeKind::NK_GLOBAL_STMT: 
        case NodeKind::NK_NONLOCAL_STMT:
        case NodeKind::NK_ASSERT_STMT:
        case NodeKind::NK_DECORATED:
        case NodeKind::NK_DECORATOR:
        case NodeKind::NK_DECORATORS:
        case NodeKind::NK_ASYNC_FUNCDEF:
        case NodeKind::NK_FUNCDEF:
        case NodeKind::NK_PARAMETERS:
        case NodeKind::NK_CLASS:
        case NodeKind::NK_ARGLIST:
        case NodeKind::NK_ARGUMENT:
        case NodeKind::NK_TYPED_ARGS_LIST:
        case NodeKind::NK_VAR_ARGS_LIST:
        case NodeKind::NK_TFP_DEF:
        case NodeKind::NK_VFP_DEF:
        case NodeKind::NK_STMT_LIST:
        case NodeKind::NK_SINGLE_INPUT:
        case NodeKind::NK_FILE_INPUT:
        case NodeKind::NK_EVAL_INPUT:
        case NodeKind::NK_FUNC_BODY_SUITE:
        case NodeKind::NK_FUNC_TYPE_INPUT: 
        case NodeKind::NK_FUNC_TYPE:
        case NodeKind::NK_TYPE_LIST:
            return true;
        default:
            return false; 
    }
}


// ASTUnaryExpressionNode /////////////////////////////////////////////////////////////////////////////////////////////
ASTUnaryExpressionNode::ASTUnaryExpressionNode( unsigned int start, 
                                    unsigned int end, 
                                    NodeKind kind,
                                    std::shared_ptr<Token> op1,
                                    std::shared_ptr<ASTExpressionNode> right
                                ) 
                                : ASTExpressionNode(start, end, kind),
                                    m_Op1(op1), m_Right(right) {};

std::shared_ptr<Token> ASTUnaryExpressionNode::getOperatorOne() { return m_Op1; };

std::shared_ptr<ASTExpressionNode> ASTUnaryExpressionNode::getRightNode() { return m_Right; };


// ASTBinaryExpressionNode ////////////////////////////////////////////////////////////////////////////////////////////
ASTBinaryExpressionNode::ASTBinaryExpressionNode( unsigned int start, 
                                    unsigned int end, 
                                    NodeKind kind,
                                    std::shared_ptr<ASTExpressionNode> left,
                                    std::shared_ptr<Token> op1,
                                    std::shared_ptr<ASTExpressionNode> right
                                ) 
                                : ASTExpressionNode(start, end, kind),
                                    m_Left(left), m_Op1(op1), m_Right(right) {};

std::shared_ptr<ASTExpressionNode> ASTBinaryExpressionNode::getLeftNode() { return m_Left; };

std::shared_ptr<Token> ASTBinaryExpressionNode::getOperatorOne() { return m_Op1; };

std::shared_ptr<ASTExpressionNode> ASTBinaryExpressionNode::getRightNode() { return m_Right; };


//
ASTComparisonExpressionNode::ASTComparisonExpressionNode(
            unsigned int start,
            unsigned int end, 
            NodeKind kind,
            std::shared_ptr<ASTExpressionNode> left,
            std::shared_ptr<Token> op1,
            std::shared_ptr<Token> op2,
            std::shared_ptr<ASTExpressionNode> right
        ) : ASTExpressionNode(start, end, kind), m_Left(left), m_Op1(op1), m_Op2(op2), m_Right(right) {}

std::shared_ptr<ASTExpressionNode> ASTComparisonExpressionNode::getLeftNode() { return m_Left; }
std::shared_ptr<Token> ASTComparisonExpressionNode::getOperatorOne() { return m_Op1; }
std::shared_ptr<Token> ASTComparisonExpressionNode::getOperatorTwo() { return m_Op2; }
std::shared_ptr<ASTExpressionNode> ASTComparisonExpressionNode::getRightNode() { return m_Right; }


// ASTTestExpressionNode //////////////////////////////////////////////////////////////////////////////////////////////
ASTTestExpressionNode::ASTTestExpressionNode(  unsigned int start, 
                                    unsigned int end, 
                                    std::shared_ptr<ASTExpressionNode> left,
                                    std::shared_ptr<Token> op1,
                                    std::shared_ptr<ASTExpressionNode> right,
                                    std::shared_ptr<Token> op2,
                                    std::shared_ptr<ASTExpressionNode> next
                                ) 
                                : ASTExpressionNode(start, end, NodeKind::NK_TEST),
                                    m_Left(left), m_Op1(op1), m_Right(right), m_Op2(op2), m_Next(next) {};

std::shared_ptr<ASTExpressionNode> ASTTestExpressionNode::getLeftNode() { return m_Left; };

std::shared_ptr<Token> ASTTestExpressionNode::getOperatorOne() { return m_Op1; };

std::shared_ptr<ASTExpressionNode> ASTTestExpressionNode::getRightNode() { return m_Right; };

std::shared_ptr<Token> ASTTestExpressionNode::getOperatorTwo() { return m_Op2; };

std::shared_ptr<ASTExpressionNode> ASTTestExpressionNode::getNextNode() { return m_Next; };


// ASTLambdaExpressionNode ////////////////////////////////////////////////////////////////////////////////////////////
ASTLambdaExpressionNode::ASTLambdaExpressionNode(    unsigned int start, 
                                        unsigned int end, 
                                        std::shared_ptr<Token> op1,
                                        std::shared_ptr<ASTExpressionNode> left,
                                        std::shared_ptr<Token> op2,
                                        std::shared_ptr<ASTExpressionNode> right
                                    )    
                                    : ASTExpressionNode(start, end, NodeKind::NK_LAMBDA),
                                        m_Op1(op1), m_Left(left), m_Op2(op2), m_Right(right) {};

std::shared_ptr<Token> ASTLambdaExpressionNode::getOperatorOne() { return m_Op1; };

std::shared_ptr<ASTExpressionNode> ASTLambdaExpressionNode::getLeftNode() { return m_Left; };

std::shared_ptr<Token> ASTLambdaExpressionNode::getOperatorTwo() { return m_Op2; };

std::shared_ptr<ASTExpressionNode> ASTLambdaExpressionNode::getRightNode() { return m_Right; };


// ASTAtomExpressionNode //////////////////////////////////////////////////////////////////////////////////////////////
ASTAtomExpressionNode::ASTAtomExpressionNode(  unsigned int start, 
                                    unsigned int end, 
                                    std::shared_ptr<Token> op1,
                                    std::shared_ptr<ASTExpressionNode> left,
                                    std::shared_ptr<ASTExpressionNode> right
                                  )    
                                  : ASTExpressionNode(start, end, NodeKind::NK_ATOM_EXPR),
                                        m_Op1(op1), m_Left(left), m_Right(right) {};

std::shared_ptr<Token> ASTAtomExpressionNode::getOperatorOne() { return m_Op1; };

std::shared_ptr<ASTExpressionNode> ASTAtomExpressionNode::getLeftNode() { return m_Left; };

std::shared_ptr<ASTExpressionNode> ASTAtomExpressionNode::getRightNode() { return m_Right; };

bool ASTAtomExpressionNode::isAwait() { return m_Op1 != nullptr; };


// ASTLiteralExpressionNode ///////////////////////////////////////////////////////////////////////////////////////////
ASTLiteralExpressionNode::ASTLiteralExpressionNode(   unsigned int start, 
                                        unsigned int end,
                                        NodeKind kind,
                                        std::shared_ptr<Token> op1
                                    )
                                    : ASTExpressionNode(start, end, kind), m_Op1(op1) {};

std::shared_ptr<Token> ASTLiteralExpressionNode::getOperatorOne() { return m_Op1; };

bool ASTLiteralExpressionNode::isNameLiteral() { return m_Kind == NodeKind::NK_NAME; };

bool ASTLiteralExpressionNode::isNumberLiteral() { return m_Kind == NodeKind::NK_NUMBER; };

bool ASTLiteralExpressionNode::isStringLiteral() { return m_Kind == NodeKind::NK_STRING; };


// ASTStringListExpressionNode ////////////////////////////////////////////////////////////////////////////////////////
ASTStringListExpressionNode::ASTStringListExpressionNode(unsigned int start, unsigned int end) : ASTExpressionNode(start, end, NodeKind::NK_STRING_LIST)
{};

void ASTStringListExpressionNode::addStringNode(std::shared_ptr<Token> node) {  m_StringList->push_back(node); };

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTStringListExpressionNode::getStringNodes() { return m_StringList; };

unsigned int ASTStringListExpressionNode::count() { return (m_StringList.get())->size(); };
