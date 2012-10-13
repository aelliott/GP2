#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <QVector>
#include <QFile>
#include <QFileSystemWatcher>
#include <QObject>
#include <QString>

/*!
 * \brief Container type for GP projects, allowing for monitoring and updating
 *      project files.
 *
 * The general structure of projects is as follows:
 *
 * \code
 *  Project:
 *      - project file (.gpproj)
 *      - rules:
 *          - rule files (.gprule)
 *      - programs:
 *          - program files (.gpprog)
 *      - graphs
 *          - graph files (.dot, .gxl)
 * \endcode
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

    bool initProject();

    void setCurrentFile(const QString &fileName, FileTypes type);

    bool save(QString file = QString());
    bool saveAs(QString file = QString());
    bool saveAll();

    bool import();
    bool import(QString file);

private:
    QString _projectPath;
    QFileSystemWatcher *_watcher;
    QFile _projectFile;
    QFile *_currentFile;
    QVector<QFile> _rules;
    QVector<QFile> _programs;
    QVector<QFile> _graphs;
};

#endif // PROJECT_HPP
