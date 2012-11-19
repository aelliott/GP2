#ifndef PROJECT_HPP
#define PROJECT_HPP

#include "global.hpp"

#include <QVector>
#include <QFile>
#include <QFileSystemWatcher>
#include <QObject>
#include <QString>
#include <QDebug>

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
 * An example of a simple project file follows, demonstrating the syntax used:
 *
 * \code
 *  GP_VERSION = 2.0
 *  GP_DEVELOPER_VERSION = 0.1
 *
 *  PROJECT_NAME = Project 1
 *
 *  RULES = rules/rule01.gpr \\
 *      rules/rule02.gpr \\
 *      rules/rule03.gpr
 *
 *  PROGRAMS = programs/program01.gpx
 *
 *  GRAPHS = graphs/graph01.dot \\
 *      graphs/graph02.dot \\
 *      /usr/course/gra/external_graph.dot
 *
 *  RUN_CONFIGURATIONS = (program01, graphs/graph01.dot) \\
 *      (program01, /usr/course/gra/external_graph.dot)
 * \endcode
 */
class Project : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief The FileTypes enum specifies the types of file the project can
     *  contain
     */
    enum FileTypes
    {
        //! The project (.gpp) file
        ProjectFile,
        //! Any GP rule (.gpr) files
        Rule,
        //! Any GP program (.gpx) files
        Program,
        //! Any graph (.dot or .gxl) files
        Graph
    };

    /*!
     * \brief The GPVersions enum specifies the types of GP program allowed
     */
    enum GPVersions
    {
        //! Greg Manning's GP
        GP1,
        //! GP2, without rooted graph programs
        GP2,
        //! GP2, with rooted graph programs
        RootedGP2
    };

    /*!
     * Construct a new Project, if a path to a valid project file is found then
     * initialise using that file, otherwise create a blank project which then
     * must be initialised with initProject() before the project can be used
     *
     * \param projectPath   Optional path to an existing project file
     * \param parent        This object's parent object
     */
    Project(const QString &projectPath = QString(), QObject *parent = 0);

    /*!
     * Destroy the project object and free memory
     */
    ~Project();

    /*!
     * Return the name of this project
     *
     * \return The project's name
     */
    QString name() const;

    /*!
     * Set the name of this project to the provided string
     *
     * \param name  The new name of this project
     */
    void setName(const QString &name);

    /*!
     * Open an existing project file at the provided location
     *
     * \param projectPath   The path to the project file
     * \return True if successfully opened, false otherwise
     */
    bool openProject(const QString &projectPath);

    /*!
     * Initialise a new project at the target location, a blank project file is
     * created along with subdirectories for rules, programs and graphs.
     *
     * \param targetPath    The path the project should be created at
     * \param projectName   The initial name for this project
     * \return  Returns true if the project was successfully created, false if
     *  it was not or if the user cancelled
     */
    bool initProject(const QString &targetPath, const QString &projectName);

    // Set of methods to create new (empty) files
    /*!
     * Create a new GP rule for this project, automatically add it to the
     * project file.
     *
     * \param name  The name of the new GP rule to create
     */
    void newRule(const QString &name = QString());

    /*!
     * Create a new GP program for this project, automatically add it to the
     * project file.
     *
     * \param name  The name of the new GP program to create
     */
    void newProgram(const QString &name = QString());

    /*!
     * Create a new blank graph file, the type of which can be specified,
     * automatically add it to the project file
     *
     * \param name  The name of the new graph to create
     * \param type  The type of graph to create
     */
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

private slots:
    /*!
     * Internal slot used to handle file update signals from the directory
     * watcher instance. Determines if the file modified is tracked and if it is
     * then a signal is emitted to pass this change on to the application.
     *
     * \param   file    The file which has changed
     */
    void fileChanged(QString file);

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
     * Status variables for the current project to simplify data input/output
     * from Project objects
     */
    double _gpVersion;
    double _gpDeveloperVersion;
    QString _name;

    /*!
     * Collection of convenience QFile objects which allow for simple access to
     * the files underneath just by calling open/close on the correct QFile.
     *
     * _currentFile should be kept up-to-date with the file currently open in
     * the Edit tab (if there is one).
     */
    QFile *_projectFile;
    QFile *_currentFile;
    QVector<QFile *> _rules;
    QVector<QFile *> _programs;
    QVector<QFile *> _graphs;
};

const QString GPVersionToString(Project::GPVersions version);
Project::GPVersions stringToGPVersion(const QString &version);

#endif // PROJECT_HPP
