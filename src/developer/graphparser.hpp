/*!
 * \file
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
