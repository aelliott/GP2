#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <QVector>
#include <QFile>
#include <QFileSystemWatcher>
#include <QObject>
#include <QString>

#include "graph.hpp"

/*!
 * \brief Container type for GP projects, allowing for monitoring and updating
 *      project files.
 *
 * The general structure of projects is as follows:
 *
 * \code
 *  Project:
 *      - project file: (.gpp)
 *          - version information
 *          - rule/program/graph locations
 *          - run profiles
 *      - rules:
 *          - rule files (.gpr)
 *      - programs:
 *          - program files (.gpx)
 *      - graphs
 *          - graph files (.dot, .gxl)
 * \endcode
 *
 *
 */
class Project : public QObject
{
    Q_OBJECT

public:
    enum FileTypes
    {
        ProjectFile,
        Rule,
        Program,
        Graph
    };

    Project(const QString &projectPath = QString(), QObject *parent = 0);
    ~Project();

    bool initProject(const QString &targetPath, const QString &projectName);

    // Set of methods to create new (empty) files
    void newRule(const QString &name = QString());
    void newProgram(const QString &name = QString());
    void newGraph(const QString &name = QString(), Graph::GraphTypes type = Graph::Default);

    // Set of methods to add files to the current tracked project
    void addRule(const QString &path);
    void addProgram(const QString &path);
    void addGraph(const QString &path);

    void setCurrentFile(const QString &fileName, FileTypes type);

    bool save(QString file = QString());
    bool saveAs(QString file = QString());
    bool saveAll();

    bool import();
    bool import(QString file);

private:
    /*!
     * This is the path to the project file, the project directory is the
     * directory which contains this file and is therefore easily derived from
     * this variable.
     */
    QString _projectPath;

    /*!
     * A watcher is necessary to check for edits made to files from outside of
     * this application, in such cases the project should signal and allow the
     * application to prompt the user on whether they want to save the existing
     * data or reload the file.
     */
    QFileSystemWatcher *_watcher;

    /*!
     * Collection of convenience QFile objects which allow for simple access to
     * the files underneath just by calling open/close on the correct QFile.
     *
     * _currentFile should be kept up-to-date with the file currently open in
     * the Edit tab (if there is one).
     */
    QFile _projectFile;
    QFile *_currentFile;
    QVector<QFile> _rules;
    QVector<QFile> _programs;
    QVector<QFile> _graphs;
};

#endif // PROJECT_HPP
