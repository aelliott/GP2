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
    //! GP2
    GP2
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
    AlternativeGraph,
    //! A LaTeX output format
    LaTeXGraph
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

/*!
 * \brief A defined set of directions which a layout mechanism can work in
 *
 * This is primarily useful for the tree-based layout algorithms which can
 * orient the tree in any of four directions.
 */
enum LayoutDirections
{
    Layout_TopToBottom,
    Layout_RightToLeft,
    Layout_BottomToTop,
    Layout_LeftToRight
};

//! The default orientation to use
#define DEFAULT_LAYOUT_DIRECTION Layout_TopToBottom

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
#define DEFAULT_GP_VERSION GP2

//! The default extension for GP rules
#define GP_RULE_EXTENSION ".gpr"

//! The default extension for GP programs
#define GP_PROGRAM_EXTENSION ".gpx"

//! The default extension for GP projects
#define GP_PROJECT_EXTENSION ".gpp"

//! The default extensions for GP graphs
#define GP_GRAPH_ALTERNATIVE_EXTENSION ".gpg"
#define GP_GRAPH_DOT_EXTENSION ".gv"
#define GP_GRAPH_GXL_EXTENSION ".gxl"
#define GP_GRAPH_LATEX_EXTENSION ".tex"

//! The number of recent projects to track
#define MAX_RECENT_PROJECTS 5

//! Set a load of default fonts
#if defined(Q_OS_WIN)
#define EDITOR_DEFAULT_FONT QFont("Lucida Console",10)
#elif defined(Q_OS_MAC)
#define EDITOR_DEFAULT_FONT QFont("Monaco", 12)
#else
#define EDITOR_DEFAULT_FONT QFont("Bitstream Vera Sans Mono",9)
#endif

//! Default Z-index of nodes in the visualisation
#define NODE_Z_VALUE 20
//! Default Z-index of edges in the visualisation
#define EDGE_Z_VALUE 10

//! Defines whether or not debug information is shown in the visualisation view
#define SHOW_VISUALISATION_DEBUG false
#define DEBUG_COLOUR QColor(Qt::lightGray)

}

#endif // GLOBAL_HPP
