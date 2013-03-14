#ifndef PROJECT_HPP
#define PROJECT_HPP

#include "global.hpp"

#include "graph.hpp"
#include "program.hpp"
#include "rule.hpp"

#include <QVector>
#include <vector>
#include <QFile>
#include <QObject>
#include <QString>
#include <QDebug>

namespace Developer {

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
 * An example of a simple project file follows, the format is a simple dialect
 * of XML containing the necessary information to locate all of the project's
 * files.
 *
 * \code
 *  <?xml version="1.0" encoding="UTF-8" ?>
 *  <project name="Project1" directory="/home/user/gpdeveloper/project1">
 *      <rules>
 *          <rule>rule1.gpr</rule>
 *          <rule>rule2.gpr</rule>
 *          <rule>rule3.gpr</rule>
 *      </rules>
 *      <programs>
 *          <program>program1.gpx</program>
 *      </programs>
 *      <graphs>
 *          <graph>graph1.gxl</graph>
 *          <graph>graph2.dot</graph>
 *      </graphs>
 *      <runconfigurations>
 *          <runconfiguration name="run1" program="program1" />
 *      </runconfigurations>
 *  </project>
 * \endcode
 */
class Project : public GPFile
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
        RuleFile,
        //! Any GP program (.gpx) files
        ProgramFile,
        //! Any graph (.dot or .gxl) files
        GraphFile
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
     * Return the GP ver
     */
    GPVersions gpVersion() const;
    void setGPVersion(GPVersions version);

    QString error() const;

    bool open();

    /*!
     * Open an existing project file at the provided location
     *
     * \param projectPath   The path to the project file
     * \return True if successfully opened, false otherwise
     */
    bool open(const QString &projectPath);

    /*!
     * Initialise a new project at the target location, a blank project file is
     * created along with subdirectories for rules, programs and graphs.
     *
     * \param targetPath    The path the project should be created at
     * \param projectName   The initial name for this project
     * \param gpVersion     The version of GP this project uses
     * \return  Returns true if the project was successfully created, false if
     *  it was not or if the user cancelled
     */
    bool initProject(const QString &targetPath, const QString &projectName, GPVersions gpVersion);

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
    void newGraph(const QString &name = QString(), GraphTypes type = Default);

    // Set of methods to add files to the current tracked project
    void addRule(const QString &path);
    void addProgram(const QString &path);
    void addGraph(const QString &path);

    void setCurrentFile(const QString &fileName, FileTypes type);

    // Inherited methods from GPFile
    bool save();
    bool saveAs(const QString &filePath);

    /*!
     * \brief Save the file specified
     * \param file  The path to the file
     * \return Boolean, true if saved successfully, false otherwise
     */
    bool saveFile(QString filePath = QString());
    bool saveFileAs(QString filePath = QString());
    bool saveAll();

    bool import();
    bool import(QString file);

signals:
    void fileChanged(QString file);

private slots:
    /*!
     * Internal slot used to handle file update signals from the directory
     * watcher instance. Determines if the file modified is tracked and if it is
     * then a signal is emitted to pass this change on to the application.
     *
     * \param filePath  The file which has changed
     */
    void fileModified(QString filePath);

private:
    /*!
     * Status variables for the current project to simplify data input/output
     * from Project objects
     */
    GPVersions _gpVersion;
    double _gpDeveloperVersion;
    QString _name;

    /*!
     * Error string which contains the last error encountered for elaboration
     * after returning false
     */
    QString _error;

    QVector<Rule*> _rules;
    QVector<Graph*> _graphs;
    QVector<Program*> _programs;
};

}

#endif // PROJECT_HPP
