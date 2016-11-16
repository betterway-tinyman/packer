#pragma once

#include <samchon/library/IOperator.hpp>

/* -------------------------------------------------------------
	CRITICAL SECTION
------------------------------------------------------------- */
#include <samchon/library/CriticalAllocator.hpp>
#include <samchon/library/CriticalList.hpp>
#include <samchon/library/CriticalVector.hpp>
#include <samchon/library/CriticalTreeMap.hpp>
#include <samchon/library/CriticalHashMap.hpp>

#include <samchon/library/RWMutex.hpp>
#include <samchon/library/Semaphore.hpp>

/* -------------------------------------------------------------
	DATA
------------------------------------------------------------- */
#include <samchon/library/XML.hpp>
#include <samchon/library/Date.hpp>

//// HTTP
//#include <samchon/library/HTTPLoader.hpp>
//#include <samchon/library/URLVariables.hpp>
//
//// SQL
//#include <samchon/library/SQLi.hpp>
//#include <samchon/library/TSQLi.hpp>
//
//// SQL STATEMENT
//#include <samchon/library/SQLStatement.hpp>
//#include <samchon/library/TSQLStatement.hpp>

/* -------------------------------------------------------------
	UTILITY
------------------------------------------------------------- */
// STRING
#include <samchon/library/Base64.hpp>
#include <samchon/library/Charset.hpp>
#include <samchon/library/StringUtil.hpp>

// MATH
#include <samchon/library/Math.hpp>
#include <samchon/library/GeneticAlgorithm.hpp>
#include <samchon/library/GAPopulation.hpp>
#include <samchon/library/GAParameters.hpp>

// CASE_GENERATOR
#include <samchon/library/CaseGenerator.hpp>
#include <samchon/library/CombinedPermutationGenerator.hpp>
#include <samchon/library/PermutationGenerator.hpp>
#include <samchon/library/FactorialGenerator.hpp>