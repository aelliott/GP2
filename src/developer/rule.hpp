#ifndef RULE_HPP
#define RULE_HPP

#include "gpfile.hpp"

namespace Developer {

class Graph;

/*!
 * \brief The Rule class represents a file file (.gpr) in GP Developer
 *
 * A rule in GP consists of a LHS (aka pattern graph), RHS (aka replacement
 * graph) and a boolean condition which must evaluate to true for the rule to be
 * applicable.
 */
class Rule : public GPFile
{
    Q_OBJECT

public:
    /*!
     * \brief Construct a new Rule
     *
     * Optionally the second parameter may contain a path to a rule file. If
     * this is provided then the Rule will automatically try to initialise
     * itself from this file.
     *
     * \param rulePath  Optional. Path to a rule file to open
     * \param parent    This object's parent object
     */
    explicit Rule(const QString &rulePath = QString(), QObject *parent = 0);

    /*!
     * \brief Get this rule's name (its identifier)
     * \return A string containing the rule's name if the Rule contains one, an
     *  empty string otherwise.
     */
    QString name() const;
    /*!
     * \brief Set this rule's name (its identifier)
     *
     * Identifiers in GP are restricted to the following regular expression:
     * `[a-zA-Z_][a-zA-Z0-9_]{,N}` where N is at least 62 (the size of permitted
     * identifiers can be larger than this, the GP2 implementation will document
     * the maximum identifier size).
     *
     * \param ruleName
     */
    void setName(const QString &ruleName);

    QString documentation() const;

    bool save();
    bool saveAs(const QString &filePath);

    bool open();

private:
    QString _name;
    QString _documentation;
    Graph *_lhs;
    Graph *_rhs;
    QString _condition;
};

}

#endif // RULE_HPP
