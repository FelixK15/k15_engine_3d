/**
 * @file K15_ResourceImporterFont.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/05/13
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

#include "K15_PrecompiledHeader.h"

#include "K15_Font.h"
#include "K15_ResourceImporterFont.h"
#include "K15_ResourceManager.h"
#include "K15_Texture.h"

#include "json/reader.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	ResourceImporterFont::ResourceImporterFont()
		: ResourceImporterBase("Font")
	{

	}
	/*********************************************************************************/
	ResourceImporterFont::~ResourceImporterFont()
	{

	}
	/*********************************************************************************/
	void ResourceImporterFont::getExtensionList(ExtensionSet& p_ExtensionSet)
	{
		p_ExtensionSet.insert(".font");
	}
	/*********************************************************************************/
	void ResourceImporterFont::getMagicNumber(MagicNumberSet& p_MagicNumber)
	{

	}
	/*********************************************************************************/
	ResourceBase* ResourceImporterFont::_load(const RawData& p_ResourceData, const Rtti& p_ResourceType)
	{
		Font* font = 0;
		if(p_ResourceType.isInstanceOf(Font::TYPE))
		{
			StringStream content;

			content.write((const char*)p_ResourceData.data,p_ResourceData.size);

			Json::Value root;
			Json::Reader reader;

			if(reader.parse((std::istream&)content,root,false))
			{
				font = K15_NEW Font();
				String name = root["Name"].asString();
				String texture = root["Texture"].asString();
				Texture* fontTexture = g_ResourceManager->getResource<Texture>(texture);

				fontTexture->setTextureSamplerSlot(Texture::TS_SLOT8);

				font->setName(name);
				font->setTexture(fontTexture);

				int8 counter = 0;
				float posX = 0.0f;
				float posY = 0.0f;
				#pragma message("Temporary hard coded")
				for(uint32 y = 0;y < 256;y+=32)
				{
					for(uint32 x = 0;x < 512;x+=32, ++counter, posX += (32.0f / 512.f))
					{
						Font::Letter letter;
						letter.character = ' ' + counter;
						letter.left = posX;
						letter.top = posY;
						letter.right = posX + (32.0f / 512.f);
						letter.bottom = posY + (32.0f / 256.f);

						font->addLetter(letter);
					}

					posX = 0.0f;
					posY += (32.0f / 256.f);
				}
			}
			else
			{
				setError(reader.getFormattedErrorMessages());
			}
		}

		return font;
	}
}}// end of K15_Engine::Core namespace
