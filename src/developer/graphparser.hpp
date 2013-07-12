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
#ifndef GRAPHPARSER_HPP
#define GRAPHPARSER_HPP

#include "parsertypes.hpp"
#include <QString>

namespace Developer {

/*!
 * \brief Parse in a graph from the "alternative" format using boost::spirit
 * \param graphString   A string containing the graph to parse
 * \return A graph_t representing the provided graph if it is valid, an
 *  uninitialised one if not
 */
graph_t parseAlternativeGraph(const std::string &graphString);

/*!
 * \brief Parse in a graph from the "dot" format (used by graphviz)
 * \param graphString   A string containing the graph to parse
 * \return A graph_t representing the provided graph
 */
graph_t parseDotGraph(const QString &graphString);

/*!
 * \brief Parse in a graph from the "GXL" format (Graph eXchange Language)
 * \param graphString   A string containing the graph to parse
 * \return A graph_t representing the provided graph
 */
graph_t parseGxlGraph(const QString &graphString);

}

#endif // GRAPHPARSER_HPP
