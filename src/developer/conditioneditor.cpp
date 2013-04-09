/*!
 * \file
 */
#include "conditioneditor.hpp"
#include "conditionhighlighter.hpp"
#include "conditiontokens.hpp"

#include <QDebug>
#include <QRegExp>
#include <QSettings>
#include <QString>
#include <QToolTip>

namespace Developer {

ConditionEditor::ConditionEditor(QWidget *parent)
    : CodeEditor(parent)
    , _condition("")
    , _cache("")
    , _pos(0)
{
    _keywords << "edge" << "node" << "not" << "and" << "or" << "int" << "string"
              << "atom" << "empty" << "true" << "false" << "indeg" << "outdeg";

    // Mouse tracking is required for tooltips
    setMouseTracking(true);

    QSettings settings;
    QFont font = settings.value("Editor/Font", EDITOR_DEFAULT_FONT).value<QFont>();
    setFont(font);

    _highlighter = new ConditionHighlighter(this->document());
    _highlighter->setTokens(_tokens);
}

QRegExp ConditionEditor::pattern(int type) const
{
    QString keywordsList = _keywords.join("|");

    switch(type)
    {
    case ConditionLexeme_Identifier:
        return QRegExp("[a-zA-Z0-9_]+");
    case ConditionLexeme_Number:
        return QRegExp("-?(\\d+\\.?\\d*|\\d*\\.\\d+)");
    case ConditionLexeme_QuotedString:
        return QRegExp("\"[^\"]*\"");
    case ConditionLexeme_Keyword:
        return QRegExp(keywordsList);
    case ConditionLexeme_Operator:
        return QRegExp("=|!=|,|>|<|>=|<=|\\-|\\+|/|\\*");
    case ConditionLexeme_OpenParen:
        return QRegExp("\\(");
    case ConditionLexeme_CloseParen:
        return QRegExp("\\)");
    case ConditionLexeme_CommentOpen:
        return QRegExp("/\\*");
    case ConditionLexeme_CommentClose:
        return QRegExp("\\*/");
    case ConditionLexeme_Comment:
    case ConditionLexeme_Default:
    default:
        qDebug() << "Unknown type passed into ConditionEditor::pattern(): "
                    << type;
        return QRegExp();
    }
}

void ConditionEditor::parse()
{
    _condition = toPlainText();
    if(_cache == _condition)
        return;
    else
        _cache = _condition;

    _tokens.clear();
    _pos = 0;

    while(_pos < _condition.length())
    {
        if(consumeWhitespace() || consumeComments())
            continue;

        int posBefore = _pos;
        parseCondition();

        if(posBefore == _pos)
            consumeError("Expecting a condition.");
    }

    _highlighter->setTokens(_tokens);
}

void ConditionEditor::parseCondition()
{

}

bool ConditionEditor::consumeWhitespace()
{
    QRegExp rx("\\s+");
    if(rx.indexIn(_condition,_pos) == _pos)
    {
        Q_ASSERT(rx.matchedLength() > 0);
        _pos += rx.matchedLength();
        return true;
    }
    else
        return false;
}

bool ConditionEditor::consumeComments()
{
    // Check for a comment
    QRegExp rx = pattern(ConditionLexeme_CommentOpen);
    int matchPos = -1;
    Token *token = new Token;
    token->startPos = _pos;
    if((matchPos = rx.indexIn(_condition,_pos)) == _pos)
    {
        // Comment found, now we need to check for an ending and if we can't
        // find one then we just mark a comment to the end of the condition
        // and finish
        token->lexeme = ConditionLexeme_Comment;
        _pos += rx.matchedLength();
        rx = pattern(ConditionLexeme_CommentClose);
        if((matchPos = rx.indexIn(_condition, _pos)) > 0)
        {
            // We found a closing token, set the end position correctly and
            // advance the position
            token->text = _condition.mid(
                        _pos,
                        (matchPos-_pos) + rx.matchedLength());
            _pos = matchPos + rx.matchedLength();
            token->endPos = _pos;
            _tokens.push_back(token);
            return true;
        }
        else
        {
            // There was no closing token, match to the end of the string
            token->text = _condition.mid(
                        _pos,
                        _condition.length() - _pos);
            token->endPos = _condition.length();
            _pos = _condition.length();
            _tokens.push_back(token);
            return true;
        }
    }

    // No comment found
    delete token;
    return false;
}

void ConditionEditor::consumeError(const QString &expecting)
{
    Token *error = new Token;
    error->lexeme = ConditionLexeme_Error;
    error->startPos = _pos;

    // Identifiers are the only contiguous segments
    QRegExp rx = pattern(ConditionLexeme_Identifier);
    if(rx.indexIn(_condition,_pos) != _pos)
    {
        // This isn't a block, move along one char
        error->text = QString(_condition.at(_pos));
        error->description = errorString(error->text, _pos) +
                expecting;
        error->endPos = ++_pos;
        _tokens.push_back(error);
        return;
    }

    error->text = rx.cap(0);
    error->endPos = _pos + rx.matchedLength();
    error->description = errorString(error->text, _pos) +
            expecting;
    _tokens.push_back(error);
    _pos += rx.matchedLength();
}

QString ConditionEditor::errorString(const QString &tokenFound, int position)
{
    QString ret =  tr("Unexpected token %1 at position %2. ").arg(
                tokenFound,
                QVariant(position).toString());
    //qDebug() << ret;
    return ret;
}

void ConditionEditor::mouseMoveEvent(QMouseEvent *e)
{
    QTextCursor textCursor = cursorForPosition(e->pos());
    textCursor.select(QTextCursor::WordUnderCursor);
    if(textCursor.selectedText().isEmpty())
        QToolTip::hideText();
    else
    {
        //! \todo Adapt this to handle error tokens /inside/ what the editor
        //! considers words
        // We have a word here, is it an error token?
        for(QVector<Token *>::iterator iter = _tokens.begin();
            iter != _tokens.end(); ++iter)
        {
            Token *t = *iter;
            if(textCursor.position() >= t->startPos
                    && textCursor.position() <= t->endPos)
            {
                if(t->lexeme == ConditionLexeme_Error)
                    QToolTip::showText(e->globalPos(), t->description);
            }
        }
    }
    CodeEditor::mouseMoveEvent(e);
}

}
