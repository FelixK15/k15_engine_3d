/**
 * @file K15_MaterialManager.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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

#ifndef _K15Engine_Rendering_MaterialManager_h_
#define _K15Engine_Rendering_MaterialManager_h_

#ifndef K15_USE_PRECOMPILED_HEADER
# include "K15_Prerequisites.h"
# include "K15_Singleton.h"
# include "K15_Application.h"
# include "K15_AllocatedObject.h"
#endif // K15_USE_PRECOMPILED_HEADER

#include "K15_PoolAllocator.h"
#include "K15_Material.h"
namespace K15_Engine { namespace Rendering {
  class K15_CORE_API MaterialManager : public Singleton<MaterialManager>, public ApplicationAllocatedObject, public PoolAllocator<K15_SIZE_MATERIAL>
  {
  public:
    /*********************************************************************************/
    typedef HashMap(ObjectName,MaterialPtr) MaterialMap;
    /*********************************************************************************/

  public:
    MaterialManager();
    ~MaterialManager();

    MaterialPtr loadMaterial(const ObjectName& p_MaterialName) const;
    
  private:
    MaterialMap m_MaterialMap;
  };// end of MaterialManager class declaration
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_MaterialManager_h_