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
#ifndef DOTPARSER_HPP
#define DOTPARSER_HPP

#include <QRegExp>
#include <QStringList>
#include <QTextDocument>
#include <QTextCursor>
#include "parsertypes.hpp"

namespace Developer {

class DotParser
{
public:
    enum DotLexemes
    {
        SingleLineComment,
        CommentOpen,
        CommentClose,
        Identifier,
        GraphOpen,
        GraphClose,
        EdgeOperator,
        AttributeListOpen,
        AttributeListClose,
        StatementSeparator,
        QuotedString,
        Number
    };

    DotParser(const QString &dotString = QString());
    ~DotParser();

    bool parse(const QString &dotString = QString());
    bool parseGraph();
    bool parseItem();
    QMap<QString,QVariant> parseAttributes();

    bool consumeWhitespace();
    bool consumeComments();
    void consumeError();

    graph_t toGraph() const;

    QRegExp pattern(int type) const;

private:
    QString _contents;
    int _pos;
    graph_t _graph;
    QStringList _nodes;
    QStringList _edges;
    int _idCounter;
    QTextDocument *_document;
    QTextCursor *_cursor;
};

}

#endif // DOTPARSER_HPP
