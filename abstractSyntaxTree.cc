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
        case NodeKind::NK_ARGLIST:
        case NodeKind::NK_ARGUMENT:
        case NodeKind::NK_LIST_ARGUMENT:
        case NodeKind::NK_KW_ARGUMENT:
        case NodeKind::NK_COMP_ARGUMENT:
        case NodeKind::NK_COLON_ASSIGN_ARGUMENT:
        case NodeKind::NK_ASSIGN_ARGUMENT:
        case NodeKind::NK_WITH_ITEM:
        case NodeKind::NK_EXCEPT_CLAUSE:
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

void ASTStringListExpressionNode::setEndPosition(unsigned int end) { m_EndPos = end; }


// ASTSubscriptionExpressionNode //////////////////////////////////////////////////////////////////////////////////////
ASTSubscriptionExpressionNode::ASTSubscriptionExpressionNode(   unsigned int start, 
                                                                unsigned int end,
                                                                std::shared_ptr<ASTExpressionNode> left,
                                                                std::shared_ptr<Token> op1,
                                                                std::shared_ptr<ASTExpressionNode> right,
                                                                std::shared_ptr<Token> op2,
                                                                std::shared_ptr<ASTExpressionNode> next
                                                            )
                                                            : ASTExpressionNode(start, end, ASTNode::NodeKind::NK_SUBSCRIPTION),
                                                            m_Left(left), m_Op1(op1), m_Right(right), m_Op2(op2), m_Next(next) {}

std::shared_ptr<ASTExpressionNode> ASTSubscriptionExpressionNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTSubscriptionExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTSubscriptionExpressionNode::getRight() { return m_Right; }

std::shared_ptr<Token> ASTSubscriptionExpressionNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTExpressionNode> ASTSubscriptionExpressionNode::getNext() { return m_Next; }


// ASTListExpressionNode //////////////////////////////////////////////////////////////////////////////////////////////
ASTListExpressionNode::ASTListExpressionNode(unsigned int start, unsigned int end, ASTNode::NodeKind kind) : ASTExpressionNode(start, end, kind) {}

void ASTListExpressionNode::addNodes(std::shared_ptr<ASTExpressionNode> node, std::shared_ptr<Token> comma)
{
    m_Nodes->push_back(node); m_CommaNodes->push_back(comma);
}

void ASTListExpressionNode::addEndPosition(unsigned int end)
{
    m_EndPos = end;
}

unsigned int ASTListExpressionNode::count() { return m_Nodes->size(); }

std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> ASTListExpressionNode::getNodes()
{
    return m_Nodes;
}

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTListExpressionNode::getCommaNodes()
{
    return m_CommaNodes;
}


// ASTIfComprehensionExpressionNode ///////////////////////////////////////////////////////////////////////////////////
ASTIfComprehensionExpressionNode::ASTIfComprehensionExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op, std::shared_ptr<ASTExpressionNode> right, std::shared_ptr<ASTExpressionNode> next)
    : ASTExpressionNode(start, end, ASTNode::NodeKind::NK_COMP_IF), m_Op(op), m_Right(right), m_Next(next) { }

std::shared_ptr<Token> ASTIfComprehensionExpressionNode::getOperatorOne() { return m_Op; }

std::shared_ptr<ASTExpressionNode> ASTIfComprehensionExpressionNode::getRight() { return m_Right; }

std::shared_ptr<ASTExpressionNode> ASTIfComprehensionExpressionNode::getNext() { return m_Next; }


// ASTYieldFromExpressionNode /////////////////////////////////////////////////////////////////////////////////////////
ASTYieldFromExpressionNode::ASTYieldFromExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2, std::shared_ptr<ASTExpressionNode> right)
    : ASTExpressionNode(start, end, ASTNode::NodeKind::NK_YIELD_FROM), m_Op1(op1), m_Op2(op2), m_Right(right) { }

std::shared_ptr<Token> ASTYieldFromExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTYieldFromExpressionNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTExpressionNode> ASTYieldFromExpressionNode::getRight() { return m_Right; }


// ASTYieldExpression /////////////////////////////////////////////////////////////////////////////////////////////////
ASTYieldExpression::ASTYieldExpression(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<ASTStatementNode> right)
    : ASTExpressionNode(start, end, ASTNode::NodeKind::NK_YIELD), m_Op1(op1), m_Right(right) { }

std::shared_ptr<Token> ASTYieldExpression::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTStatementNode> ASTYieldExpression::getRight() { return m_Right; }


// ASTSyncCompForExpressionNode ///////////////////////////////////////////////////////////////////////////////////////
ASTSyncCompForExpressionNode::ASTSyncCompForExpressionNode(   
                                                            unsigned int start, 
                                                            unsigned int end, 
                                                            std::shared_ptr<Token> op1, 
                                                            std::shared_ptr<ASTExpressionNode> left, 
                                                            std::shared_ptr<Token> op2, 
                                                            std::shared_ptr<ASTExpressionNode> right, 
                                                            std::shared_ptr<ASTExpressionNode> next
                                                            )
                                                            : ASTExpressionNode(start, end, ASTNode::NodeKind::NK_SYNC_COMP_FOR), m_Op1(op1), m_Left(left),
                                                                m_Op2(op2), m_Right(right), m_Next(next) { }

std::shared_ptr<Token> ASTSyncCompForExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTSyncCompForExpressionNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTSyncCompForExpressionNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTExpressionNode> ASTSyncCompForExpressionNode::getRight() { return m_Right; }

std::shared_ptr<ASTExpressionNode> ASTSyncCompForExpressionNode::getNext() { return m_Next; }


// ASTTrailerListExpressionNode ///////////////////////////////////////////////////////////////////////////////////////
ASTTrailerListExpressionNode::ASTTrailerListExpressionNode(unsigned int start, unsigned int end)
    :   ASTExpressionNode(start, end, ASTNode::NodeKind::NK_TRAILER) { }

void ASTTrailerListExpressionNode::addTrailerNode(std::shared_ptr<ASTExpressionNode> node) { m_Nodes->push_back(node); }

unsigned int ASTTrailerListExpressionNode::count() { return m_Nodes->size(); }

std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> ASTTrailerListExpressionNode::getNodes() { return m_Nodes; }

void ASTTrailerListExpressionNode::addEndPosition(unsigned int end)
{
    m_EndPos = end;
}


// ASTDotNameExpressionNode ///////////////////////////////////////////////////////////////////////////////////////////
ASTDotNameExpressionNode::ASTDotNameExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2)
    :   ASTExpressionNode(start, end, ASTNode::NodeKind::NK_DOT_NAME), m_Op1(op1), m_Op2(op2) { }

std::shared_ptr<Token> ASTDotNameExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTDotNameExpressionNode::getOperatorTwo() { return m_Op2; }


// ASTCallExpressionNode //////////////////////////////////////////////////////////////////////////////////////////////
ASTCallExpressionNode::ASTCallExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<ASTExpressionNode> right, std::shared_ptr<Token> op2)
    : ASTExpressionNode(start, end, ASTNode::NodeKind::NK_CALL), m_Op1(op1), m_Right(right), m_Op2(op2) {}

std::shared_ptr<Token> ASTCallExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTCallExpressionNode::getRight() { return m_Right; }

std::shared_ptr<Token> ASTCallExpressionNode::getOperatorTwo() { return m_Op2; }


// ASTIndexExpressionNode /////////////////////////////////////////////////////////////////////////////////////////////
ASTIndexExpressionNode::ASTIndexExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<ASTExpressionNode> right, std::shared_ptr<Token> op2)
    : ASTExpressionNode(start, end, ASTNode::NodeKind::NK_CALL), m_Op1(op1), m_Right(right), m_Op2(op2) {}

std::shared_ptr<Token> ASTIndexExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTIndexExpressionNode::getRight() { return m_Right; }

std::shared_ptr<Token> ASTIndexExpressionNode::getOperatorTwo() { return m_Op2; }


// ASTListArgumentExpressionNode //////////////////////////////////////////////////////////////////////////////////////
ASTListArgumentExpressionNode::ASTListArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2)
    : ASTExpressionNode(start, end, ASTNode::NodeKind::NK_LIST_ARGUMENT), m_Op1(op1), m_Op2(op2) { }

std::shared_ptr<Token> ASTListArgumentExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTListArgumentExpressionNode::getOperatorTwo() { return m_Op2; }


// ASTKWArgumentExpressionNode ////////////////////////////////////////////////////////////////////////////////////////
ASTKWArgumentExpressionNode::ASTKWArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2)
    : ASTExpressionNode(start, end, ASTNode::NodeKind::NK_KW_ARGUMENT), m_Op1(op1), m_Op2(op2) { }

std::shared_ptr<Token> ASTKWArgumentExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTKWArgumentExpressionNode::getOperatorTwo() { return m_Op2; }


// ASTCompArgumentExpressionNode //////////////////////////////////////////////////////////////////////////////////////
ASTCompArgumentExpressionNode::ASTCompArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<ASTExpressionNode> right)
    : ASTExpressionNode(start, end, ASTNode::NodeKind::NK_COMP_ARGUMENT), m_Op1(op1), m_Right(right) { }

std::shared_ptr<Token> ASTCompArgumentExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTCompArgumentExpressionNode::getRight() { return m_Right; }


// ASTColonAssignArgumentExpressionNode ///////////////////////////////////////////////////////////////////////////////
ASTColonAssignArgumentExpressionNode::ASTColonAssignArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2, std::shared_ptr<Token> op3)
    :   ASTExpressionNode(start, end, ASTNode::NodeKind::NK_COLON_ASSIGN_ARGUMENT), m_Op1(op1), m_Op2(op2), m_Op3(op3) {}

std::shared_ptr<Token> ASTColonAssignArgumentExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTColonAssignArgumentExpressionNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<Token> ASTColonAssignArgumentExpressionNode::getOperatorThree() { return m_Op3; }


// ASTAssignArgumentExpressionNode ////////////////////////////////////////////////////////////////////////////////////
ASTAssignArgumentExpressionNode::ASTAssignArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2, std::shared_ptr<Token> op3)
    :   ASTExpressionNode(start, end, ASTNode::NodeKind::NK_ASSIGN_ARGUMENT), m_Op1(op1), m_Op2(op2), m_Op3(op3) {}

std::shared_ptr<Token> ASTAssignArgumentExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTAssignArgumentExpressionNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<Token> ASTAssignArgumentExpressionNode::getOperatorThree() { return m_Op3; }


// ASTArgumentExpressionNode //////////////////////////////////////////////////////////////////////////////////////////
ASTArgumentExpressionNode::ASTArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1) 
    :   ASTExpressionNode(start, end, ASTNode::NodeKind::NK_ARGUMENT), m_Op1(op1) { }

std::shared_ptr<Token> ASTArgumentExpressionNode::getOperatorOne() { return m_Op1; }


// ASTCompoundLiteralExpressionNode ///////////////////////////////////////////////////////////////////////////////////
ASTCompoundLiteralExpressionNode::ASTCompoundLiteralExpressionNode( unsigned int start,
                                                                    unsigned int end,
                                                                    ASTNode::NodeKind kind,
                                                                    std::shared_ptr<Token> op1,
                                                                    std::shared_ptr<ASTExpressionNode> right,
                                                                    std::shared_ptr<Token> op2) 
    :   ASTExpressionNode(start, end, kind), m_Op1(op1), m_Right(right), m_Op2(op2) {}

std::shared_ptr<Token> ASTCompoundLiteralExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTCompoundLiteralExpressionNode::getRight() { return m_Right; }

std::shared_ptr<Token> ASTCompoundLiteralExpressionNode::getOperatorTwo() { return m_Op2; }


// ASTDictionarySetDataExpressionNode /////////////////////////////////////////////////////////////////////////////////
ASTDictionarySetDataExpressionNode::ASTDictionarySetDataExpressionNode(unsigned int start, unsigned int end, ASTNode::NodeKind kind)
    :   ASTExpressionNode(start, end, kind) { }

void ASTDictionarySetDataExpressionNode::addEndPosition(unsigned int end) { m_EndPos = end; }

void ASTDictionarySetDataExpressionNode::addSetEntry(std::shared_ptr<ASTExpressionNode> left) { m_Keys->push_back(left); }

void ASTDictionarySetDataExpressionNode::addDictionaryEntry(std::shared_ptr<ASTExpressionNode> left, std::shared_ptr<Token> colon, std::shared_ptr<ASTExpressionNode> right)
{
    m_Keys->push_back(left);
    m_Colons->push_back(colon);
    m_Values->push_back(right);
}

void ASTDictionarySetDataExpressionNode::addComma(std::shared_ptr<Token> comma) { m_Commas->push_back(comma); }

unsigned int ASTDictionarySetDataExpressionNode::count() { return m_Keys->size(); }

std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> ASTDictionarySetDataExpressionNode::getKeys() { return m_Keys; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTDictionarySetDataExpressionNode::getColons() { return m_Colons; }

std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> ASTDictionarySetDataExpressionNode::getValues() { return m_Values;}

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTDictionarySetDataExpressionNode::getCommas() { return m_Commas; }


// ASTIfStatementNode /////////////////////////////////////////////////////////////////////////////////////////////////
ASTIfStatementNode::ASTIfStatementNode( unsigned int start, 
                                        unsigned int end, 
                                        std::shared_ptr<Token> op1, 
                                        std::shared_ptr<ASTExpressionNode> left, 
                                        std::shared_ptr<Token> op2, 
                                        std::shared_ptr<ASTStatementNode> right
                                      ) : ASTStatementNode(start, end, ASTNode::NodeKind::NK_IF_STMT), m_Op1(op1), m_Left(left), m_Op2(op2), m_Right(right) {}

void ASTIfStatementNode::addElifStatement(std::shared_ptr<ASTStatementNode> node) { m_Elifs->push_back(node); }

void ASTIfStatementNode::addElseStatement(std::shared_ptr<ASTStatementNode> node) { m_Else = node; }

std::shared_ptr<Token> ASTIfStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTIfStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTIfStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTStatementNode> ASTIfStatementNode::getRight() { return m_Right; }

std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> ASTIfStatementNode::getElifsNodes() { return m_Elifs; }

std::shared_ptr<ASTStatementNode> ASTIfStatementNode::getElseNode() { return m_Else; }

unsigned int ASTIfStatementNode::count() { return m_Elifs->size(); }

void ASTIfStatementNode::addEndPosition(unsigned int end) { m_EndPos = end; }


// ASTElifStatementNode ///////////////////////////////////////////////////////////////////////////////////////////////
ASTElifStatementNode::ASTElifStatementNode( unsigned int start, 
                                        unsigned int end, 
                                        std::shared_ptr<Token> op1, 
                                        std::shared_ptr<ASTExpressionNode> left, 
                                        std::shared_ptr<Token> op2, 
                                        std::shared_ptr<ASTStatementNode> right
                                      ) : ASTStatementNode(start, end, ASTNode::NodeKind::NK_ELIF_STMT), m_Op1(op1), m_Left(left), m_Op2(op2), m_Right(right) {}

std::shared_ptr<Token> ASTElifStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTElifStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTElifStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTStatementNode> ASTElifStatementNode::getRight() { return m_Right; }


// ASTElseStatementNode ///////////////////////////////////////////////////////////////////////////////////////////////
ASTElseStatementNode::ASTElseStatementNode( unsigned int start, 
                                            unsigned int end, 
                                            std::shared_ptr<Token> op1, 
                                            std::shared_ptr<Token> op2, 
                                            std::shared_ptr<ASTStatementNode> right
                                          ) : ASTStatementNode(start, end, ASTNode::NodeKind::NK_ELSE_STMT), m_Op1(op1), m_Op2(op2), m_Right(right) {}

std::shared_ptr<Token> ASTElseStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTElseStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTStatementNode> ASTElseStatementNode::getRight() { return m_Right; }


// ASTWhileStatementNode //////////////////////////////////////////////////////////////////////////////////////////////
ASTWhileStatementNode::ASTWhileStatementNode(   unsigned int start, 
                                                unsigned int end, 
                                                std::shared_ptr<Token> op1, 
                                                std::shared_ptr<ASTExpressionNode> left, 
                                                std::shared_ptr<Token> op2, 
                                                std::shared_ptr<ASTStatementNode> right
                                            ) : ASTStatementNode(start, end, ASTNode::NodeKind::NK_WHILE_STMT), m_Op1(op1), m_Left(left), m_Op2(op2), m_Right(right) {}

void ASTWhileStatementNode::addElseStatement(std::shared_ptr<ASTStatementNode> node) { m_Else = node; }

std::shared_ptr<Token> ASTWhileStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTWhileStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTWhileStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTStatementNode> ASTWhileStatementNode::getRight() { return m_Right; }

std::shared_ptr<ASTStatementNode> ASTWhileStatementNode::getElseNode() { return m_Else; }

void ASTWhileStatementNode::addEndPosition(unsigned int end) { m_EndPos = end; }


// ASTForStatementNode ////////////////////////////////////////////////////////////////////////////////////////////////
ASTForStatementNode::ASTForStatementNode(   unsigned int start,
                                            unsigned int end,
                                            std::shared_ptr<Token> op1,
                                            std::shared_ptr<ASTExpressionNode> left,
                                            std::shared_ptr<Token> op2,
                                            std::shared_ptr<ASTExpressionNode> right,
                                            std::shared_ptr<Token> op3,
                                            std::shared_ptr<ASTTypeCommentNode> comment,
                                            std::shared_ptr<ASTStatementNode> next
                                        )
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_FOR_STMT), m_Op1(op1), m_Left(left), m_Op2(op2), m_Right(right), m_Op3(op3), m_Comment(comment), m_Next(next) { }

void ASTForStatementNode::addElseStatement(std::shared_ptr<ASTStatementNode> node) { m_Else = node; }

std::shared_ptr<Token> ASTForStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTForStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTForStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTExpressionNode> ASTForStatementNode::getRight() { return m_Right; }

std::shared_ptr<Token> ASTForStatementNode::getOperatorThree() { return m_Op3; }

std::shared_ptr<ASTTypeCommentNode> ASTForStatementNode::getTypeComment() { return m_Comment; }

std::shared_ptr<ASTStatementNode> ASTForStatementNode::getNext() { return m_Next; }

std::shared_ptr<ASTStatementNode> ASTForStatementNode::getElseNode() { return m_Else; }

void ASTForStatementNode::addEndPosition(unsigned int end) { m_EndPos = end; }


// ASTWithStatementNode ///////////////////////////////////////////////////////////////////////////////////////////////
ASTWithStatementNode::ASTWithStatementNode( unsigned int start,
                                            unsigned int end,
                                            std::shared_ptr<Token> op1,
                                            std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> nodes,
                                            std::shared_ptr<std::vector<std::shared_ptr<Token>>> commas,
                                            std::shared_ptr<Token> op2,
                                            std::shared_ptr<ASTTypeCommentNode> comment,
                                            std::shared_ptr<ASTStatementNode> right
                                          )
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_WITH_STMT), m_Op1(op1), m_Nodes(nodes), m_Commas(commas), m_Op2(op2), m_Comment(comment), m_Right(right) { }

std::shared_ptr<Token> ASTWithStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> ASTWithStatementNode::getNodes() { return m_Nodes; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTWithStatementNode::getCommas() { return m_Commas; }

std::shared_ptr<Token> ASTWithStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTTypeCommentNode> ASTWithStatementNode::getTypeComment() { return m_Comment; }

std::shared_ptr<ASTStatementNode> ASTWithStatementNode::getRight() { return m_Right; }


// ASTAsyncStatementNode //////////////////////////////////////////////////////////////////////////////////////////////
ASTAsyncStatementNode::ASTAsyncStatementNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op, std::shared_ptr<ASTStatementNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_ASYNC_STMT), m_Op1(op), m_Right(right) { }

std::shared_ptr<Token> ASTAsyncStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTStatementNode> ASTAsyncStatementNode::getRight() { return m_Right; }


// ASTSuiteStatementNode //////////////////////////////////////////////////////////////////////////////////////////////
ASTSuiteStatementNode::ASTSuiteStatementNode(   unsigned int start,
                                                unsigned int end,
                                                std::shared_ptr<Token> op1,
                                                std::shared_ptr<Token> op2,
                                                std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes,
                                                std::shared_ptr<Token> op3
                                            )
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_SUITE), m_Op1(op1), m_Op2(op2), m_Nodes(nodes), m_Op3(op3) { }

std::shared_ptr<Token> ASTSuiteStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTSuiteStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> ASTSuiteStatementNode::getNodes() { return m_Nodes; }

std::shared_ptr<Token> ASTSuiteStatementNode::getOperatorThree() { return m_Op3; }

unsigned int ASTSuiteStatementNode::count() { return m_Nodes->size(); }


// ASTExceptionClauseExpressionNode ///////////////////////////////////////////////////////////////////////////////////
ASTExceptionClauseExpressionNode::ASTExceptionClauseExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<ASTExpressionNode> left, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2)
    :   ASTExpressionNode(start, end, ASTNode::NodeKind::NK_EXCEPT_CLAUSE), m_Left(left), m_Op1(op1), m_Op2(op2) { }

std::shared_ptr<ASTExpressionNode> ASTExceptionClauseExpressionNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTExceptionClauseExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTExceptionClauseExpressionNode::getOperatorTwo() { return m_Op2; }


// ASTTryStatementNode ////////////////////////////////////////////////////////////////////////////////////////////////
ASTTryStatementNode::ASTTryStatementNode(   unsigned int start,
                                            unsigned int end,
                                            std::shared_ptr<Token> op1,
                                            std::shared_ptr<Token> op2,
                                            std::shared_ptr<ASTStatementNode> left,
                                            std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes,
                                            std::shared_ptr<ASTStatementNode> elsePart,
                                            std::shared_ptr<ASTStatementNode> finallyPart
                                        )
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_TRY_STMT), m_Op1(op1), m_Op2(op2), m_Left(left), m_Nodes(nodes), m_ElsePart(elsePart), m_FinallyPart(finallyPart) { }

std::shared_ptr<Token> ASTTryStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTTryStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTStatementNode> ASTTryStatementNode::getLeft() { return m_Left; }

std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> ASTTryStatementNode::getNodes() { return m_Nodes; }

std::shared_ptr<ASTStatementNode> ASTTryStatementNode::getElsePart() { return m_ElsePart; }

std::shared_ptr<ASTStatementNode> ASTTryStatementNode::getFinallyPart() { return m_FinallyPart; }


// ASTExceptStatementNode /////////////////////////////////////////////////////////////////////////////////////////////
ASTExceptStatementNode::ASTExceptStatementNode( unsigned int start, 
                                                unsigned int end, 
                                                std::shared_ptr<Token> op1, 
                                                std::shared_ptr<ASTExpressionNode> left, 
                                                std::shared_ptr<Token> op2, 
                                                std::shared_ptr<ASTStatementNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_EXCEPT_STMT), m_Op1(op1), m_Left(left), m_Op2(op2), m_Right(right) { }

std::shared_ptr<Token> ASTExceptStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTExceptStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTExceptStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTStatementNode> ASTExceptStatementNode::getRight() { return m_Right; }


// ASTFinnalyStatementNode ////////////////////////////////////////////////////////////////////////////////////////////
ASTFinallyStatementNode::ASTFinallyStatementNode(   unsigned int start, 
                                                    unsigned int end, 
                                                    std::shared_ptr<Token> op1, 
                                                    std::shared_ptr<Token> op2, 
                                                    std::shared_ptr<ASTStatementNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_FINALLY_STMT), m_Op1(op1), m_Op2(op2), m_Right(right) { }

std::shared_ptr<Token> ASTFinallyStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTFinallyStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTStatementNode> ASTFinallyStatementNode::getRight() { return m_Right; }


// ASTClassDefStatementNode ///////////////////////////////////////////////////////////////////////////////////////////
ASTClassDefStatementNode::ASTClassDefStatementNode( unsigned int start,
                                                    unsigned int end,
                                                    std::shared_ptr<Token> op1,
                                                    std::shared_ptr<Token> op2,
                                                    std::shared_ptr<Token> op3,
                                                    std::shared_ptr<ASTExpressionNode> left,
                                                    std::shared_ptr<Token> op4,
                                                    std::shared_ptr<Token> op5,
                                                    std::shared_ptr<ASTStatementNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_CLASS), m_Op1(op1), m_Op2(op2), m_Op3(op3), m_Left(left), m_Op4(op4), m_Op5(op5), m_Right(right) { }
                                    
std::shared_ptr<Token> ASTClassDefStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTClassDefStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<Token> ASTClassDefStatementNode::getOperatorThree() { return m_Op3; }

std::shared_ptr<ASTExpressionNode> ASTClassDefStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTClassDefStatementNode::getOperatorFour() { return m_Op4; }

std::shared_ptr<Token> ASTClassDefStatementNode::getOperatorFive() { return m_Op5; }

std::shared_ptr<ASTStatementNode> ASTClassDefStatementNode::getRight() { return m_Right; }


// ASTDecoratorStatementNode //////////////////////////////////////////////////////////////////////////////////////////
ASTDecoratorStatementNode::ASTDecoratorStatementNode(   unsigned int start,
                                                        unsigned int end,
                                                        std::shared_ptr<Token> op1,
                                                        std::shared_ptr<ASTStatementNode> left,
                                                        std::shared_ptr<Token> op2,
                                                        std::shared_ptr<ASTExpressionNode> right,
                                                        std::shared_ptr<Token> op3,
                                                        std::shared_ptr<Token> op4)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_DECORATOR), m_Op1(op1), m_Left(left), m_Op2(op2), m_Right(right), m_Op3(op3), m_Op4(op4) { }

std::shared_ptr<Token> ASTDecoratorStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTStatementNode> ASTDecoratorStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTDecoratorStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTExpressionNode> ASTDecoratorStatementNode::getRight() { return m_Right; }

std::shared_ptr<Token> ASTDecoratorStatementNode::getOperatorThree() { return m_Op3; }

std::shared_ptr<Token> ASTDecoratorStatementNode::getOperatorFour() { return m_Op4; }


// ASTStatementListNode ///////////////////////////////////////////////////////////////////////////////////////////////
ASTStatementListNode::ASTStatementListNode(unsigned int start, unsigned int end, ASTNode::NodeKind kind)
    :   ASTStatementNode(start, end, kind) { }

void ASTStatementListNode::addNode(std::shared_ptr<ASTStatementNode> node) { m_Nodes->push_back(node); }

void ASTStatementListNode::addSeparator(std::shared_ptr<Token> op) { m_Ops->push_back(op); }

void ASTStatementListNode::addEndPosition(unsigned int end) {  m_EndPos = end; }

std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> ASTStatementListNode::getNodes() { return m_Nodes; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTStatementListNode::getSeparators() { return m_Ops; }

unsigned int ASTStatementListNode::count() { return m_Nodes->size(); }


// ASTDecoratedStatementNode //////////////////////////////////////////////////////////////////////////////////////////
ASTDecoratedStatementNode::ASTDecoratedStatementNode(unsigned int start, unsigned int end, std::shared_ptr<ASTStatementNode> left, std::shared_ptr<ASTStatementNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_DECORATED), m_Left(left), m_Right(right) { }

std::shared_ptr<ASTStatementNode> ASTDecoratedStatementNode::getLeft() { return m_Left; }

std::shared_ptr<ASTStatementNode> ASTDecoratedStatementNode::getRight() { return m_Right; }


// ASTAsyncFuncDefStatementNode ///////////////////////////////////////////////////////////////////////////////////////
ASTAsyncFuncDefStatementNode::ASTAsyncFuncDefStatementNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op, std::shared_ptr<ASTStatementNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_DECORATED), m_Op1(op), m_Right(right) { }

std::shared_ptr<Token> ASTAsyncFuncDefStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTStatementNode> ASTAsyncFuncDefStatementNode::getRight() { return m_Right; }


// ASTFuncDefStatementNode ////////////////////////////////////////////////////////////////////////////////////////////
ASTFuncDefStatementNode::ASTFuncDefStatementNode(   unsigned int start,
                                                    unsigned int end,
                                                    std::shared_ptr<Token> op1,
                                                    std::shared_ptr<Token> op2,
                                                    std::shared_ptr<ASTStatementNode> left,
                                                    std::shared_ptr<Token> op3,
                                                    std::shared_ptr<ASTExpressionNode> right,
                                                    std::shared_ptr<Token> op4,
                                                    std::shared_ptr<ASTExpressionNode> comment,
                                                    std::shared_ptr<ASTStatementNode> next)
    : ASTStatementNode(start, end, ASTNode::NodeKind::NK_FUNCDEF), m_Op1(op1), m_Op2(op2), m_Left(left), m_Op3(op3), m_Op4(op4), m_TypeComment(comment), m_Next(next) { }

std::shared_ptr<Token> ASTFuncDefStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTFuncDefStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTStatementNode> ASTFuncDefStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTFuncDefStatementNode::getOperatorThree() { return m_Op3; }

std::shared_ptr<ASTExpressionNode> ASTFuncDefStatementNode::getRight() { return m_Right; }

std::shared_ptr<Token> ASTFuncDefStatementNode::getOperatorFour() { return m_Op4; }

std::shared_ptr<ASTExpressionNode> ASTFuncDefStatementNode::getTypeComment() { return m_TypeComment; }

std::shared_ptr<ASTStatementNode> ASTFuncDefStatementNode::getNext() { return m_Next; }


// ASTParameterStatementNode //////////////////////////////////////////////////////////////////////////////////////////
ASTParameterStatementNode::ASTParameterStatementNode(   unsigned int start, 
                                                        unsigned int end, 
                                                        std::shared_ptr<Token> op1, 
                                                        std::shared_ptr<ASTStatementNode> rigth, 
                                                        std::shared_ptr<Token> op2)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_PARAMETERS), m_Op1(op1), m_Right(rigth), m_Op2(op2) { }

std::shared_ptr<Token> ASTParameterStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTStatementNode> ASTParameterStatementNode::getRight() { return m_Right; }

std::shared_ptr<Token> ASTParameterStatementNode::getOperatorTwo() { return m_Op2; }


// ASTListSimpleStatementNode /////////////////////////////////////////////////////////////////////////////////////////
ASTListSimpleStatementNode::ASTListSimpleStatementNode( unsigned int start, 
                                                        unsigned int end,
                                                        std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes,
                                                        std::shared_ptr<std::vector<std::shared_ptr<Token>>> separators,
                                                        std::shared_ptr<Token> op)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_SIMPLE_STMT_LIST), m_Nodes(nodes), m_Separators(separators), m_Op1(op) { }

std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> ASTListSimpleStatementNode::getNodes() { return m_Nodes; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTListSimpleStatementNode::getSeparators() { return m_Separators; }

std::shared_ptr<Token> ASTListSimpleStatementNode::getOperatorOne() { return m_Op1; }

unsigned int ASTListSimpleStatementNode::count() { return m_Nodes->size(); }


// ASTSimpleFlowStatementNode /////////////////////////////////////////////////////////////////////////////////////////
ASTSimpleFlowStatementNode::ASTSimpleFlowStatementNode(unsigned int start, unsigned int end, ASTNode::NodeKind kind, std::shared_ptr<Token> op)
    :   ASTStatementNode(start, end, kind), m_Op1(op) { }

std::shared_ptr<Token> ASTSimpleFlowStatementNode::getOperatorOne() { return m_Op1; }


// ASTReturnStatementNode /////////////////////////////////////////////////////////////////////////////////////////////
ASTReturnStatementNode::ASTReturnStatementNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op, std::shared_ptr<ASTStatementNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_RETURN), m_Op1(op), m_Right(right) { }

std::shared_ptr<Token> ASTReturnStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTStatementNode> ASTReturnStatementNode::getRigth() { return m_Right; }


// ASTYieldStatementNode //////////////////////////////////////////////////////////////////////////////////////////////
ASTYieldStatementNode::ASTYieldStatementNode(unsigned int start, unsigned int end, std::shared_ptr<ASTExpressionNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_YIELD_STMT), m_Right(right) { }

std::shared_ptr<ASTExpressionNode> ASTYieldStatementNode::getRight() { return m_Right; }


// ASTRaiseStatementNode //////////////////////////////////////////////////////////////////////////////////////////////
ASTRaiseStatementNode::ASTRaiseStatementNode(   unsigned int start, 
                                                unsigned int end,
                                                std::shared_ptr<Token> op1,
                                                std::shared_ptr<ASTExpressionNode> left,
                                                std::shared_ptr<Token> op2,
                                                std::shared_ptr<ASTExpressionNode> right
                                            ) 
    : ASTStatementNode(start, end, ASTNode::NodeKind::NK_RAISE), m_Op1(op1), m_Left(left), m_Op2(op2), m_Right(right) { }

std::shared_ptr<Token> ASTRaiseStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTRaiseStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTRaiseStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTExpressionNode> ASTRaiseStatementNode::getRight() { return m_Right; }


// ASTGlobalNonLocalStatementNode /////////////////////////////////////////////////////////////////////////////////////
ASTGlobalNonLocalStatementNode::ASTGlobalNonLocalStatementNode( unsigned int start,
                                                                unsigned int end,
                                                                ASTNode::NodeKind kind,
                                                                std::shared_ptr<Token> op1,
                                                                std::shared_ptr<std::vector<std::shared_ptr<Token>>> nodes,
                                                                std::shared_ptr<std::vector<std::shared_ptr<Token>>> separators
                                                                )
    :   ASTStatementNode(start, end, kind), m_Op1(op1), m_Nodes(nodes), m_Separators(separators) { }

unsigned int ASTGlobalNonLocalStatementNode::count() { return m_Nodes->size(); }

std::shared_ptr<Token> ASTGlobalNonLocalStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTGlobalNonLocalStatementNode::getNodes() { return m_Nodes; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTGlobalNonLocalStatementNode::getSeparators() { return m_Separators; }


// ASTAssertStatementNode /////////////////////////////////////////////////////////////////////////////////////////////
ASTAssertStatementNode::ASTAssertStatementNode( unsigned int start, 
                                                unsigned int end, 
                                                std::shared_ptr<Token> op1, 
                                                std::shared_ptr<ASTExpressionNode> left, 
                                                std::shared_ptr<Token> op2, 
                                                std::shared_ptr<ASTExpressionNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_ASSERT_STMT), m_Op1(op1), m_Left(left), m_Op2(op2), m_Right(right) { }

std::shared_ptr<Token> ASTAssertStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTAssertStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTAssertStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTExpressionNode> ASTAssertStatementNode::getRight() { return m_Right; }


// ASTDelStatementNode ////////////////////////////////////////////////////////////////////////////////////////////////
ASTDelStatementNode::ASTDelStatementNode(   unsigned int start, 
                                            unsigned int end, 
                                            std::shared_ptr<Token> op, 
                                            std::shared_ptr<ASTExpressionNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_DEL), m_Op1(op), m_Right(right) { }

std::shared_ptr<Token> ASTDelStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTDelStatementNode::getRight() { return m_Right; }


// ASTPassStatementNode ///////////////////////////////////////////////////////////////////////////////////////////////
ASTPassStatementNode::ASTPassStatementNode( unsigned int start, 
                                            unsigned int end, 
                                            std::shared_ptr<Token> op)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_DEL), m_Op1(op) { }

std::shared_ptr<Token> ASTPassStatementNode::getOperatorOne() { return m_Op1; }


// ASTSingleInputNode /////////////////////////////////////////////////////////////////////////////////////////////////
ASTSingleInputNode::ASTSingleInputNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op, std::shared_ptr<ASTStatementNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_SINGLE_INPUT), m_Op1(op), m_Right(right) { }

std::shared_ptr<Token> ASTSingleInputNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTStatementNode> ASTSingleInputNode::getRight() { return m_Right; }


// ASTFileInputNode ///////////////////////////////////////////////////////////////////////////////////////////////////
ASTFileInputNode::ASTFileInputNode( unsigned int start,
                                    unsigned int end,
                                    std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes,
                                    std::shared_ptr<std::vector<std::shared_ptr<Token>>> newlines,
                                    std::shared_ptr<Token> eof)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_FILE_INPUT), m_Newlines(newlines), m_Nodes(nodes), m_Eof(eof) { }

std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> ASTFileInputNode::getNodes() { return m_Nodes; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTFileInputNode::getNewlines() { return m_Newlines; }

std::shared_ptr<Token> ASTFileInputNode::getEOF() { return m_Eof; }


// ASTEvalInputNode ///////////////////////////////////////////////////////////////////////////////////////////////////
ASTEvalInputNode::ASTEvalInputNode(unsigned int start, unsigned int end, std::shared_ptr<ASTExpressionNode> right, std::shared_ptr<std::vector<std::shared_ptr<Token>>> newlines, std::shared_ptr<Token> eof)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_EVAL_INPUT), m_Newlines(newlines), m_Right(right), m_Eof(eof) { }

std::shared_ptr<ASTExpressionNode> ASTEvalInputNode::getRight() { return m_Right; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTEvalInputNode::getNewlines() { return m_Newlines; }

std::shared_ptr<Token> ASTEvalInputNode::getEOF() { return m_Eof; }


// ASTDottedNameStatementNode /////////////////////////////////////////////////////////////////////////////////////////
ASTDottedNameStatementNode::ASTDottedNameStatementNode( unsigned int start,
                                                        unsigned int end,
                                                        std::shared_ptr<std::vector<std::shared_ptr<Token>>> nodes,
                                                        std::shared_ptr<std::vector<std::shared_ptr<Token>>> dots)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_DOTTED_NAME), m_Nodes(nodes), m_Dots(dots) { }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTDottedNameStatementNode::getNodes() { return m_Nodes; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTDottedNameStatementNode::getDots() { return m_Dots; }

unsigned int ASTDottedNameStatementNode::count() { return m_Nodes->size(); }


// ASTImportContainerStatementNode ////////////////////////////////////////////////////////////////////////////////////
ASTImportContainerStatementNode::ASTImportContainerStatementNode(   unsigned int start,
                                                                    unsigned int end,
                                                                    ASTNode::NodeKind kind,
                                                                    std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes,
                                                                    std::shared_ptr<std::vector<std::shared_ptr<Token>>> commas)
    :   ASTStatementNode(start, end, kind), m_Nodes(nodes), m_Commas(commas) { }

std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> ASTImportContainerStatementNode::getNodes() { return m_Nodes; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTImportContainerStatementNode::getCommas() { return m_Commas; }

unsigned int ASTImportContainerStatementNode::count() { return m_Nodes->size(); }


// ASTDottedNameAsStatementNode ///////////////////////////////////////////////////////////////////////////////////////
ASTDottedNameAsStatementNode::ASTDottedNameAsStatementNode(unsigned int start, unsigned int end, std::shared_ptr<ASTStatementNode> left, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_DOTTED_AS_NAME), m_Left(left), m_Op1(op1), m_Op2(op2) { }

std::shared_ptr<ASTStatementNode> ASTDottedNameAsStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTDottedNameAsStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTDottedNameAsStatementNode::getOperatorTwo() { return m_Op2; }


// ASTImportAsNameStatementNode ///////////////////////////////////////////////////////////////////////////////////////
ASTImportAsNameStatementNode::ASTImportAsNameStatementNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2, std::shared_ptr<Token> op3)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_IMPORT_AS_NAME), m_Op1(op1), m_Op2(op2), m_Op3(op3) { }

std::shared_ptr<Token> ASTImportAsNameStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTImportAsNameStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<Token> ASTImportAsNameStatementNode::getOperatorThree() { return m_Op3; }


// ASTImportStatementNode /////////////////////////////////////////////////////////////////////////////////////////////
ASTImportStatementNode::ASTImportStatementNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<ASTStatementNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_IMPORT_NAME), m_Op1(op1), m_Right(right) { }

std::shared_ptr<Token> ASTImportStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTStatementNode> ASTImportStatementNode::getRight() { return m_Right; }


// ASTFromImportStatementNode /////////////////////////////////////////////////////////////////////////////////////////
ASTFromImportStatementNode::ASTFromImportStatementNode( unsigned int start,
                                                        unsigned int end,
                                                        std::shared_ptr<Token> op1,
                                                        std::shared_ptr<std::vector<std::shared_ptr<Token>>> dots,
                                                        std::shared_ptr<ASTStatementNode> left,
                                                        std::shared_ptr<Token> op2,
                                                        std::shared_ptr<Token> op3,
                                                        std::shared_ptr<ASTStatementNode> right,
                                                        std::shared_ptr<Token> op4)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_IMPORT_FROM), m_Op1(op1), m_Op2(op2), m_Op3(op3), m_Op4(op4), m_Dots(dots), m_Left(left), m_Right(right) { }

std::shared_ptr<Token> ASTFromImportStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTFromImportStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<Token> ASTFromImportStatementNode::getOperatorThree() { return m_Op3; }

std::shared_ptr<Token> ASTFromImportStatementNode::getOperatorFour() { return m_Op4; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTFromImportStatementNode::getDots() { return m_Dots; }

unsigned int ASTFromImportStatementNode::count() { return m_Dots->size(); }

std::shared_ptr<ASTStatementNode> ASTFromImportStatementNode::getLeft() { return m_Left; }

std::shared_ptr<ASTStatementNode> ASTFromImportStatementNode::getRight() { return m_Right; }


// ASTDefStatementNode ////////////////////////////////////////////////////////////////////////////////////////////////
ASTDefStatementNode::ASTDefStatementNode(unsigned int start, unsigned int end, ASTNode::NodeKind kind, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2, std::shared_ptr<ASTExpressionNode> right)
    :   ASTStatementNode(start, end, kind), m_Op1(op1), m_Op2(op2), m_Right(right) { }

std::shared_ptr<Token> ASTDefStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<Token> ASTDefStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<ASTExpressionNode> ASTDefStatementNode::getRight() { return m_Right; }


// ASTTestListCompStatementNod ////////////////////////////////////////////////////////////////////////////////////////
ASTTestListCompStatementNode::ASTTestListCompStatementNode(unsigned int start, unsigned int end, std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> nodes, std::shared_ptr<std::vector<std::shared_ptr<Token>>> commas)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_TEST_LIST_COMP), m_Nodes(nodes), m_Commas(commas) {}

std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> ASTTestListCompStatementNode::getNodes() { return m_Nodes; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTTestListCompStatementNode::getCommas() { return m_Commas; }


// ASTOperatorAssignmentStatementNode /////////////////////////////////////////////////////////////////////////////////
ASTOperatorAssignmentStatementNode::ASTOperatorAssignmentStatementNode(unsigned int start, unsigned int end, ASTNode::NodeKind kind, std::shared_ptr<ASTStatementNode> left, std::shared_ptr<Token> op, std::shared_ptr<ASTExpressionNode> right)
    :   ASTStatementNode(start, end, kind), m_Left(left), m_Op1(op), m_Right(right) { }

std::shared_ptr<ASTStatementNode> ASTOperatorAssignmentStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTOperatorAssignmentStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTOperatorAssignmentStatementNode::getRight() { return m_Right; }


// ASTAnnotationStatementNode /////////////////////////////////////////////////////////////////////////////////////////
ASTAnnotationStatementNode::ASTAnnotationStatementNode(unsigned int start, unsigned int end, std::shared_ptr<ASTStatementNode> left, std::shared_ptr<Token> op, std::shared_ptr<ASTExpressionNode> right)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_ANN_ASSIGN), m_Left(left), m_Op1(op), m_Right(right) { }

std::shared_ptr<ASTStatementNode> ASTAnnotationStatementNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTAnnotationStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTAnnotationStatementNode::getRight() { return m_Right; }


// ASTAssignStatementNode /////////////////////////////////////////////////////////////////////////////////////////////
ASTAssignStatementNode::ASTAssignStatementNode(unsigned int start, unsigned int end, std::shared_ptr<ASTStatementNode> left, std::shared_ptr<Token> op, std::shared_ptr<ASTStatementNode> right, std::shared_ptr<ASTExpressionNode> comment)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_ASSIGN), m_Left(left), m_Op1(op), m_Right(right), m_Comment(comment) { }

std::shared_ptr<ASTStatementNode> ASTAssignStatementNode::getLeft() { return m_Left;}

std::shared_ptr<Token> ASTAssignStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTStatementNode> ASTAssignStatementNode::getRight() { return m_Right; }

std::shared_ptr<ASTExpressionNode> ASTAssignStatementNode::getComment() { return m_Comment; }


// ASTFuncBodySuiteStatementNode //////////////////////////////////////////////////////////////////////////////////////
ASTFuncBodySuiteStatementNode::ASTFuncBodySuiteStatementNode(   unsigned int start,
                                                                unsigned int end,
                                                                std::shared_ptr<Token> op1,
                                                                std::shared_ptr<ASTExpressionNode> comment,
                                                                std::shared_ptr<Token> op2,
                                                                std::shared_ptr<Token> op3,
                                                                std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> nodes,
                                                                std::shared_ptr<Token> op4)
    :   ASTStatementNode(start, end, ASTNode::NodeKind::NK_FUNC_BODY_SUITE), m_Op1(op1), m_Comment(comment), m_Op2(op2), m_Op3(op3), m_Nodes(nodes), m_Op4(op4) { }

std::shared_ptr<Token> ASTFuncBodySuiteStatementNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTFuncBodySuiteStatementNode::getComment() { return m_Comment; }

std::shared_ptr<Token> ASTFuncBodySuiteStatementNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<Token> ASTFuncBodySuiteStatementNode::getOperatorThree() { return m_Op3; }

std::shared_ptr<std::vector<std::shared_ptr<ASTStatementNode>>> ASTFuncBodySuiteStatementNode::getNodes() { return m_Nodes; }

std::shared_ptr<Token> ASTFuncBodySuiteStatementNode::getOperatorFour() { return m_Op4; }

unsigned int ASTFuncBodySuiteStatementNode::count() { return m_Nodes->size(); }


// ASTFuncTypeInput ///////////////////////////////////////////////////////////////////////////////////////////////////
ASTFuncTypeInput::ASTFuncTypeInput( unsigned int start, 
                                    unsigned int end, 
                                    std::shared_ptr<ASTExpressionNode> right, 
                                    std::shared_ptr<std::vector<std::shared_ptr<Token>>> newlines, 
                                    std::shared_ptr<Token> eof)
    :   ASTExpressionNode(start, end, ASTNode::NodeKind::NK_FUNC_TYPE_INPUT), m_Right(right), m_Newlines(newlines), m_Eof(eof) { }

std::shared_ptr<ASTExpressionNode> ASTFuncTypeInput::getRight() { return m_Right; }

std::shared_ptr<std::vector<std::shared_ptr<Token>>> ASTFuncTypeInput::getNewlines() { return m_Newlines; }

std::shared_ptr<Token> ASTFuncTypeInput::getEof() { return m_Eof; }


// ASTFuncTypeExpressionNode //////////////////////////////////////////////////////////////////////////////////////////
ASTFuncTypeExpressionNode::ASTFuncTypeExpressionNode(   unsigned int start, 
                                                        unsigned int end, 
                                                        std::shared_ptr<Token> op1, 
                                                        std::shared_ptr<ASTExpressionNode> left, 
                                                        std::shared_ptr<Token> op2, 
                                                        std::shared_ptr<Token> op3, 
                                                        std::shared_ptr<ASTExpressionNode> right)
    :   ASTExpressionNode(start, end, ASTNode::NodeKind::NK_FUNC_TYPE), m_Op1(op1), m_Left(left), m_Op2(op2), m_Op3(op3), m_Right(right) { }

std::shared_ptr<Token> ASTFuncTypeExpressionNode::getOperatorOne() { return m_Op1; }

std::shared_ptr<ASTExpressionNode> ASTFuncTypeExpressionNode::getLeft() { return m_Left; }

std::shared_ptr<Token> ASTFuncTypeExpressionNode::getOperatorTwo() { return m_Op2; }

std::shared_ptr<Token> ASTFuncTypeExpressionNode::getOperatorThree() { return m_Op3; }

std::shared_ptr<ASTExpressionNode> ASTFuncTypeExpressionNode::getRight() { return m_Right; }


//
