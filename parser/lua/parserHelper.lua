function cleanCPPSnippet(code) --removes cpp comments and several keywords
    local cleanCode = code
    cleanCode = string.gsub(cleanCode,"K15_CORE_API","")
    cleanCode = string.gsub(cleanCode,"//.-\n+","")
    cleanCode = string.gsub(cleanCode,"/%*.-%*/","");
    return cleanCode
end