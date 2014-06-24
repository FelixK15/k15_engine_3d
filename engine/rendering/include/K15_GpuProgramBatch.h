/**
 * @file K15_GpuProgramBatch.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/05
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

#ifndef _K15Engine_Renderer_GpuProgramBatch_h_
#define _K15Engine_Renderer_GpuProgramBatch_h_


#include "K15_Prerequisites.h"
#include "K15_GpuProgram.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	class K15_CORE_API GpuProgramBatchImplBase : public RenderingAllocatedObject
	{
	public:
		GpuProgramBatchImplBase();
		virtual ~GpuProgramBatchImplBase();

		virtual bool compile() = 0;

		INLINE GpuProgramBatch* getProgramBatch() const;
		INLINE void setProgramBatch(GpuProgramBatch* p_Batch);

	protected:
		GpuProgramBatch* m_ProgramBatch;
	};
	/*********************************************************************************/
	class K15_CORE_API GpuProgramBatch : public RenderingAllocatedObject
	{
	public:
		typedef DynamicArray(GpuProgram*) GpuProgramList;

	public:
		GpuProgramBatch();
		GpuProgramBatch(const GpuProgramList& p_Programs);
		~GpuProgramBatch();

		bool compile();
		bool hasGpuProgramStage(Enum p_ProgramStage);

		INLINE void setError(const String& p_Error);

		INLINE const String& getError() const;
		INLINE const GpuProgramList& getGpuProgramList() const;

		GpuProgram* getGpuProgramByStage(Enum p_ProgramStage);
		void addGpuProgram(GpuProgram* p_Program, bool p_OverwriteProgramWithSameState = false);
		void removeGpuProgram(Enum p_ProgramStage);

		INLINE GpuProgramBatchImplBase* getImpl() const;
		INLINE void setImpl(GpuProgramBatchImplBase* p_Impl);

	private:
		GpuProgramBatchImplBase* m_Impl;
		GpuProgramList m_Programs;
		String m_Error;
		bool m_Compiled;
	}; //GpuProgramBatch class declaration
#	include "K15_GpuProgramBatch.inl"
}} // end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_GpuProgramBatch_h_