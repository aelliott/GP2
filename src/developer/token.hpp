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
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <QString>

namespace Developer {

/*!
 * \brief The Token struct describes a match of a token to a particular
 *  substring within the document
 */
struct Token
{
    //! The start position of this token
    int startPos;
    //! The end position of this token
    int endPos;
    //! The type of token this instance represents
    int lexeme;
    //! The actual matched text
    QString text;
    //!  \brief A description of this token, this is primarily used to contain
    //! error messages for Error type tokens
    QString description;
};

}

#endif // TOKEN_HPP
