/*!
 * \file
 * \author Alex Elliott
 *
 * \section LICENSE
 * This file is part of GP Developer.
 *
 * GP Developer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * GP Developer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GP Developer.  If not, see <http://www.gnu.org/licenses/>.
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
enum ProgramLexemes
{
    //! No specfic token (i.e. whitespace)
    ProgramLexeme_Default,
    //! An identifier for a rule, macro, node or edge
    ProgramLexeme_Identifier,
    //! A macro or main declaration (identifier = ...)
    ProgramLexeme_Declaration,
    //! The declaration operator (=)
    ProgramLexeme_DeclarationOperator,
    //! The declaration separator (.)
    ProgramLexeme_DeclarationSeparator,
    //! A keyword such as "main", "skip" or "else"
    ProgramLexeme_Keyword,
    //! A comment
    ProgramLexeme_Comment,
    //! (Internal) The opening of a comment
    ProgramLexeme_CommentOpen,
    //! (Internal) The closing of a comment
    ProgramLexeme_CommentClose,
    //! An opening parenthesis
    ProgramLexeme_OpenParen,
    //! A closing parenthesis
    ProgramLexeme_CloseParen,
    //! An opening curly brace
    ProgramLexeme_OpenBrace,
    //! A closing curly brace
    ProgramLexeme_CloseBrace,
    //! The "repeat as long as you can" operator: !
    ProgramLexeme_Repeat,
    //! The statement separator
    ProgramLexeme_StatementSeparator,
    //! Rule separator within a rule set
    ProgramLexeme_RuleSeparator,
    //! An error such as an unmatched brace or parenthesis
    ProgramLexeme_Error
};

}

#endif // PROGRAMTOKENS_HPP
