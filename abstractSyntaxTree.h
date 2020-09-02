
#ifndef PYTHONCORE_ABSTRACT_SYNTAX_TREE_H
#define PYTHONCORE_ABSTRACT_SYNTAX_TREE_H

#include <memory>
#include <vector>

#include <pythonCoreTokenizer.h>

namespace PythonCore::Runtime
{

    class ASTNode
    {
        public:
            enum class NodeKind
            {
                NK_INVALID, NK_NAMED_EXPR, NK_TEST, NK_LAMBDA, NK_OR_TEST, NK_AND_TEST, NK_NOT_TEST, NK_LESS, NK_LESS_EQUAL,
                NK_EQUAL, NK_GREATER_EQUAL, NK_GREATER, NK_NOT_EQUAL, NK_IN, NK_NOT_IN, NK_IS, NK_IS_NOT, NK_STAR_EXPR,
                NK_OR, NK_XOR, NK_AND, NK_SHIFT_LEFT, NK_SHIFT_RIGHT, NK_PLUS, NK_MINUS, NK_MUL, NK_DIV, NK_FLOOR_DIV, NK_MODULO,
                NK_MATRICE, NK_POWER, NK_ATOM_EXPR, NK_NAME, NK_NUMBER, NK_STRING, NK_LIST, NK_TUPLE, NK_DICTIONARY, NK_SET,
                NK_TRUE, NK_FALSE, NK_NONE, NK_ELIPSIS, NK_TEST_LIST_COMP, NK_TRAILER, NK_SUBSCRIPTION_LIST, NK_SUBSCRIPTION,
                NK_EXPR_LIST, NK_TEST_LIST, NK_SYNC_COMP_FOR, NK_COMP_FOR, NK_COMP_IF, NK_YIELD, NK_YIELD_FROM, NK_BIT_INVERT,
                NK_ASYNC_STMT, NK_IF_STMT, NK_ELIF_STMT, NK_ELSE_STMT, NK_WHILE_STMT, NK_FOR_STMT, NK_TRY_STMT, NK_EXCEPT_STMT,
                NK_FINALLY_STMT, NK_WITH_STMT, NK_WITH_ITEM, NK_SUITE, NK_PLUS_ASSIGN, NK_MINUS_ASSIGN, NK_MUL_ASSIGN,
                NK_DIV_ASSIGN, NK_FLOOR_DIV_ASSIGN, NK_POWER_ASSIGN, NK_MATRICE_ASSIGN, NK_MODULO_ASSIGN, NK_SHIFT_LEFT_ASSIGN,
                NK_SHIFT_RIGHT_ASSIGN, NK_BIT_AND_ASSIGN, NK_BIT_XOR_ASSIGN, NK_BIT_OR_ASSIGN, NK_ASSIGN, NK_ANN_ASSIGN,
                NK_SIMPLE_STMT_LIST, NK_DEL, NK_PASS, NK_BREAK, NK_CONTINUE, NK_RETURN, NK_RAISE, NK_YIELD_STMT,
                NK_IMPORT_NAME, NK_IMPORT_FROM, NK_DOTTED_NAME, NK_DOTTED_AS_NAME, NK_DOTTED_AS_NAMES, NK_IMPORT_AS_NAME,
                NK_IMPORT_AS_NAMES, NK_GLOBAL_STMT, NK_NONLOCAL_STMT, NK_ASSERT_STMT, NK_DECORATED, NK_DECORATOR, NK_DECORATORS,
                NK_ASYNC_FUNCDEF, NK_FUNCDEF, NK_PARAMETERS, NK_CLASS, NK_ARGLIST, NK_ARGUMENT, NK_TYPED_ARGS_LIST, NK_VAR_ARGS_LIST,
                NK_TFP_DEF, NK_VFP_DEF, NK_STMT_LIST, NK_SINGLE_INPUT, NK_FILE_INPUT, NK_EVAL_INPUT, NK_FUNC_BODY_SUITE,
                NK_FUNC_TYPE_INPUT, NK_FUNC_TYPE, NK_TYPE_LIST, NK_STRING_LIST, NK_DOT_NAME, NK_CALL, NK_INDEN, NK_LIST_ARGUMENT,
                NK_KW_ARGUMENT, NK_COMP_ARGUMENT, NK_COLON_ASSIGN_ARGUMENT, NK_ASSIGN_ARGUMENT, NK_KV_LIST, NK_SET_LIST
            };

        public:
            ASTNode(unsigned int start, unsigned int end, NodeKind kind);
            unsigned int getStartPosition(); 
            unsigned int getEndPosition(); 
            NodeKind kind(); 
            bool isKind(NodeKind kind); 
            bool isExpressionNode();
            bool isStatementNode();
                
        protected:
            unsigned int m_StartPos;
            unsigned int m_EndPos;
            NodeKind m_Kind;
    };

    class ASTExpressionNode : public ASTNode
    {
        public:
            ASTExpressionNode(unsigned int start, unsigned int end, NodeKind kind) : ASTNode(start, end, kind) {};
        
    };

    class ASTStatementNode : public ASTNode
    {
        public:
            ASTStatementNode(unsigned int start, unsigned int end, NodeKind kind) : ASTNode(start, end, kind) {};
    };


    class ASTUnaryExpressionNode : public ASTExpressionNode
    {
        public:
            ASTUnaryExpressionNode( unsigned int start, 
                                    unsigned int end, 
                                    NodeKind kind,
                                    std::shared_ptr<Token> op1,
                                    std::shared_ptr<ASTExpressionNode> right
                                );
                                
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getRightNode();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<ASTExpressionNode> m_Right;
    };

    class ASTBinaryExpressionNode : public ASTExpressionNode
    {
        public:
            ASTBinaryExpressionNode( unsigned int start, 
                                    unsigned int end, 
                                    NodeKind kind,
                                    std::shared_ptr<ASTExpressionNode> left,
                                    std::shared_ptr<Token> op1,
                                    std::shared_ptr<ASTExpressionNode> right
                                );

            std::shared_ptr<ASTExpressionNode> getLeftNode();
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getRightNode();

        protected:
            std::shared_ptr<ASTExpressionNode> m_Left;
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<ASTExpressionNode> m_Right;
    };

    class ASTComparisonExpressionNode : public ASTExpressionNode
    {
        public:
            ASTComparisonExpressionNode(    unsigned int start, 
                                            unsigned int end, 
                                            NodeKind kind,
                                            std::shared_ptr<ASTExpressionNode> left,
                                            std::shared_ptr<Token> op1,
                                            std::shared_ptr<Token> op2,
                                            std::shared_ptr<ASTExpressionNode> right
                                        );

            std::shared_ptr<ASTExpressionNode> getLeftNode();
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<Token> getOperatorTwo();
            std::shared_ptr<ASTExpressionNode> getRightNode();

        protected:
            std::shared_ptr<ASTExpressionNode> m_Left;
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<Token> m_Op2;
            std::shared_ptr<ASTExpressionNode> m_Right;
    };

    class ASTTestExpressionNode : public ASTExpressionNode
    {
        public:
            ASTTestExpressionNode(  unsigned int start, 
                                    unsigned int end, 
                                    std::shared_ptr<ASTExpressionNode> left,
                                    std::shared_ptr<Token> op1,
                                    std::shared_ptr<ASTExpressionNode> right,
                                    std::shared_ptr<Token> op2,
                                    std::shared_ptr<ASTExpressionNode> next
                                );

            std::shared_ptr<ASTExpressionNode> getLeftNode();
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getRightNode();
            std::shared_ptr<Token> getOperatorTwo();
            std::shared_ptr<ASTExpressionNode> getNextNode();

        protected:
            std::shared_ptr<ASTExpressionNode> m_Left;
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<ASTExpressionNode> m_Right;
            std::shared_ptr<Token> m_Op2;
            std::shared_ptr<ASTExpressionNode> m_Next;
    };

    class ASTLambdaExpressionNode : public ASTExpressionNode
    {
        public:
            ASTLambdaExpressionNode(    unsigned int start, 
                                        unsigned int end, 
                                        std::shared_ptr<Token> op1,
                                        std::shared_ptr<ASTExpressionNode> left,
                                        std::shared_ptr<Token> op2,
                                        std::shared_ptr<ASTExpressionNode> right
                                    );  

            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getLeftNode();
            std::shared_ptr<Token> getOperatorTwo();
            std::shared_ptr<ASTExpressionNode> getRightNode();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<ASTExpressionNode> m_Left;
            std::shared_ptr<Token> m_Op2;
            std::shared_ptr<ASTExpressionNode> m_Right;
    };

    class ASTAtomExpressionNode : public ASTExpressionNode
    {
        public:
            ASTAtomExpressionNode(  unsigned int start, 
                                    unsigned int end, 
                                    std::shared_ptr<Token> op1,
                                    std::shared_ptr<ASTExpressionNode> left,
                                    std::shared_ptr<ASTExpressionNode> right
                                 );

            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getLeftNode();
            std::shared_ptr<ASTExpressionNode> getRightNode();
            bool isAwait();

        protected:
            std::shared_ptr<Token> m_Op1; // 'await'
            std::shared_ptr<ASTExpressionNode> m_Left; // Atom
            std::shared_ptr<ASTExpressionNode> m_Right; // Trailers
    };

    class ASTLiteralExpressionNode : public ASTExpressionNode
    {
        public:
            ASTLiteralExpressionNode(   unsigned int start, 
                                        unsigned int end,
                                        NodeKind kind,
                                        std::shared_ptr<Token> op1
                                    );

            std::shared_ptr<Token> getOperatorOne();
            bool isNameLiteral();
            bool isNumberLiteral();
            bool isStringLiteral();

        protected:
            std::shared_ptr<Token> m_Op1;
    };

    class ASTStringListExpressionNode : public ASTExpressionNode
    {
        public:
            ASTStringListExpressionNode(unsigned int start, unsigned int end);

            void addStringNode(std::shared_ptr<Token> node);
            std::shared_ptr<std::vector<std::shared_ptr<Token>>> getStringNodes(); 
            unsigned int count();
            void setEndPosition(unsigned int end);

        protected:
            std::shared_ptr<std::vector<std::shared_ptr<Token>>> m_StringList;
    };

    class ASTSubscriptionExpressionNode : public ASTExpressionNode
    {
        public:
            ASTSubscriptionExpressionNode(  unsigned int start, 
                                            unsigned int end,
                                            std::shared_ptr<ASTExpressionNode> left,
                                            std::shared_ptr<Token> op1,
                                            std::shared_ptr<ASTExpressionNode> right,
                                            std::shared_ptr<Token> op2,
                                            std::shared_ptr<ASTExpressionNode> next
                                            );

            std::shared_ptr<ASTExpressionNode> getLeft();
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getRight();
            std::shared_ptr<Token> getOperatorTwo();
            std::shared_ptr<ASTExpressionNode> getNext();

        protected:
            std::shared_ptr<ASTExpressionNode> m_Left;
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<ASTExpressionNode> m_Right;
            std::shared_ptr<Token> m_Op2;
            std::shared_ptr<ASTExpressionNode> m_Next;
    };

    class ASTListExpressionNode : public ASTExpressionNode
    {
        public:
            ASTListExpressionNode(unsigned int start, unsigned int end, ASTNode::NodeKind kind);
            void addNodes(std::shared_ptr<ASTExpressionNode> node, std::shared_ptr<Token> comma);
            void addEndPosition(unsigned int end);
            unsigned int count();
            std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> getNodes();
            std::shared_ptr<std::vector<std::shared_ptr<Token>>> getCommaNodes();

        protected:
            std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> m_Nodes;
            std::shared_ptr<std::vector<std::shared_ptr<Token>>> m_CommaNodes;
    };

    class ASTIfComprehensionExpressionNode : public ASTExpressionNode
    {
        public:
            ASTIfComprehensionExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op, std::shared_ptr<ASTExpressionNode> right, std::shared_ptr<ASTExpressionNode> next);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getRight();
            std::shared_ptr<ASTExpressionNode> getNext();

        protected:
            std::shared_ptr<Token> m_Op;
            std::shared_ptr<ASTExpressionNode> m_Right;
            std::shared_ptr<ASTExpressionNode> m_Next;
    };

    class ASTYieldFromExpressionNode : public ASTExpressionNode
    {
        public:
            ASTYieldFromExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2, std::shared_ptr<ASTExpressionNode> right);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<Token> getOperatorTwo();
            std::shared_ptr<ASTExpressionNode> getRight();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<Token> m_Op2;
            std::shared_ptr<ASTExpressionNode> m_Right;
    };

    class ASTYieldExpression : public ASTExpressionNode
    {
        public:
            ASTYieldExpression(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<ASTStatementNode> right);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTStatementNode> getRight();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<ASTStatementNode> m_Right;
    };

    class ASTSyncCompForExpressionNode : public ASTExpressionNode
    {
        public:
            ASTSyncCompForExpressionNode(   unsigned int start, 
                                            unsigned int end, 
                                            std::shared_ptr<Token> op1, 
                                            std::shared_ptr<ASTExpressionNode> left, 
                                            std::shared_ptr<Token> op2, 
                                            std::shared_ptr<ASTExpressionNode> right, 
                                            std::shared_ptr<ASTExpressionNode> next
                                        );
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getLeft();
            std::shared_ptr<Token> getOperatorTwo();
            std::shared_ptr<ASTExpressionNode> getRight();
            std::shared_ptr<ASTExpressionNode> getNext();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<ASTExpressionNode> m_Left;
            std::shared_ptr<Token> m_Op2;
            std::shared_ptr<ASTExpressionNode> m_Right;
            std::shared_ptr<ASTExpressionNode> m_Next;

    };

    class ASTTrailerListExpressionNode : public ASTExpressionNode
    {
        public:
            ASTTrailerListExpressionNode(unsigned int start, unsigned int end);
            void addTrailerNode(std::shared_ptr<ASTExpressionNode> node);
            unsigned int count();
            std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> getNodes();
            void addEndPosition(unsigned int end);

        protected:
            std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> m_Nodes;
    };

    class ASTDotNameExpressionNode : public ASTExpressionNode
    {
        public:
            ASTDotNameExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<Token> getOperatorTwo();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<Token> m_Op2;

    };

    class ASTCallExpressionNode : public ASTExpressionNode
    {
        public:
            ASTCallExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<ASTExpressionNode> right, std::shared_ptr<Token> op2);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getRight();
            std::shared_ptr<Token> getOperatorTwo();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<ASTExpressionNode> m_Right;
            std::shared_ptr<Token> m_Op2;

    };

    class ASTIndexExpressionNode : public ASTExpressionNode
    {
        public:
            ASTIndexExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<ASTExpressionNode> right, std::shared_ptr<Token> op2);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getRight();
            std::shared_ptr<Token> getOperatorTwo();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<ASTExpressionNode> m_Right;
            std::shared_ptr<Token> m_Op2;

    };

    class ASTListArgumentExpressionNode : public ASTExpressionNode 
    {
        public:
            ASTListArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<Token> getOperatorTwo();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<Token> m_Op2;
    };

    class ASTKWArgumentExpressionNode : public ASTExpressionNode 
    {
        public:
            ASTKWArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<Token> getOperatorTwo();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<Token> m_Op2;
    };

    class ASTCompArgumentExpressionNode : public ASTExpressionNode 
    {
        public:
            ASTCompArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<ASTExpressionNode> right);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getRight();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<ASTExpressionNode> m_Right;
    };

    class ASTColonAssignArgumentExpressionNode : public ASTExpressionNode 
    {
        public:
            ASTColonAssignArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2, std::shared_ptr<Token> op3);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<Token> getOperatorTwo();
            std::shared_ptr<Token> getOperatorThree();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<Token> m_Op2;
            std::shared_ptr<Token> m_Op3;
    };

    class ASTAssignArgumentExpressionNode : public ASTExpressionNode 
    {
        public:
            ASTAssignArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1, std::shared_ptr<Token> op2, std::shared_ptr<Token> op3);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<Token> getOperatorTwo();
            std::shared_ptr<Token> getOperatorThree();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<Token> m_Op2;
            std::shared_ptr<Token> m_Op3;
    };

    class ASTArgumentExpressionNode : public ASTExpressionNode 
    {
        public:
            ASTArgumentExpressionNode(unsigned int start, unsigned int end, std::shared_ptr<Token> op1);
            std::shared_ptr<Token> getOperatorOne();
        
        protected:
            std::shared_ptr<Token> m_Op1;
    };

    class ASTCompoundLiteralExpressionNode : public ASTExpressionNode
    {
        public:
            ASTCompoundLiteralExpressionNode(   unsigned int start,
                                                unsigned int end,
                                                ASTNode::NodeKind kind,
                                                std::shared_ptr<Token> op1,
                                                std::shared_ptr<ASTExpressionNode> right,
                                                std::shared_ptr<Token> op2);
            std::shared_ptr<Token> getOperatorOne();
            std::shared_ptr<ASTExpressionNode> getRight();
            std::shared_ptr<Token> getOperatorTwo();

        protected:
            std::shared_ptr<Token> m_Op1;
            std::shared_ptr<ASTExpressionNode> m_Right;
            std::shared_ptr<Token> m_Op2;
    };

    class ASTDictionarySetDataExpressionNode : public ASTExpressionNode
    {
        public:
            ASTDictionarySetDataExpressionNode(unsigned int start, unsigned int end, ASTNode::NodeKind kind);
            void addEndPosition(unsigned int end);
            unsigned int count();
            std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> getKeys();
            std::shared_ptr<std::vector<std::shared_ptr<Token>>> getColons();
            std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> getValues();
            std::shared_ptr<std::vector<std::shared_ptr<Token>>> getCommas();

        protected:
            std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> m_Keys;
            std::shared_ptr<std::vector<std::shared_ptr<Token>>> m_Colons;
            std::shared_ptr<std::vector<std::shared_ptr<ASTExpressionNode>>> m_Values;
            std::shared_ptr<std::vector<std::shared_ptr<Token>>> m_Commas;
    };


}

#endif