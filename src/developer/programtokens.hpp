/*!
 * \file
 */
#ifndef PROGRAMTOKENS_HPP
#define PROGRAMTOKENS_HPP

#include <QString>

namespace Developer {

/*!
 * \brief The Lexemes enum defines all of the types of token that the parser
 *  uses
 *
 * Some of them are internal such as CommentOpen and CommentClose which
 * relate to distinct regular expressions but once the parser has matched
 * those then the whole comment is marked using the Comment enumerator
 * value.
 */
enum Lexemes
{
    //! No specfic token (i.e. whitespace)
    Default,
    //! An identifier for a rule, macro, node or edge
    Identifier,
    //! A macro or main declaration (identifier = ...)
    Declaration,
    //! The declaration operator (=)
    DeclarationOperator,
    //! The declaration separator (.)
    DeclarationSeparator,
    //! A keyword such as "main", "skip" or "else"
    Keyword,
    //! A comment
    Comment,
    //! (Internal) The opening of a comment
    CommentOpen,
    //! (Internal) The closing of a comment
    CommentClose,
    //! An opening parenthesis
    OpenParen,
    //! A closing parenthesis
    CloseParen,
    //! An opening curly brace
    OpenBrace,
    //! A closing curly brace
    CloseBrace,
    //! The "repeat as long as you can" operator: !
    Repeat,
    //! The statement separator
    StatementSeparator,
    //! Rule separator within a rule set
    RuleSeparator,
    //! An error such as an unmatched brace or parenthesis
    Error
};

}

#endif // PROGRAMTOKENS_HPP
