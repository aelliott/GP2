/*!
 * \file
 */
#ifndef CONDITIONTOKENS_HPP
#define CONDITIONTOKENS_HPP

namespace Developer {

enum ConditionLexemes
{
    ConditionLexeme_Default,

    ConditionLexeme_Identifier,
    ConditionLexeme_Number,
    ConditionLexeme_QuotedString,
    ConditionLexeme_Keyword,
    ConditionLexeme_Operator,
    ConditionLexeme_OpenParen,
    ConditionLexeme_CloseParen,
    ConditionLexeme_CommentOpen,
    ConditionLexeme_CommentClose,
    ConditionLexeme_Comment,

    ConditionLexeme_Error
};

}

#endif // CONDITIONTOKENS_HPP
