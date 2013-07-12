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
#ifndef PROGRAMEDITOR_HPP
#define PROGRAMEDITOR_HPP

#include "codeeditor.hpp"

namespace Developer {

class ProgramHighlighter;

/*!
 * \brief The ProgramEditor class encapsulates the necessary classes to display
 *  and edit a GP program
 *
 * It handles parsing the program into a sequence of tokens and passes those
 * along to a ProgramHighlighter instance for syntax highlighting. Error tokens
 * display descriptions in tooltips when the user hovers their mouse over the
 * erroneous token.
 */
class ProgramEditor : public CodeEditor
{
    Q_OBJECT

public:
    ProgramEditor(QWidget *parent = 0);

    QRegExp pattern(int type) const;

public slots:
    /*!
     * \brief Parse the program being edited into a vector of tokens
     */
    void parse();

    ProgramHighlighter *highlighter() const;

protected:
    bool consumeComments();
    bool consumeWhitespace();
    void consumeError(const QString &expecting = QString());

    void parseDeclarations();
    void parseCommandSeqence();
    void parseCommand();
    void parseBlock();
    void parseRuleSet();
    void parseIf();
    void parseTry();

    /*!
     * \brief Produce a simple formatted text string when an invalid token is
     *  found
     *
     * This is just to keep the error messages looking consistent.
     *
     * \todo Convert position into line:column
     *
     * \param tokenFound    The unexpected token which was found
     * \param position      The position of the token
     * \return A formatted QString with the provided details
     */
    QString errorString(const QString &tokenFound, int position);

    /*!
     * \brief Catch mouse movement over the editor in order to overlay error
     *  descriptions in tooltips
     * \param e The mouse move event which has been captured
     */
    void mouseMoveEvent(QMouseEvent *e);

private:
    ProgramHighlighter *_highlighter;
    QString _program;
    int _pos;
    int _scopeDepth;
    QString _cache;
};

}

#endif // PROGRAMEDITOR_HPP
