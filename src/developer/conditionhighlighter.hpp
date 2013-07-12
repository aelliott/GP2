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
#ifndef CONDITIONHIGHLIGHTER_HPP
#define CONDITIONHIGHLIGHTER_HPP

#include <QSyntaxHighlighter>
#include "token.hpp"

namespace Developer {

class ConditionHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit ConditionHighlighter(QTextDocument *parent = 0);

    /*!
     * \brief Gets the format associated with the provided type
     * \param type  The type of the token whose format we are retrieving. This
     *  will be a value from the Lexemes enum
     * \return The QTextCharFormat associated with this lexeme
     */
    QTextCharFormat format(int type) const;

public slots:
    /*!
     * \brief Set the sequence of tokens which correspond with this program
     * \param tokens    The sequence of tokens
     */
    void setTokens(QVector<Token *> tokens);
    /*!
     * \brief A wrapper function which complies with the standard
     *  QSyntaxHighlighter interface
     * \param text  The text contents of the current block being highlighted
     */
    void highlightBlock(const QString &text);

private:
    QVector<Token *> _tokens;
};

}

#endif // CONDITIONHIGHLIGHTER_HPP
