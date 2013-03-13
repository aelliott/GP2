#ifndef GPFILE_HPP
#define GPFILE_HPP

#include <QObject>
#include <QString>
#include <QFile>

/*!
 * \brief Abstract base class for GP project files
 *
 * The GPFile class provides a generalised interface for querying and changing
 * the state of files in a GP project. The class maintains a FileStatus member
 * variable which informs the rest of the system whether the file needs to be
 * saved, reloaded, etc.
 *
 * In addition it provides an interface for those tasks. The save() function
 * saves any pending changes to the file contained in _path (or prompts the user
 * for a location if one is not set), and saveAs() saves the file to a new
 * location given by its argument.
 */
class GPFile : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Enumeration covering the possible states of a file in the system
     */
    enum FileStatus
    {
        //! Normal, no changes
        Normal,
        //! File has been modified
        Modified,
        //! File has been modified externally
        ExternallyModified,
        //! File has been deleted / file not found
        Deleted,

        //! Error, unknown issue
        Error
    };

    /*!
     * \brief Construct a new GPFile with the given file path if provided
     * \param filePath  The path to this file on the filesystem
     * \param parent    This object's parent object
     */
    GPFile(const QString &filePath = QString(), QObject *parent = 0);

    /*!
     * \brief Get the current path of the active file
     * \return A string containing the path to the current file on the
     *  filesystem
     */
    QString path() const;

    /*!
     * \brief Get the current status of this file
     * \return The status of this file
     * \sa FileStatus
     */
    FileStatus status() const;

    /*!
     * \brief Save this file to the current active file
     *
     * If there is no current active file (in the case of a new unsaved file)
     * then prompt the user for a location to save this file.
     *
     * \return Boolean, true if saved successfully, false otherwise
     */
    virtual bool save() = 0;

    /*!
     * \brief Save this file to a new location
     * \param filePath  The new path to save this file to
     * \return Boolean, true if saved successfully, false otherwise
     */
    virtual bool saveAs(const QString &filePath) = 0;

    bool open();

protected:
    /*!
     * This is the path to the project file, the project directory is the
     * directory which contains this file and is therefore easily derived from
     * this variable.
     */
    QString _path;
    QFile *_fp;

    FileStatus _status;
};

#endif // GPFILE_HPP
