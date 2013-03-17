#include "project.hpp"

#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QDateTime>

#include <QDomDocument>

namespace Developer {

Project::Project(const QString &projectPath, QObject *parent)
    : GPFile(projectPath, parent)
    , _gpVersion(DEFAULT_GP_VERSION)
    , _gpDeveloperVersion(GP_DEVELOPER_VERSION)
    , _name("")
    , _null(true)
    , _error("")
{
    if(!projectPath.isEmpty())
        open(projectPath);
}

Project::~Project()
{

}

QString Project::name() const
{
    return _name;
}

void Project::setName(const QString &name)
{
    _name = name;
}

GPVersions Project::gpVersion() const
{
    return _gpVersion;
}

void Project::setGPVersion(GPVersions version)
{
    _gpVersion = version;
}

bool Project::isNull() const
{
    return _null;
}

QString Project::error() const
{
    return _error;
}

QDir Project::rulesDir() const
{
    QDir d = dir();
    d.cd("rules");

    // This directory should always exist, if it doesn't then make it again
    if(!d.exists())
        d.mkpath(d.absolutePath());

    return d;
}

QDir Project::programsDir() const
{
    QDir d = dir();
    d.cd("programs");

    // This directory should always exist, if it doesn't then make it again
    if(!d.exists())
        d.mkpath(d.absolutePath());

    return d;
}

QDir Project::graphsDir() const
{
    QDir d = dir();
    d.cd("graphs");

    // This directory should always exist, if it doesn't then make it again
    if(!d.exists())
        d.mkpath(d.absolutePath());

    return d;
}

bool Project::open()
{
    return GPFile::open();
}

bool Project::open(const QString &projectPath)
{
    _path = projectPath;
    if(!open())
    {
        _error = tr("The project specified (%1) could not be opened"
                    ).arg(projectPath);
        return false;
    }

    // We need the file to exist to proceed, fail if the status is not Normal
    if(_status != GPFile::Normal)
    {
        _error = tr("The project specified (%1) did not exist"
                    ).arg(projectPath);
        return false;
    }

    // Parse the file, details of the format are in the documentation for the
    // Project class
    QString proj = _fp->readAll();
    if(proj.isEmpty())
    {
        _error = tr("The project specified (%1) was empty"
                    ).arg(projectPath);
        return false;
    }

    QDomDocument document("project");
    if(!document.setContent(proj))
    {
        _error = tr("The project file (%1) could not be parsed as XML"
                    ).arg(projectPath);
        return false;
    }

    // First get the top-level <project> element, fail if it cannot be found
    QDomNodeList nodes = document.elementsByTagName("project");
    if(nodes.isEmpty())
    {
        _error = tr("The project file (%1) provided could not be parsed as a GP"
                    "Developer project"
                    ).arg(projectPath);
        return false;
    }

    // Take the node returned, make it into an element, and then grab all of the
    // attributes associated with it
    QDomNode node = nodes.at(0);
    QDomElement elem = node.toElement();
    QString name = elem.attribute("name");
    QString directory = elem.attribute("directory");
    QString gpVersion = elem.attribute(
                "gpversion",
                GPVersionToString(DEFAULT_GP_VERSION)
                );
    QString gpDeveloperVersion = elem.attribute(
                "developerversion",
                QVariant(GP_DEVELOPER_VERSION).toString()
                );

    // A name and a target directory must be present, in addition the directory
    // provided must exist
    if(name.isEmpty() || directory.isEmpty()
            || !QDir(directory).exists())
    {
        _error = tr("The project file (%1) provided could not be parsed as a GP"
                    "Developer project"
                    ).arg(projectPath);
        return false;
    }

    _null = false;
    _error = "";
    return true;
}

bool Project::initProject(const QString &targetPath, const QString &projectName, GPVersions gpVersion)
{
    QDir dir(targetPath);
    if(!dir.isAbsolutePath(targetPath))
        return false;

    if(!dir.exists())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(0, tr("Create Directory?"),
                                      tr("The directory specified (%1) does not"
                                         " exist, create it?").arg(targetPath),
                                      QMessageBox::Yes | QMessageBox::Cancel
                                      );
        if(reply != QMessageBox::Yes)
            return false;

        dir.mkpath(targetPath);
    }

    // Check for subdirectories called "rules", "programs" or "graphs" - create
    // them automatically if they don't exist.
    QString path = targetPath;
    if(path.at(targetPath.length()-1) != dir.separator())
        path += QString(dir.separator());
    QString rulesPath = path + "rules";
    QString programsPath = path + "programs";
    QString graphsPath = path + "graphs";

    QDir rules(rulesPath);
    if(!rules.exists())
        rules.mkpath(rulesPath);

    QDir programs(programsPath);
    if(!programs.exists())
        programs.mkpath(programsPath);

    QDir graphs(graphsPath);
    if(!graphs.exists())
        graphs.mkpath(graphsPath);

    QFile file(dir.filePath(projectName + GP_PROJECT_EXTENSION));
    _path = dir.filePath(projectName + GP_PROJECT_EXTENSION);
    file.open(QFile::WriteOnly);

    setName(projectName);

    /*
     * Write a basic template project file
     *
     * Replacements:
     *  %1 => Project name
     *  %2 => Project working directory
     *  %3 => GP version
     *  %4 => GP Developer version
     */
    QFile fp(":/templates/newproject.gpp");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString newProject = fp.readAll();
    newProject = newProject.arg(
                projectName,
                targetPath,
                GPVersionToString(gpVersion),
                QVariant(GP_DEVELOPER_VERSION).toString()
                );

    // First just open up the file, don't parse it
    if(!open())
        return false;

    _fp->write(QVariant(newProject).toByteArray());

    // Now we can parse it
    open(_path);

    return true;
}

void Project::newRule(const QString &name)
{
    // Have we been passed an absolute path?
    //! \todo handle this

    // Get the directory in which we should be creating this rule
    QDir d = rulesDir();

    // Check if a rule with this name already exists
    QString filePath = d.filePath(name + GP_RULE_EXTENSION);
    QFile file(filePath);

    if(file.exists())
    {
        QMessageBox::StandardButton response;
        response = QMessageBox::warning(
                    0,
                    tr("File Exists"),
                    tr("A file called \"%1\" already exists, do you want to"
                       "overwrite it?").arg(filePath),
                    QMessageBox::Yes | QMessageBox::Cancel
                    );
        // The user hit cancel, bail without creating a rule
        if(response != QMessageBox::Yes)
            return;
    }

    file.open(QFile::ReadWrite);
    file.write(QVariant(
                   QString("/* Rule: %1, created at: %2").arg(
                       name,
                       QDateTime::currentDateTime().toString("dd/MM/YYYY hh:mm:ss")
                       )
                   ).toByteArray());

    // Add the resulting file
    addRule(filePath);
}

void Project::newProgram(const QString &name)
{
    // Have we been passed an absolute path?
    //! \todo handle this

    // Get the directory in which we should be creating this rule
    QDir d = programsDir();

    // Check if a rule with this name already exists
    QString filePath = d.filePath(name + GP_PROGRAM_EXTENSION);
    QFile file(filePath);

    if(file.exists())
    {
        QMessageBox::StandardButton response;
        response = QMessageBox::warning(
                    0,
                    tr("File Exists"),
                    tr("A file called \"%1\" already exists, do you want to"
                       "overwrite it?").arg(filePath),
                    QMessageBox::Yes | QMessageBox::Cancel
                    );
        // The user hit cancel, bail without creating a program
        if(response != QMessageBox::Yes)
            return;
    }

    file.open(QFile::ReadWrite);
    file.write(QVariant(
                   QString("/* Program: %1, created at: %2").arg(
                       name,
                       QDateTime::currentDateTime().toString("dd/MM/YYYY hh:mm:ss")
                       )
                   ).toByteArray());

    // Add the resulting file
    addProgram(filePath);
}

void Project::newGraph(const QString &name, GraphTypes type)
{
    // Have we been passed an absolute path?
    //! \todo handle this

    // Get the directory in which we should be creating this rule
    QDir d = graphsDir();

    // Determine the type the graph should be
    switch(type)
    {
    case DotGraph:
    case GxlGraph:
        // Already specified, move on
        break;
    case DefaultGraph:
    default:
        // Check if one is implied by the extension
        if(name.endsWith(".dot"))
            type = DotGraph;
        else if(name.endsWith(".gxl"))
            type = GxlGraph;
        else
            type = DEFAULT_GRAPH_FORMAT;
    }

    // Compute the correct file path for this input and graph type
    QString filePath;
    if(name.endsWith(".dot") || name.endsWith(".gxl"))
    {
        filePath = d.filePath(name);
    }
    else
    {
        switch(type)
        {
        case GxlGraph:
            filePath = d.filePath(name + GP_GRAPH_GXL_EXTENSION);
            break;
        case DotGraph:
        default:
            filePath = d.filePath(name + GP_GRAPH_DOT_EXTENSION);
            break;
        }
    }
    QFile file(filePath);

    // Check if a rule with this name already exists
    if(file.exists())
    {
        QMessageBox::StandardButton response;
        response = QMessageBox::warning(
                    0,
                    tr("File Exists"),
                    tr("A file called \"%1\" already exists, do you want to"
                       "overwrite it?").arg(filePath),
                    QMessageBox::Yes | QMessageBox::Cancel
                    );
        // The user hit cancel, bail without creating a program
        if(response != QMessageBox::Yes)
            return;
    }

    file.open(QFile::ReadWrite);

    switch(type)
    {
    case GxlGraph:
        file.write(QVariant(
                       QString("<graph></graph>").arg(
                           name,
                           QDateTime::currentDateTime().toString("dd/MM/YYYY hh:mm:ss")
                           )
                       ).toByteArray());
        break;
    case DotGraph:
    default:
        file.write(QVariant(
                       QString("").arg(
                           name,
                           QDateTime::currentDateTime().toString("dd/MM/YYYY hh:mm:ss")
                           )
                       ).toByteArray());
        break;
    }

    // Add the resulting file
    addProgram(filePath);
}

void Project::addRule(const QString &filePath)
{
    // Don't add non-existent files
    QFile f(filePath);
    if(!f.exists())
        return;

    // Don't re-add files which are already part of the project
    if(containsFile(filePath))
    {
        QMessageBox::warning(
                    0,
                    tr("File Already Present"),
                    tr("The file you have asked GP Developer to add to this "
                       "project (%1) is already being tracked. GP Developer "
                       "has not added the file again.").arg(filePath),
                    QMessageBox::Ok
                    );
        return;
    }

    Rule *rule = new Rule(filePath, this);
    _rules.push_back(rule);
    save();
}

void Project::addProgram(const QString &filePath)
{
    // Don't add non-existent files
    QFile f(filePath);
    if(!f.exists())
        return;

    // Don't re-add files which are already part of the project
    if(containsFile(filePath))
    {
        QMessageBox::warning(
                    0,
                    tr("File Already Present"),
                    tr("The file you have asked GP Developer to add to this "
                       "project (%1) is already being tracked. GP Developer "
                       "has not added the file again.").arg(filePath),
                    QMessageBox::Ok
                    );
        return;
    }

    Program *program = new Program(filePath, this);
    _programs.push_back(program);
    save();
}

void Project::addGraph(const QString &filePath)
{
    // Don't add non-existent files
    QFile f(filePath);
    if(!f.exists())
        return;

    // Don't re-add files which are already part of the project
    if(containsFile(filePath))
    {
        QMessageBox::warning(
                    0,
                    tr("File Already Present"),
                    tr("The file you have asked GP Developer to add to this "
                       "project (%1) is already being tracked. GP Developer "
                       "has not added the file again.").arg(filePath),
                    QMessageBox::Ok
                    );
        return;
    }

    Graph *graph = new Graph(filePath, this);
    _graphs.push_back(graph);
    save();
}

void Project::setCurrentFile(const QString &fileName, FileTypes type)
{

}

bool Project::containsFile(const QString &filePath)
{
    return (containsGraph(filePath) || containsProgram(filePath)
                || containsRule(filePath));
}

bool Project::containsGraph(const QString &filePath)
{
    // We don't contain non-existent files
    QFileInfo file1(filePath);
    if(!file1.exists())
        return false;

    for(graphIter iter = _graphs.begin(); iter != _graphs.end(); ++iter)
    {
        Graph *g = *iter;
        QFileInfo file2(g->path());

        // Both files exist, so we compare their final paths
        if(file2.exists())
        {
            if(file1.absoluteFilePath() == file2.absoluteFilePath())
                return true;
        }
    }

    // If we didn't find it before, it's not there
    return false;
}

bool Project::containsRule(const QString &filePath)
{
    // We don't contain non-existent files
    QFileInfo file1(filePath);
    if(!file1.exists())
        return false;

    for(ruleIter iter = _rules.begin(); iter != _rules.end(); ++iter)
    {
        Rule *g = *iter;
        QFileInfo file2(g->path());

        // Both files exist, so we compare their final paths
        if(file2.exists())
        {
            if(file1.absoluteFilePath() == file2.absoluteFilePath())
                return true;
        }
    }

    // If we didn't find it before, it's not there
    return false;
}

bool Project::containsProgram(const QString &filePath)
{
    // We don't contain non-existent files
    QFileInfo file1(filePath);
    if(!file1.exists())
        return false;

    for(programIter iter = _programs.begin(); iter != _programs.end(); ++iter)
    {
        Program *g = *iter;
        QFileInfo file2(g->path());

        // Both files exist, so we compare their final paths
        if(file2.exists())
        {
            if(file1.absoluteFilePath() == file2.absoluteFilePath())
                return true;
        }
    }

    // If we didn't find it before, it's not there
    return false;
}

bool Project::save()
{
    return false;
}

bool Project::saveAs(const QString &filePath)
{
    // Update the filesystem watcher
    if(!GPFile::saveAs(filePath))
        return false;

    QFileInfo info(filePath);
    if(!info.dir().exists())
    {
        QMessageBox::StandardButton response;
        response = QMessageBox::warning(
                    0,
                    tr("Directory Not Found"),
                    tr("The directory specified (%1) does not exist. Create it?"
                       ).arg(info.dir().path()),
                    QMessageBox::Yes | QMessageBox::Cancel
                    );
        // The user hit cancel, bail without changing anything
        if(response != QMessageBox::Yes)
            return false;
    }

    //! \todo Decide whether this should convert all paths to absolute ones
    //!     based on the old project location, if it should transfer across all
    //!     of the relative files which are within the project directory, or if
    //!     it should offer a combination of these as a user choice

    _path = filePath;
    return save();
}

bool Project::saveFile(QString filePath)
{
    return false;
}

bool Project::saveFileAs(QString filePath)
{
    return false;
}

bool Project::saveAll()
{
    return false;
}

void Project::fileModified(QString filePath)
{
    // This shouldn't happen, but just in case
    if(!containsFile(filePath))
        return;
}

}
