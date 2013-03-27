#ifndef PROGRAMEDITOR_HPP
#define PROGRAMEDITOR_HPP

#include "codeeditor.hpp"

namespace Developer {

class ProgramHighlighter;

class ProgramEditor : public CodeEditor
{
    Q_OBJECT

public:
    ProgramEditor(QWidget *parent = 0);

    QRegExp pattern(int type) const;

public slots:
    void parse();

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

    QString errorString(const QString &tokenFound, int position);

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
