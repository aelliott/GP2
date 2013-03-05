#ifndef GLOBAL_HPP
#define GLOBAL_HPP

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

//! The current version of this software
#define GP_DEVELOPER_VERSION 0.1

//! The default version of GP the system should assume
#define DEFAULT_GP_VERSION RootedGP2

#endif // GLOBAL_HPP
