/**
 * @file K15_JobBase.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/26
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Core_JobBase_h_
#define _K15Engine_Core_JobBase_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_HashedString.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core {
	class K15_CORE_API JobBase
	{
	public:
		enum eJobStatus
		{
			JS_UNITITIALIZED,
			JS_QUEUED,
			JS_RUNNING,
			JS_FINISHED
		};//JobStatus

	public:
		JobBase(const ObjectName& p_Name);
		~JobBase();

		void execute();

		inline Enum getStatus() const;
		inline void setStatus(Enum p_Status);

		inline const ObjectName& getName() const;

	protected:
		virtual Enum internalExecute() = 0;

	private:
		ObjectName m_Name;
		Enum m_Status;
	};// end of * class declaration
#include "K15_JobBase.inl"
}}// end of K15_Engine::Core namespace

#endif //_K15Engine_Core_JobBase_h_