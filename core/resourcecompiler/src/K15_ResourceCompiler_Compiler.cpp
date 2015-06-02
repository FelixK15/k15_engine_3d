#include "K15_ResourceCompiler_Compiler.h"
#include "K15_ResourceCompiler_CompilerEnvironment.h"

#include "K15_ResourceCompiler_ResourceProcessorMesh.h"
#include "K15_ResourceCompiler_ResourceProcessorTexture.h"

#include "K15_ResourceCompiler_StringUtil.h"
#include "K15_ResourceCompiler_Logging.h"

#include "K15_ResourceCompiler_ResourceData.h"

#ifdef _WIN32
	#include "win32/K15_ResourceCompiler_Win32.h"
#endif

namespace K15_ResourceCompiler
{
	namespace internal
	{
		/*********************************************************************************/
		ResourceDataVector createResourceDatas(const StringSet& p_InputFiles, const ResourceProcessorVector& p_ResourceProcessor);
		ResourceDataVector mergeResourceDatas(const ResourceDataVector& p_InputResourceDatas);
		/*********************************************************************************/

	}

	/*********************************************************************************/
	Compiler::Compiler(const CompilerEnvironment p_Environment)
		: m_Environment(p_Environment),
		m_Error(),
		m_ReturnValue(0)
	{

	}
	/*********************************************************************************/
	Compiler::~Compiler()
	{

	}
	/*********************************************************************************/
	bool Compiler::compile()
	{
		if(m_Environment.getInputPath().empty())
		{
			m_Error = "No input file/dir defined.";
			m_ReturnValue = -5;

			return false;
		}
		
		if(m_Environment.getOutputPath().empty())
		{
			m_Error = "No output dir defined.";
			m_ReturnValue = -10;

			return false;
		}

		StringSet inputFiles = FileFinder::getFiles(m_Environment.getInputPath());

		if(inputFiles.size() == 0)
		{
			LOG_ERROR("Could not find file(s): %s", m_Environment.getInputPath().c_str());
			return false;
		}

		//gather all resource datas
		ResourceDataVector resourceDatas = internal::createResourceDatas(inputFiles, m_ResourceProcessors);

		//merge resource data (e.g. dependencies with real resource data)
		resourceDatas = internal::mergeResourceDatas(resourceDatas);

		for (ResourceDataVector::iterator resourceDataIter = resourceDatas.begin();
			 resourceDataIter != resourceDatas.end();
			 ++resourceDataIter)
		{
			ResourceData& currentResourceData = (*resourceDataIter);
			compileResourceData(currentResourceData);
		}
		
		return true;
	}
	/*********************************************************************************/
	const String& Compiler::getError() const
	{
		return m_Error;
	}
	/*********************************************************************************/
	int Compiler::getReturnValue() const
	{
		return m_ReturnValue;
	}
	/*********************************************************************************/
	bool Compiler::hasProcessor(const String& p_Name) const
	{
		for(std::vector<ResourceProcessorBase*>::const_iterator iter = m_ResourceProcessors.begin();
			iter != m_ResourceProcessors.end(); 
			++iter)
		{
			ProcessorEnvironment environment = (*iter)->getProcessorEnvironment();

			if(environment.processorName == p_Name)
			{
				return true;
			}
		}

		return false;
	}
	/*********************************************************************************/
	void Compiler::addProcessor(ResourceProcessorBase* p_ResourceProcessor)
	{
		assert(p_ResourceProcessor);

		m_ResourceProcessors.push_back(p_ResourceProcessor);
	}
	/*********************************************************************************/
	void Compiler::addDefaultProcessors()
	{
		if(!hasProcessor("Mesh Processor"))
		{
			addProcessor(new ResourceProcessorMesh(m_Environment));
		}

		if(!hasProcessor("Texture Processor"))
		{
			addProcessor(new ResourceprocessorTexture(m_Environment));
		}
	}
	/*********************************************************************************/
	bool Compiler::compileResourceData(const ResourceData& p_ResourceData)
	{
		ProcessorEnvironment processorSettings;

		for(std::vector<ResourceProcessorBase*>::iterator iter = m_ResourceProcessors.begin();
			iter != m_ResourceProcessors.end(); ++iter)
		{
			ResourceProcessorBase* processor = (*iter);
			String extension = StringUtil::getFileExtension(p_ResourceData.filePath);

			//Check if the processor can actually process the current asset (file extension will be checked)
			if(processor->canProcess(extension))
			{
				processorSettings = processor->getProcessorEnvironment();

				String expectedFileName = StringUtil::generateExpectingOutputFilePath(p_ResourceData.filePath, processorSettings);

				//Check if file already exists and if we shouls override the existing file or not.
				if(FileFinder::fileExisting(expectedFileName)
					&& m_Environment.getFileCollisionBehavior() == FEB_DONT_REPLACE_EXISTING_FILE)
				{
					LOG_WARNING("Ignoring \"%s\" because it already exists...", expectedFileName.c_str());
				}
				else
				{
					LOG_NORMAL("Processing \"%s\" ...", p_ResourceData.filePath.c_str());

					ResourceProcessResult processResult = processor->process(p_ResourceData);

					if(processResult.processState == RPS_FAILURE)
					{
						LOG_ERROR("Error processing \"%s\" with processor \"%s\"",
							p_ResourceData.filePath.c_str(), processorSettings.processorName.c_str(),
							processor->getError().c_str());

						return false;
					}
					else if(processResult.processState == RPS_SUCCESS)
					{
						LOG_NORMAL("Successfully processed \"%s\" with processor \"%s\"",
							p_ResourceData.filePath.c_str(), processorSettings.processorName.c_str(),
							processor->getError().c_str());
					}
				}
			}
		}

		return true;
	}
	/*********************************************************************************/

	

	/*********************************************************************************/
	ResourceDataVector internal::createResourceDatas(const StringSet& p_InputFiles, const ResourceProcessorVector& p_ResourceProcessor)
	{
		ResourceDataVector returnData;
		ResourceProcessorBase* resourceProcessor = 0;
		String inputFile;
		String inputFileExtension;

		for (ResourceProcessorVector::const_iterator resourceProcessorIter = p_ResourceProcessor.begin();
			 resourceProcessorIter != p_ResourceProcessor.end();
			 ++resourceProcessorIter)
		{
			resourceProcessor = (*resourceProcessorIter);

			for (StringSet::const_iterator inputFileIter = p_InputFiles.begin();
				inputFileIter != p_InputFiles.end();
				++inputFileIter)
			{
				inputFile = (*inputFileIter);
				inputFileExtension = StringUtil::getFileExtension(inputFile);

				if (resourceProcessor->canProcess(inputFileExtension))
				{
					ResourceData inputFileData = resourceProcessor->gatherData(inputFile);
					returnData.push_back(inputFileData);
				}
			}
		}
		
		return returnData;
	}
	/*********************************************************************************/
	ResourceDataVector internal::mergeResourceDatas(const ResourceDataVector& p_InputResourceData)
	{
		ResourceDataVector outputResourceData;
		ResourceData currentResourceData = {};

		for (ResourceDataVector::const_iterator resourceDataIter = p_InputResourceData.begin();
			 resourceDataIter != p_InputResourceData.end();
			 ++resourceDataIter)
		{
			currentResourceData = (*resourceDataIter);

			ResourceDataVector::const_iterator dependencyResource = std::find_if(currentResourceData.dependencies.begin(), 
																			  currentResourceData.dependencies.end(),
																			  [&currentResourceData](const ResourceData& p_ResourceData)->bool{
																				return currentResourceData.filePath == p_ResourceData.filePath;  
																			  });

			if (dependencyResource != currentResourceData.dependencies.end())
			{
				//merge data from dependency with data from "real" resource
				currentResourceData.parameters.insert(currentResourceData.parameters.begin(),
													  dependencyResource->parameters.begin(),
													  dependencyResource->parameters.end());
			}

			outputResourceData.push_back(currentResourceData);
		}

		return outputResourceData;
	}
	/*********************************************************************************/
}