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
#ifndef RULEPARSER_HPP
#define RULEPARSER_HPP

#include "parsertypes.hpp"

namespace Developer {

// Forward declaration of rule_t (parsertypes.hpp)
struct rule_t;

/*!
 * \brief parseRule takes in a string containing a saved rule and returns a
 *  rule_t datastructure with extracted data
 * \param rule  The rule to parse as a string. Not the path to a file.
 * \return A rule_t representing the rule passed in
 */
rule_t parseRule(const std::string &rule);

}

#endif // RULEPARSER_HPP
