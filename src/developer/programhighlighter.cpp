#include "programhighlighter.hpp"

#include <QSettings>
#include <QDebug>
#include <QVector>

#include "global.hpp"

namespace Developer {

ProgramHighlighter::ProgramHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // Set the list of keywords
    _keywords << "main" << "if" << "try" << "then" << "else" << "skip" << "fail"
              << "or";
}

void ProgramHighlighter::highlightBlock(const QString &text)
{
    int pos = 0;
    // This tracks the last (significant) lexeme encountered
    Lexemes last = Default;
    // This vector contains the position of opening parentheses - as closing
    // parens are found they are popped off the vector, at the end any unmatched
    // parens are marked as an error
    QVector<int> openParens;
    setCurrentBlockState(DefaultState);
    while(pos < text.length())
    {
        // Set up some variables for use in this pass
        QRegExp rx;
        int matchPos = -1;

        // If we're in the "InComment" state then the initial match should find
        // the close
        // We also handle the current block being in a comment for consistency's
        // sake
        if(currentBlockState() == InCommentState
                || previousBlockState() == InCommentState)
        {
            rx = pattern(CommentClose);
            if((matchPos = rx.indexIn(text,pos)) > 0)
            {
                // Include the closing section in the block to format
                matchPos += rx.matchedLength();
                setFormat(pos, matchPos-pos, format(Comment));
                pos = matchPos;
                setCurrentBlockState(DefaultState);
                continue;
            }
            else
            {
                // There's no closing comment in here, it's all comment from
                // here to the end of the string.
                setCurrentBlockState(InCommentState);
                setFormat(pos, text.length()-pos, format(Comment));
                break;
            }
        }

        // Check for a comment
        rx = pattern(CommentOpen);
        if((matchPos = rx.indexIn(text,pos)) == pos)
        {
            // Comment found, since this is a multi-line expression we need to
            // check if the close comment is in this block. If it isn't then we
            // just make the remaining text a comment and set the state to
            // InComment.
            setFormat(pos, rx.matchedLength(), format(Comment));
            pos += rx.matchedLength();
            setCurrentBlockState(InCommentState);
            continue;
        }

        // Check for a keyword
        rx = pattern(Keyword);
        if((matchPos = rx.indexIn(text, pos)) == pos)
        {
            setFormat(pos, rx.matchedLength(), format(Keyword));
            pos += rx.matchedLength();
            last = Keyword;
            continue;
        }

        // Check for a quoted string
        rx = pattern(QuotedString);
        if((matchPos = rx.indexIn(text, pos)) == pos)
        {
            setFormat(pos, rx.matchedLength(), format(QuotedString));
            pos += rx.matchedLength();
            last = QuotedString;
            continue;
        }

        // Check for a number
        rx = pattern(Number);
        if((matchPos = rx.indexIn(text, pos)) == pos)
        {
            setFormat(pos, rx.matchedLength(), format(Number));
            pos += rx.matchedLength();
            last = Number;
            continue;
        }

        // Check for an identifier
        rx = pattern(Identifier);
        if((matchPos = rx.indexIn(text,pos)) == pos)
        {
            setFormat(pos, rx.matchedLength(), format(Identifier));
            pos += rx.matchedLength();
            last = Identifier;
            continue;
        }

        // No match in this pass, set as default and advance the position by
        // one
        setFormat(pos, 1, format(Default));
        ++pos;
    }
}

QTextCharFormat ProgramHighlighter::format(int type) const
{
    QSettings settings;
    QColor background = settings.value(
                "Editor/BackgroundColor",
                Qt::white
                ).value<QColor>();

    // Start a character format, the majority will be on the default background
    QTextCharFormat ret;
    ret.setBackground(background);

    QFont defaultFont = EDITOR_DEFAULT_FONT;
    QFont defaultCommentFont = defaultFont;
    defaultCommentFont.setItalic(true);

    switch(type)
    {
    case Default:
        ret.setForeground(settings.value(
                              "Editor/Types/Default/Foreground",
                              Qt::black
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
    case Identifier:
        ret.setForeground(settings.value(
                              "Editor/Types/Identifier/Foreground",
                              Qt::darkBlue
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
    case Number:
        ret.setForeground(settings.value(
                              "Editor/Types/Number/Foreground",
                              Qt::darkCyan
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
                              Qt::red
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
    case Keyword:
        ret.setForeground(settings.value(
                              "Editor/Types/Keyword/Foreground",
                              Qt::darkYellow
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
    case Comment:
    case CommentOpen:
    case CommentClose:
        ret.setForeground(settings.value(
                              "Editor/Types/Comment/Foreground",
                              Qt::darkCyan
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
    default:
        qDebug() << "Unknown type passed in: " << type;
    case Error:
        ret.setForeground(settings.value(
                              "Editor/Types/Error/Foreground",
                              Qt::darkRed
                              ).value<QColor>()
                          );
        ret.setBackground(settings.value(
                              "Editor/Types/Error/Background",
                              QColor(0xff,0xcc,0xcc)
                              ).value<QColor>()
                          );
        ret.setFont(settings.value(
                        "Editor/Types/Error/Font",
                        defaultFont
                        ).value<QFont>()
                    );
        return ret;
    }
}

QRegExp ProgramHighlighter::pattern(int type) const
{
    switch(type)
    {
    case Comment:
        return QRegExp("[^*]*");
    case CommentOpen:
        return QRegExp("/\\*");
    case CommentClose:
        return QRegExp("\\*/");
    case Number:
        return QRegExp("\\d+");
    case Keyword:
        return QRegExp(_keywords.join("\\b|"));
    case QuotedString:
        return QRegExp("\"[^\"]*\"");
    case Identifier:
        return QRegExp("[a-zA-Z_][a-zA-Z0-9_]{,62}");
    case Default:
    case Error:
    default:
        qDebug() << "Attempted to find pattern for invalid type: " << type;
        return QRegExp();
    }
}

}
