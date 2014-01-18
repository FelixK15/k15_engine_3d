defaultTypes = {
	"char","int8","uint8",
	"short","int16","uint16",
	"int","int32","uint32","Enum",
	"long","int64","uint64"
}

function getTypeInfo(line)
	local evaluateResult = {}
	
	for key, value in pairs(defaultTypes) do
		if string.find(line,value) then
			evaluateResult["defaultType"] = true
			evaluateResult["typeName"] = value
		else
			evaluateResult["defaultType"] = false
		end
		
		if string.find(line,"*") then
			evaluateResult["pointer"] = true;
		else
			evaluateResult["pointer"] = false;
		end
		
	end

	return evaluateResult
end