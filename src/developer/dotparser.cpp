/*!
 * \file
 */
#include "dotparser.hpp"

#include <QPointF>

namespace Developer {

DotParser::DotParser(const QString &dotString)
    : _contents(dotString)
    , _pos(0)
{
    if(!dotString.isEmpty())
        parse();
}

bool DotParser::parse(const QString &dotString)
{
    if(!dotString.isEmpty())
        _contents = dotString;

    _graph.canvasX = 0.0;
    _graph.canvasY = 0.0;
    _graph.nodes.clear();
    _graph.edges.clear();
    _pos = 0;
    _nodes.clear();

    return parseGraph();
}

bool DotParser::parseGraph()
{
    while(_pos < _contents.length())
    {
        if(consumeWhitespace() || consumeComments())
            continue;

        QRegExp rx = pattern(GraphOpen);
        if(rx.indexIn(_contents,_pos) == _pos)
        {
            _pos += rx.matchedLength();

            rx = pattern(GraphClose);

            while(_pos < _contents.length())
            {
                if(consumeWhitespace() || consumeComments())
                    continue;

                if(rx.indexIn(_contents,_pos) == _pos)
                    return true;

                if(!parseItem())
                    consumeError();
            }
        }
        else
            consumeError();
    }

    return false;
}

bool DotParser::parseItem()
{
    QRegExp rx = pattern(Identifier);
    int statementStart = _pos;
    if(rx.indexIn(_contents, _pos) == _pos)
    {
        QString id = rx.cap(0);
        _pos += rx.matchedLength();
        if(id == "graph")
        {
            // Parse graph properties
            while(consumeWhitespace() || consumeComments());

            QMap<QString, QVariant> attributes = parseAttributes();
            if(attributes.contains("bb"))
            {
                QString canvas = attributes["bb"].toString();
                QStringList rect = canvas.split(",");
                if(rect.length() < 4)
                {
                    qDebug() << "Could not parse bb value: " << canvas;
                    qDebug() << "Expected \\d+(,\\d+){3}";
                }

                _graph.canvasX = QVariant(rect.at(2)).toDouble();
                _graph.canvasY = QVariant(rect.at(3)).toDouble();
            }
        }
        else if(id == "subgraph" || id == "node" || id == "edge")
        {
            // Ignored properties
            while(consumeWhitespace() || consumeComments());
            parseAttributes();
        }
        else
        {
            // It's a regular ID, check if this is a node or an edge by looking
            // for an edge operator
            while(consumeWhitespace() || consumeComments());

            rx = pattern(EdgeOperator);
            if(rx.indexIn(_contents, _pos) == _pos)
            {
                // Edge
                _pos += rx.matchedLength();
                while(consumeWhitespace() || consumeComments());

                rx = pattern(Identifier);
                if(rx.indexIn(_contents, _pos) == _pos)
                {
                    _pos += rx.matchedLength();
                    QString to = rx.cap(0);

                    if(!_nodes.contains(id) || !_nodes.contains(to))
                    {
                        qDebug() << "Edge missing a required node";
                        return false;
                    }

                    while(consumeWhitespace() || consumeComments());

                    QMap<QString, QVariant> attributes = parseAttributes();
                    QString edgeId;
                    if(attributes.contains("id"))
                        edgeId = attributes["id"].toString();
                    else
                    {
                        qDebug() << "Edge missing ID attribute, ignoring.";
                        return false;
                    }
                    QString label;
                    if(attributes.contains("label"))
                        label = attributes["label"].toString();

                    edge_t edge;
                    edge.id = edgeId.toStdString();
                    edge.label = label.toStdString();
                    edge.from = id.toStdString();
                    edge.to = to.toStdString();
                    _graph.edges.push_back(edge);
                }
            }
            else
            {
                // Node
                _nodes << id;
                while(consumeWhitespace() || consumeComments());

                QMap<QString, QVariant> attributes = parseAttributes();

                QString label;
                QPointF position;
                if(attributes.contains("label"))
                    label = attributes["label"].toString();
                if(attributes.contains("pos"))
                {
                    QString pos = attributes["pos"].toString();
                    QStringList coords = pos.split(",");
                    if(coords.length() > 1)
                    {
                        position.setX(QVariant(coords.at(0)).toDouble());
                        position.setY(QVariant(coords.at(1)).toDouble());
                    }
                }

                node_t node;
                node.id = id.toStdString();
                node.label = label.toStdString();
                node.xPos = position.x();
                node.yPos = position.y();
                _graph.nodes.push_back(node);
            }
        }

        while(consumeWhitespace() || consumeComments());

        rx = pattern(StatementSeparator);
        if(rx.indexIn(_contents,_pos) != _pos)
        {
            qDebug() << "Dot Parser: Statement starting at " << statementStart
                     << " was not closed";
            return false;
        }

        _pos += rx.matchedLength();
        return true;
    }
    else
        return false;
}

QMap<QString,QVariant> DotParser::parseAttributes()
{
    QMap<QString,QVariant> attributes;

    QRegExp rx = pattern(AttributeListOpen);
    if(rx.indexIn(_contents,_pos) == _pos)
    {
        _pos += rx.matchedLength();
        while(_pos < _contents.length())
        {
            if(consumeWhitespace() || consumeComments())
                continue;

            rx = pattern(AttributeListClose);
            if(rx.indexIn(_contents,_pos) == _pos)
            {
                _pos += rx.matchedLength();
                return attributes;
            }

            rx = pattern(Identifier);
            if(rx.indexIn(_contents,_pos) == _pos)
            {
                QString name = rx.cap(0);
                _pos += rx.matchedLength();

                while(consumeWhitespace() || consumeComments());

                if(_contents.at(_pos) == QChar('='))
                {
                    ++_pos;
                    while(consumeWhitespace() || consumeComments());

                    rx = pattern(Number);
                    if(rx.indexIn(_contents,_pos) == _pos)
                    {
                        _pos += rx.matchedLength();
                        double number = QVariant(rx.cap(0)).toDouble();
                        attributes.insert(name, QVariant(number));
                        continue;
                    }

                    rx = pattern(Identifier);
                    if(rx.indexIn(_contents,_pos) == _pos)
                    {
                        _pos += rx.matchedLength();
                        attributes.insert(name, rx.cap(0));
                        continue;
                    }

                    rx = pattern(QuotedString);
                    if(rx.indexIn(_contents,_pos) == _pos)
                    {
                        _pos += rx.matchedLength();
                        QString str = rx.cap(0);
                        str.remove("\"");
                        attributes.insert(name, str);
                        continue;
                    }

                    qDebug() << "Dot Parser: bad attribute value given.";
                    qDebug() << name << "=" << _contents.mid(_pos, 20) + "...";
                    continue;
                }
                else
                {
                    qDebug() << "Attribute name followed by a non equals character "
                                << _contents.at(_pos);
                    continue;
                }
            }

            if(_contents.at(_pos) == QChar(','))
            {
                ++_pos;
                continue;
            }

            qDebug() << "Unexpected char " << _contents.at(_pos) << " at "
                        "position " << _pos;
            consumeError();
        }
    }

    return attributes;
}

bool DotParser::consumeWhitespace()
{
    QRegExp rx("\\s+");
    if(rx.indexIn(_contents,_pos) == _pos)
    {
        Q_ASSERT(rx.matchedLength() > 0);
        _pos += rx.matchedLength();
        return true;
    }
    else
        return false;
}

bool DotParser::consumeComments()
{
    // Check for a multi-linecomment
    QRegExp rx = pattern(CommentOpen);
    int matchPos = -1;
    if((matchPos = rx.indexIn(_contents,_pos)) == _pos)
    {
        // Comment found, now we need to check for an ending and if we can't
        // find one then we just mark a comment to the end of the program
        // and finish
        _pos += rx.matchedLength();
        rx = pattern(CommentClose);
        if((matchPos = rx.indexIn(_contents, _pos)) > 0)
        {
            // We found a closing token, set the end position correctly and
            // advance the position
            _pos = matchPos + rx.matchedLength();
            return true;
        }
        else
        {
            // There was no closing token, match to the end of the string
            _pos = _contents.length();
            return true;
        }
    }

    rx = pattern(SingleLineComment);
    if(rx.indexIn(_contents,_pos) == _pos)
    {
        _pos += rx.matchedLength();
        return true;
    }

    return false;
}

void DotParser::consumeError()
{
    // Identifiers are the only contiguous segments
    QRegExp rx = pattern(Identifier);
    if(rx.indexIn(_contents,_pos) != _pos)
    {
        // This isn't a block, move along one char
        ++_pos;
        return;
    }

    _pos += rx.matchedLength();
}

graph_t DotParser::toGraph() const
{
    return _graph;
}

QRegExp DotParser::pattern(int type) const
{
    switch(type)
    {
    case SingleLineComment:
        return QRegExp("(//|#)[^\\n]\\n");
    case CommentOpen:
        return QRegExp("/\\*");
    case CommentClose:
        return QRegExp("\\*/");
    case Identifier:
        return QRegExp("[a-zA-Z0-9_]{1,63}");
    case GraphOpen:
        return QRegExp("digraph\\s*([a-zA-Z0-9_]{,63})\\s*\\{");
    case GraphClose:
        return QRegExp("\\}");
    case EdgeOperator:
        return QRegExp("->");
    case AttributeListOpen:
        return QRegExp("\\[");
    case AttributeListClose:
        return QRegExp("\\]");
    case StatementSeparator:
        return QRegExp(";");
    case QuotedString:
        return QRegExp("\"[^\"]*\"");
    case Number:
        return QRegExp("-?(\\.\\d+|\\d+(\\.\\d*))");
    default:
        qDebug() << "DotParser::pattern(): Unknown lexeme type: " << type;
        return QRegExp();
    }
}

}
