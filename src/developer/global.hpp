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
 * \brief GPVersionToString converts from an enum value to a storable string
 *  representation
 * \param version   The version to express as a string
 * \return  A string representing the version provided
 */
const QString GPVersionToString(GPVersions version);

/*!
 * \brief stringToGPVersion converts from a stored string representation to an
 *  enum value
 * \param version   The version to express as an enum value
 * \return  The enumerator value from GPVersions which represents the provided
 *  string
 */
GPVersions stringToGPVersion(const QString &version);

/*!
 * \brief The GraphTypes enum describes the formats that graphs can be stored in
 *
 * This covers output/input types such as Dot and GXL
 */
enum GraphTypes
{
    //! Default format: use the default defined in the program settings
    DefaultGraph,
    //! Dot format
    DotGraph,
    //! GXL (Graph eXchange Language) format
    GxlGraph,
    //! The "alternative" graph format documented in the GP2 design documents
    AlternativeGraph
};

/*!
 * \brief The RuleTypes enum defines the input and output types for rules
 * \sa ProgramTypes
 */
enum RuleTypes
{
    //! GP2 XML rule format
    RuleXmlFormat,
    //! GP2 "alternative" rule format
    RuleAlternativeFormat
};

/*!
 * \brief The ProgramTypes enum defines the input and output types for programs
 * \sa RuleTypes
 */
enum ProgramTypes
{
    //! GP2 XML program format
    ProgramXmlFormat,
    //! GP2 "alternative" program format
    ProgramAlternativeFormat
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
