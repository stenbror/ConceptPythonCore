
#ifndef PYTHONCORE_PARSER_H
#define PYTHONCORE_PARSER_H

#include <abstractSyntaxTree.h>
#include <pythonCoreTokenizer.h>

namespace PythonCore::Runtime
{
    class PythonCoreParser
    {
        public:

            std::shared_ptr<ASTExpressionNode> parseNamedExpr();
            std::shared_ptr<ASTExpressionNode> parseTest();
            std::shared_ptr<ASTExpressionNode> parseTestNoCond();
            std::shared_ptr<ASTExpressionNode> parseLambdaDef();
            std::shared_ptr<ASTExpressionNode> parseLambdaDefNoCond();
            std::shared_ptr<ASTExpressionNode> parseLambdaCommon(bool isConditional);
            std::shared_ptr<ASTExpressionNode> parseOrTest();
            std::shared_ptr<ASTExpressionNode> parseAndTest();
            std::shared_ptr<ASTExpressionNode> parseNotTest();
            std::shared_ptr<ASTExpressionNode> parseComparison();
            std::shared_ptr<ASTExpressionNode> parseStarExpr();
            std::shared_ptr<ASTExpressionNode> parseExpr();
            std::shared_ptr<ASTExpressionNode> parseXorExpr();
            std::shared_ptr<ASTExpressionNode> parseAndExpr();
            std::shared_ptr<ASTExpressionNode> parseShiftExpr();
            std::shared_ptr<ASTExpressionNode> parseArithExpr();
            std::shared_ptr<ASTExpressionNode> parseTerm();
            std::shared_ptr<ASTExpressionNode> parseFactor();
            std::shared_ptr<ASTExpressionNode> parsePower();
            std::shared_ptr<ASTExpressionNode> parseAtomExpr();
            std::shared_ptr<ASTExpressionNode> parseAtom();

        protected:
            std::shared_ptr<Token> m_CurSymbol;
            std::shared_ptr<PythonCoreTokenizer> m_Lexer;
    };
} // namespace PythonCore::Runtime

#endif