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

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseAndTest() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseNotTest() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseComparison() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseStarExpr() { return nullptr; }
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