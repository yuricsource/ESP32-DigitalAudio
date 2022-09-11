//
// SingletonHolder.h
//
// $Id: //poco/1.4/Foundation/include/Poco/SingletonHolder.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  SingletonHolder
//
// Definition of the SingletonHolder template.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#pragma once

#include "mutex.hpp"

namespace Utilities {

using cpp_freertos::MutexStandard;
using cpp_freertos::LockGuard;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	This is a helper template class for managing singleton objects allocated on the heap.
/// 		The class ensures proper deletion (including calling of the destructor) of singleton
/// 		objects when the application that created them terminates.
///
/// @tparam	S	Class type that will be managed by this singleton holder.
////////////////////////////////////////////////////////////////////////////////////////////////////
template <class S>
class SingletonHolder
{
public:
	/// @brief	Creates the SingletonHolder.
	SingletonHolder()
		: _pS(0)
	{
	}
	
	/// @brief	Destroys the SingletonHolder and the singleton object that it holds.
	~SingletonHolder()
	{
		delete _pS;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Returns a pointer to the singleton object hold by the SingletonHolder. The first call
	/// 		to get will create the singleton.
	///
	/// @return	Null if it fails, else a pointer to a S.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	S* Get()
	{
		LockGuard guard(_m);
		if (!_pS) _pS = new S;
		return _pS;
	}
	
	/// @brief	Deletes the singleton object.
	void Reset()
	{
		LockGuard guard(_m);
		delete _pS;
		_pS = 0;
	}
	
private:
	S* _pS;
	MutexStandard _m;
};

} // namespace Utilities