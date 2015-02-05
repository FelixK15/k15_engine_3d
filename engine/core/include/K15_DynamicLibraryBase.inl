/**
 * @file K15_DynamicLibrary.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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
 */

/*********************************************************************************/
template<class ReturnType>
Functor0<ReturnType> DynamicLibraryBase::getSymbol(const String& p_SymbolName)
{
	if (isLoaded())
	{
		void* symbol = getSymbolInternal(p_SymbolName);

		if(!symbol)
		{
			K15_LOG_ERROR_MESSAGE("Could not find symbol \"%s\" (library:\"%s\") Error:%s",p_SymbolName.c_str(),getFileName().c_str(),Application::getInstance()->getLastError().c_str());
		}
		else
		{
			Functor0<ReturnType> func((typename Functor0<ReturnType>::FunctionType)symbol);

			return func;
		}

	}
	else
	{
	    K15_LOG_NORMAL_MESSAGE("Trying to load symbol from already unloaded library - Symbol:\"%s\" (Library:\"%s\")",p_SymbolName.c_str(),getFileName().c_str());
	}

	return Functor0<ReturnType>();
}
/*********************************************************************************/