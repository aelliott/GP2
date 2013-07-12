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
#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "gpfile.hpp"

namespace Developer {

class Program : public GPFile
{
    Q_OBJECT

public:
    explicit Program(const QString &programPath = QString(), QObject *parent = 0);

    QString name() const;
    void setName(const QString &programName);
    QString program() const;
    void setProgram(const QString &programText);
    QString documentation() const;
    void setDocumentation(const QString &programDocumentation);

    bool save();
    bool saveAs(const QString &filePath = QString());

    bool open();

private:
    QString _name;
    QString _program;
    QString _documentation;
};

}

#endif // PROGRAM_HPP
