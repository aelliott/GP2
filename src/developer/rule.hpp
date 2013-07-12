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
    enum RuleOptions
    {
        Rule_DefaultBehaviour = 0x0000,
        Rule_InjectiveMatching = 0x0001
    };

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
    const QString &name() const;
    const QString &documentation() const;
    Graph *lhs() const;
    Graph *rhs() const;
    const QString &condition() const;

    int options() const;
    bool injectiveMatching() const;

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
    void setDocumentation(const QString &docString);
    void setLhs(Graph *lhsGraph);
    void setRhs(Graph *rhsGraph);
    void setCondition(const QString &conditionString);

    void setOptions(int options);
    void setInjectiveMatching(bool injective);

    bool save();
    bool saveAs(const QString &filePath);

    bool open();

protected slots:
    void lhsGraphChanged();
    void rhsGraphChanged();

private:
    QString _name;
    QString _documentation;
    Graph *_lhs;
    Graph *_rhs;
    QString _condition;
    int _options;
};

}

#endif // RULE_HPP
