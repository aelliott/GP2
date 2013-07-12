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
#ifndef CONDITIONTOKENS_HPP
#define CONDITIONTOKENS_HPP

namespace Developer {

enum ConditionLexemes
{
    ConditionLexeme_Default,

    //! "empty"
    Empty,
    //! List separator operator ":"
    ListSeparator,
    //! Comma separator ","
    Comma,
    //! String contained within quotes
    QuotedString,
    //! Opening parenthesis "("
    OpeningParen,
    //! Closing parenthesis ")"
    ClosingParen,
    //! Negation operator "-"
    Negation,
    //! "indeg" and "outdeg"
    DegreeTest,
    //! [0-9]+
    Integer,
    //! Addition operator "+"
    Plus,
    //! Subtraction operator "-"
    Minus,
    //! Multiplication operator "*"
    Times,
    //! Division operator "/"
    Divide,
    //! Less than operator "<"
    LessThan,
    //! Less than or equal to operator "<="
    LessThanEqualTo,
    //! Greater than operator ">"
    GreaterThan,
    //! Greater than or equal to operator ">="
    GreaterThanEqualTo,
    //! Equals operator "="
    Equals,
    //! Inequality operator "!="
    NotEquals,
    //! "not"
    Not,
    EdgeTest,
    And,
    Or,

    //! A GP variable [a-zA-Z_][a-zA-Z0-9_]{,62}
    Variable,
    //! An edge in the graph [a-zA-Z0-9_]+
    GraphLexeme,

    ConditionLexeme_CommentOpen,
    ConditionLexeme_Comment,
    ConditionLexeme_CommentClose,

    //! Unexpected token
    ConditionLexeme_Error
};

}

#endif // CONDITIONTOKENS_HPP
