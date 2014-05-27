-- Description: getopt() translated from the BSD getopt(); compatible with the default Unix getopt()
--[[ Example:
	for o, a in os.getopt(arg, 'a:b') do
		print(o, a)
	end
]]--
function os.getopt(args, ostr)
	local arg, place = nil, 0;
	return function ()
		if place == 0 then -- update scanning pointer
			place = 1
			if #args == 0 or args[1]:sub(1, 1) ~= '-' then place = 0; return nil end
			if #args[1] >= 2 then
				place = place + 1
				if args[1]:sub(2, 2) == '-' then -- found "--"
					place = 0
					table.remove(args, 1);
					return nil;
				end
			end
		end
		local optopt = args[1]:sub(place, place);
		place = place + 1;
		local oli = ostr:find(optopt);
		if optopt == ':' or oli == nil then -- unknown option
			if optopt == '-' then return nil end
			if place > #args[1] then
				table.remove(args, 1);
				place = 0;
			end
			return '?';
		end
		oli = oli + 1;
		if ostr:sub(oli, oli) ~= ':' then -- do not need argument
			arg = nil;
			if place > #args[1] then
				table.remove(args, 1);
				place = 0;
			end
		else -- need an argument
			if place <= #args[1] then  -- no white space
				arg = args[1]:sub(place);
			else
				table.remove(args, 1);
				if #args == 0 then -- an option requiring argument is the last one
					place = 0;
					if ostr:sub(1, 1) == ':' then return ':' end
					return '?';
				else arg = args[1] end
			end
			table.remove(args, 1);
			place = 0;
		end
		return optopt, arg;
	end
end