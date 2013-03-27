#ifndef CODEEDITOR_HPP
#define CODEEDITOR_HPP

#include <QPlainTextEdit>
#include "token.hpp"
#include "global.hpp"

namespace Developer {

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = 0);

    /*!
     * \brief Gets the regular expression associated with the provided type
     * \param type  The type of the token whose regexp we are retrieving. This
     *  will be a value from the Lexemes enum
     * \return A QRegExp object containing a pattern which will match this type
     *  of lexeme
     */
    virtual QRegExp pattern(int type) const = 0;

public slots:
    virtual void parse() = 0;

protected:
    QVector<Token *> _tokens;
    QStringList _keywords;
};

}

#endif // CODEEDITOR_HPP
