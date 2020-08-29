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

std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseTest() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseTestNoCond() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseLambdaDef() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseLambdaDefNoCond() { return nullptr; }
std::shared_ptr<ASTExpressionNode> PythonCoreParser::parseOrTest() { return nullptr; }
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