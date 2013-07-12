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
#include "parsertypes.hpp"

#include "list.hpp"

namespace Developer {

QDebug operator<<(QDebug dbg, const std::string &str)
{
    dbg.nospace() << QString(str.c_str());
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const param_t &param)
{
    dbg.nospace() << "param(";

    if(param.variables.size() == 0)
        dbg.nospace() << "empty";
    else if(param.variables.size() == 1)
        dbg.nospace() << param.variables.at(0);
    else
    {
        bool first = true;
        for(std::vector<std::string>::const_iterator iter = param.variables.begin();
            iter != param.variables.end(); ++iter)
        {
            if(first)
                first = false;
            else
                dbg.nospace() << ",";
            dbg.nospace() << *iter;
        }
    }

    dbg.nospace() << ", " << param.type << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const std::vector<param_t> &params)
{
    dbg.nospace() << "params(";
    bool first = true;
    for(std::vector<param_t>::const_iterator iter = params.begin();
        iter != params.end(); ++iter)
    {
        if(first)
            first = false;
        else
            dbg.nospace() << ", ";
        dbg.nospace() << *iter;
    }
    dbg.nospace() << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const node_t &node)
{
    dbg.nospace() << "node(" << node.id  << ", " << List(node.label).toString()
                  << ", (" << node.xPos << "," << node.yPos << ")" << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const std::vector<node_t> &nodes)
{
    dbg.nospace() << "nodes(";
    bool first = true;
    for(std::vector<node_t>::const_iterator iter = nodes.begin();
        iter != nodes.end(); ++iter)
    {
        if(first)
            first = false;
        else
            dbg.nospace() << ", ";
        dbg.nospace() << *iter;
    }
    dbg.nospace() << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const edge_t &edge)
{
    dbg.nospace() << "edge(" << edge.from << ", " << edge.to << ", "
                  << List(edge.label).toString() << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const std::vector<edge_t> &edges)
{
    dbg.nospace() << "edges(";
    bool first = true;
    for(std::vector<edge_t>::const_iterator iter = edges.begin();
        iter != edges.end(); ++iter)
    {
        if(first)
            first = false;
        else
            dbg.nospace() << ", ";
        dbg.nospace() << *iter;
    }
    dbg.nospace() << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const interface_t &interface)
{
    dbg.nospace() << "(" << interface.lhsId << ", " << interface.rhsId << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const std::vector<interface_t> &interfaces)
{
    dbg.nospace() << "interface(";
    bool first = true;
    for(std::vector<interface_t>::const_iterator iter = interfaces.begin();
        iter != interfaces.end(); ++iter)
    {
        if(first)
            first = false;
        else
            dbg.nospace() << ", ";
        dbg.nospace() << *iter;
    }
    dbg.nospace() << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const graph_t &graph)
{
    dbg.nospace() << "graph((" << graph.canvasX << "," << graph.canvasY << "), "
                  << graph.nodes << ", " << graph.edges << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const rule_t &rule)
{
    dbg.nospace() << "rule(" << rule.documentation << ", " << rule.id << ", "
                  << rule.parameters;

    if(rule.lhs.is_initialized())
        dbg.nospace() << ", " << rule.lhs.get();
    else
        dbg.nospace() << ", graph()";

    if(rule.rhs.is_initialized())
        dbg.nospace() << ", " << rule.rhs.get();
    else
        dbg.nospace() << ", graph()";

    dbg.nospace() << ", " << rule.interfaces;

    if(rule.condition.is_initialized())
        dbg.nospace() << ", " << rule.condition.get();
    dbg.nospace() << ")";
    return dbg.space();
}

}
