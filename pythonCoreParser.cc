#include <pythonCoreParser.h>

using namespace PythonCore::Runtime;

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseNamedExpr() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_COLON_ASSIGN)
    {
        auto op = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseTest();
        return std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_NAMED_EXPR, left, op, right);
    }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseTest() 
{   
    if (m_CurSymbol->kind() == Token::TokenKind::PY_LAMBDA) return parseLambdaCommon(true);
    unsigned int start = m_Lexer->getPosition();
    auto left = parseOrTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_IF)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseOrTest();
        if (m_CurSymbol->kind() != Token::TokenKind::PY_ELSE) throw ;
        auto op2 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto next = parseTest();
        return std::make_shared<ASTTestExpressionNode>(start, m_Lexer->getPosition(), left, op1, right, op2, next);
    }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseTestNoCond() 
{ 
    return m_CurSymbol->kind() == Token::TokenKind::PY_LAMBDA ? parseLambdaCommon(false) : parseOrTest();
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseLambdaCommon(bool isConditional)
{
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_CurSymbol = m_Lexer->advance();
    auto left = m_CurSymbol->kind() == Token::TokenKind::PY_COLON ? nullptr : nullptr /* VarArgsList */ ;
    if (m_CurSymbol->kind() != Token::TokenKind::PY_COLON) throw ;
    auto op2 = m_CurSymbol;
    m_CurSymbol = m_Lexer->advance();
    auto right = isConditional ? parseTest() : parseOrTest();
    return std::make_shared<ASTLambdaExpressionNode>(start, m_Lexer->getPosition(), op1, left, op2, right); 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseOrTest() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseAndTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_OR)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseAndTest();
        auto res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_OR_TEST, left, op1, right);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_OR)
        {
            op1 = m_CurSymbol;
            m_CurSymbol = m_Lexer->advance();
            right = parseAndTest();
            res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_OR_TEST, res, op1, right);
        }
        return res;
    }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseAndTest() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseNotTest();
    if (m_CurSymbol->kind() == Token::TokenKind::PY_AND)
    {
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseNotTest();
        auto res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_AND_TEST, left, op1, right);
        while (m_CurSymbol->kind() == Token::TokenKind::PY_AND)
        {
            op1 = m_CurSymbol;
            m_CurSymbol = m_Lexer->advance();
            right = parseNotTest();
            res = std::make_shared<ASTBinaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_AND_TEST, res, op1, right);
        }
        return res;
    }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseNotTest() 
{ 
    if (m_CurSymbol->kind() == Token::TokenKind::PY_NOT)
    {
        unsigned int start = m_Lexer->getPosition();
        auto op1 = m_CurSymbol;
        m_CurSymbol = m_Lexer->advance();
        auto right = parseNotTest();
        return std::make_shared<ASTUnaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_NOT_TEST, op1, right);
    }
    return parseComparison(); 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseComparison() 
{ 
    unsigned int start = m_Lexer->getPosition();
    auto left = parseExpr();
    auto symbol = m_CurSymbol->kind();
    if (symbol == Token::TokenKind::PY_LESS || symbol == Token::TokenKind::PY_LESS_EQUAL || symbol == Token::TokenKind::PY_EQUAL || symbol == Token::TokenKind::PY_GREATER_EQUAL ||
        symbol == Token::TokenKind::PY_GREATER || symbol == Token::TokenKind::PY_EQUAL || symbol == Token::TokenKind::PY_NOT_EQUAL || symbol == Token::TokenKind::PY_IS ||
        symbol == Token::TokenKind::PY_NOT || symbol == Token::TokenKind::PY_IN)
        {
            std::shared_ptr<ASTExpressionNode> res = left;
            std::shared_ptr<Token> op1;
            std::shared_ptr<Token> op2;
            auto kind = ASTNode::NodeKind::NK_INVALID;
            while ( symbol == Token::TokenKind::PY_LESS || symbol == Token::TokenKind::PY_LESS_EQUAL || symbol == Token::TokenKind::PY_EQUAL || symbol == Token::TokenKind::PY_GREATER_EQUAL ||
                    symbol == Token::TokenKind::PY_GREATER || symbol == Token::TokenKind::PY_EQUAL || symbol == Token::TokenKind::PY_NOT_EQUAL || symbol == Token::TokenKind::PY_IS ||
                    symbol == Token::TokenKind::PY_NOT || symbol == Token::TokenKind::PY_IN)
                    {
                        switch (symbol)
                        {
                            case Token::TokenKind::PY_LESS:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_LESS;
                                break;
                            case Token::TokenKind::PY_LESS_EQUAL:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_LESS_EQUAL;
                                break;
                            case Token::TokenKind::PY_EQUAL:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_EQUAL;
                                break;
                            case Token::TokenKind::PY_GREATER_EQUAL:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_GREATER_EQUAL;
                                break;
                            case Token::TokenKind::PY_GREATER:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_GREATER;
                                break;
                            case Token::TokenKind::PY_NOT_EQUAL:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_NOT_EQUAL;
                                break;
                            case Token::TokenKind::PY_IN:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_IN;
                                break;
                            case Token::TokenKind::PY_NOT:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                if (m_CurSymbol->kind() != Token::TokenKind::PY_IN) throw ;
                                op2 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                kind = ASTNode::NodeKind::NK_NOT_IN;
                                break;
                            case Token::TokenKind::PY_IS:
                                op1 = m_CurSymbol;
                                m_CurSymbol = m_Lexer->advance();
                                if (m_CurSymbol->kind() == Token::TokenKind::PY_NOT)
                                {
                                     op2 = m_CurSymbol;
                                    m_CurSymbol = m_Lexer->advance();
                                    kind = ASTNode::NodeKind::NK_IS_NOT;
                                }
                                else kind = ASTNode::NodeKind::NK_IS;
                                break;
                            default:
                                break;
                        }
                        auto right = parseExpr();
                        res = std::make_shared<ASTComparisonExpressionNode>(start, m_Lexer->getPosition(), kind, res, op1, op2, right );
                    }
            return res;
        }
    return left; 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseStarExpr() 
{   
    unsigned int start = m_Lexer->getPosition();
    auto op1 = m_CurSymbol;
    m_CurSymbol = m_Lexer->advance();
    auto right = parseExpr();
    return std::make_shared<ASTUnaryExpressionNode>(start, m_Lexer->getPosition(), ASTNode::NodeKind::NK_STAR_EXPR, op1, right); 
}

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseExpr() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseXorExpr() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseAndExpr() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseShiftExpr() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseArithExpr() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseTerm() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseFactor() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parsePower() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseAtomExpr() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseAtom() { return nullptr; }