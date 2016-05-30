#pragma once

#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
#	ifdef SAMCHON_FRAMEWORK_EXPORT
#		define SAMCHON_FRAMEWORK_API __declspec(dllexport)
#		define SAMCHON_FRAMEWORK_EXTERN
#	else
#		define SAMCHON_FRAMEWORK_API __declspec(dllimport)
#		define SAMCHON_FRAMEWORK_EXTERN extern
#	endif
#else
#	define SAMCHON_FRAMEWORK_API
#	define SAMCHON_FRAMEWORK_EXTERN
#endif

/* -------------------------------------------------------------------------
	DISABLE WARNINGS
------------------------------------------------------------------------- */
// MACRO RE-DEFINITION
#pragma warning(disable: 4005)

// PRE-DEFINED POINTER'S DELETION
#pragma warning(disable: 4150)

// DIAMOND INHERITANCE
#pragma warning(disable: 4250)

// TEMPLATE DLL
#pragma warning(disable: 4251)

// INHERITED NON-DLL
#pragma warning(disable: 4275)

// TYPEDEF TEMPLATE'S DEFAULT PARAMETER
#pragma warning(disable: 4348)