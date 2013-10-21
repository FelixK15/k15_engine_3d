/**
 * @file K15_ApplicationParameter.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/10
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

#ifndef _K15Engine_Core_ApplicationParameter_h_
#define _K15Engine_Core_ApplicationParameter_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif// K15_USE_PRECOMPILED_HEADER

#define K15_SET_NUMERICAL_SETTING(parameter_variable,setting_name,data_type) \
	if(parameter_variable.Name == #setting_name)  \
		this->set ## setting_name(K15_Engine::System::StringUtil::stringToNumeric<data_type>(parameter_variable.Value))

#define K15_SET_BOOL_SETTING(parameter_variable,setting_name) \
	if(parameter_variable.Name == #setting_name)  \
		this->set ## setting_name(K15_Engine::System::StringUtil::toBool(parameter_variable.Value))

#define K15_SET_STRING_SETTING(parameter_variable,setting_name) \
	if(parameter_variable.Name == #setting_name)  \
		this->set ## setting_name(parameter_variable.Value)

namespace K15_Engine { namespace Core { 
	
	struct ApplicationParameter
	{
		String Group;
		String Name;
		String Value;
	};
}}//end of K15_Engine::System namespace

#endif //_K15Engine_Core_ApplicationParameter_h_