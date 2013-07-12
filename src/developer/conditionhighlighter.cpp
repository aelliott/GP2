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
#include "conditionhighlighter.hpp"
#include "conditiontokens.hpp"
#include "global.hpp"

#include <QSettings>
#include <QDebug>
#include <QVector>
#include <QTextDocument>

namespace Developer {

ConditionHighlighter::ConditionHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
}

QTextCharFormat ConditionHighlighter::format(int type) const
{
    QSettings settings;
    QColor background = settings.value(
                "Editor/BackgroundColor",
                QColor(Qt::white)
                ).value<QColor>();

    // Start a character format, the majority will be on the default background
    QTextCharFormat ret;
    ret.setBackground(background);

    QFont defaultFont = EDITOR_DEFAULT_FONT;
    QFont defaultCommentFont = defaultFont;
    defaultCommentFont.setItalic(true);

    switch(type)
    {
    case ConditionLexeme_Default:
        ret.setForeground(settings.value(
                              "Editor/Types/Default/Foreground",
                              QColor(Qt::black)
                              ).value<QColor>()
                          );
        ret.setBackground(settings.value(
                              "Editor/Types/Default/Background",
                              background
                              ).value<QColor>()
                          );
        ret.setFont(settings.value(
                        "Editor/Types/Default/Font",
                        defaultFont
                        ).value<QFont>()
                    );
        return ret;
    case Variable:
    case GraphLexeme:
        ret.setForeground(settings.value(
                              "Editor/Types/Identifier/Foreground",
                              QColor(Qt::darkBlue)
                              ).value<QColor>()
                          );
        ret.setBackground(settings.value(
                              "Editor/Types/Identifier/Background",
                              background
                              ).value<QColor>()
                          );
        ret.setFont(settings.value(
                        "Editor/Types/Identifier/Font",
                        defaultFont
                        ).value<QFont>()
                    );
        return ret;
    case Integer:
        ret.setForeground(settings.value(
                              "Editor/Types/Number/Foreground",
                              QColor(Qt::darkCyan)
                              ).value<QColor>()
                          );
        ret.setBackground(settings.value(
                              "Editor/Types/Number/Background",
                              background
                              ).value<QColor>()
                          );
        ret.setFont(settings.value(
                        "Editor/Types/Number/Font",
                        defaultFont
                        ).value<QFont>()
                    );
        return ret;
    case QuotedString:
        ret.setForeground(settings.value(
                              "Editor/Types/QuotedString/Foreground",
                              QColor(Qt::red)
                              ).value<QColor>()
                          );
        ret.setBackground(settings.value(
                              "Editor/Types/QuotedString/Background",
                              background
                              ).value<QColor>()
                          );
        ret.setFont(settings.value(
                        "Editor/Types/QuotedString/Font",
                        defaultFont
                        ).value<QFont>()
                    );
        return ret;
    case Comma:
    case ListSeparator:
    case OpeningParen:
    case ClosingParen:
    case Empty:
    case DegreeTest:
    case Negation:
    case Plus:
    case Minus:
    case Times:
    case Divide:
    case LessThan:
    case LessThanEqualTo:
    case GreaterThan:
    case GreaterThanEqualTo:
    case Equals:
    case NotEquals:
    case Not:
    case And:
    case Or:
    case EdgeTest:
        ret.setForeground(settings.value(
                              "Editor/Types/Keyword/Foreground",
                              QColor(Qt::darkYellow)
                              ).value<QColor>()
                          );
        ret.setBackground(settings.value(
                              "Editor/Types/Keyword/Background",
                              background
                              ).value<QColor>()
                          );
        ret.setFont(settings.value(
                        "Editor/Types/Keyword/Font",
                        defaultFont
                        ).value<QFont>()
                    );
        return ret;
    case ConditionLexeme_Comment:
    case ConditionLexeme_CommentOpen:
    case ConditionLexeme_CommentClose:
        ret.setForeground(settings.value(
                              "Editor/Types/Comment/Foreground",
                              QColor(Qt::darkCyan)
                              ).value<QColor>()
                          );
        ret.setBackground(settings.value(
                              "Editor/Types/Comment/Background",
                              background
                              ).value<QColor>()
                          );
        ret.setFont(settings.value(
                        "Editor/Types/Comment/Font",
                        defaultCommentFont
                        ).value<QFont>()
                    );
        return ret;
    case ConditionLexeme_Error:
        ret.setForeground(settings.value(
                              "Editor/Types/Error/Foreground",
                              QColor(Qt::darkRed)
                              ).value<QColor>()
                          );
        ret.setBackground(settings.value(
                              "Editor/Types/Error/Background",
                              QColor(0xff,0xcc,0xcc) // light red
                              ).value<QColor>()
                          );
        ret.setFont(settings.value(
                        "Editor/Types/Error/Font",
                        defaultFont
                        ).value<QFont>()
                    );
        return ret;
    default:
        qDebug() << "Unhandled type in ConditionHighlighter::format(): "
                 << type;
        return ret;
    }
}

void ConditionHighlighter::setTokens(QVector<Token *> tokens)
{
    _tokens = tokens;
}

void ConditionHighlighter::highlightBlock(const QString &text)
{
    if(text.isEmpty())
        return;
    int startPosition = currentBlock().position();
    int endPosition = startPosition + text.length();

    for(QVector<Token *>::iterator iter = _tokens.begin()
        ; iter != _tokens.end(); ++iter)
    {
        Token *t = *iter;
        // Is this entire block contained in this token?
        if(t->startPos <= startPosition && t->endPos >= endPosition)
        {
            setFormat(0, text.length(), format(t->lexeme));
            return;
        }

        // Is the start position of this token in range?
        if(t->startPos >= startPosition && t->startPos < endPosition)
        {
            if(t->endPos > endPosition)
                setFormat(t->startPos - startPosition,
                          text.length() - (t->startPos - startPosition),
                          format(t->lexeme));
            else
                setFormat(t->startPos - startPosition,
                          t->endPos - t->startPos,
                          format(t->lexeme));
        }
        // If not is the end position in range?
        else if(t->endPos > startPosition && t->endPos <= endPosition)
        {
            if(t->startPos < startPosition)
                setFormat(0,
                          t->endPos - t->startPos,
                          format(t->lexeme));
            else
                setFormat(t->startPos - startPosition,
                          t->endPos - t->startPos,
                          format(t->lexeme));
        }
    }
}

}
