#ifndef PROGRAMHIGHLIGHTER_HPP
#define PROGRAMHIGHLIGHTER_HPP

#include <QSyntaxHighlighter>
#include <QRegExp>

namespace Developer {

class ProgramHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    enum Lexemes
    {
        Default,
        Identifier,
        Number,
        Keyword,
        QuotedString,
        Comment,
        CommentOpen,
        CommentClose,
        Error
    };

    enum States
    {
        DefaultState,
        InCommentState,
        ErrorState
    };

    explicit ProgramHighlighter(QTextDocument *parent = 0);

    void highlightBlock(const QString &text);

    QTextCharFormat format(int type) const;
    QRegExp pattern(int type) const;

private:
    QStringList _keywords;
};

}

#endif // PROGRAMHIGHLIGHTER_HPP
