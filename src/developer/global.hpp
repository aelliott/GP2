#ifndef GLOBAL_HPP
#define GLOBAL_HPP

// Forward declaration for version <-> string conversions
class QString;

namespace Developer {

/*!
 * \brief The GPVersions enum specifies the types of GP program allowed
 *
 * This enumeration exists to allow for extension of the editor to new as
 * yet unspecified versions of GP by allowing them to be defined as a new
 * enumeration value. This means that if for example the language were
 * experimentally altered to include colouring semantics then a value such
 * as ColouredGP2 could be inserted to handle this.
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
 * \brief GPVersionToString
 * \param version
 * \return
 */
const QString GPVersionToString(GPVersions version);

/*!
 * \brief stringToGPVersion
 * \param version
 * \return
 */
GPVersions stringToGPVersion(const QString &version);

/*!
 * Describes the format the graph should be expressed in.
 */
enum GraphTypes
{
    //! Default format: use the default defined in the program settings
    DefaultGraph,
    //! Dot format
    DotGraph,
    //! GXL (Graph eXchange Language) format
    GxlGraph
};

//! The default graph type to use (before set in QSettings)
#define DEFAULT_GRAPH_FORMAT DotGraph

/*!
 * The current version of this software - it is important that this remains a
 * value which can be directly replaced as a constructor parameter to QVariant
 * as some portions of the codebase (notably Project) assume that it is possible
 * to run QVariant(GP_DEVELOPER_VERSION).toString().
 */
#define GP_DEVELOPER_VERSION 0.1

//! The default version of GP the system should assume
#define DEFAULT_GP_VERSION RootedGP2

//! The default extension for GP rules
#define GP_RULE_EXTENSION ".gpr"

//! The default extension for GP programs
#define GP_PROGRAM_EXTENSION ".gpx"

//! The default extension for GP projects
#define GP_PROJECT_EXTENSION ".gpp"

//! The default extensions for GP graphs
#define GP_GRAPH_DOT_EXTENSION ".dot"
#define GP_GRAPH_GXL_EXTENSION ".gxl"

}

#endif // GLOBAL_HPP
