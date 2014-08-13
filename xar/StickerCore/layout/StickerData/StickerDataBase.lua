local function SDBPrint(str) XLPrint("[Sticker][StickerDataBase] " .. tostring(str)) end

function Open(helperObj)
	SDBPrint("Open Enter")
	
	local fileSystem = XLGetObject("Sticker.Helper.FileSystem")
	local exeDir = fileSystem:GetCurrentExeDir()
	local xmlPath = fileSystem:PathCombine(exeDir, "Profiles\\sticker_data.xml")
	if not fileSystem:IsFileExist(xmlPath) then
		SDBPrint("Open Failed: sticker_data.xml not exist!")
		return
	end
	
	-- 解析 xml , 使用 xml 中的数据初始化 DataTable
	local xmlHelper = XLGetObject("Sticker.Helper.Xml")
	local xmlTable = xmlHelper:GetXml(xmlPath)
	if xmlTable ~= nil and type(xmlTable) == "table" and xmlTable["stick_data"] ~= nil and type(xmlTable["stick_data"]) == "table" then
		local xmlStickList = xmlTable["stick_data"]["stick_list"]
		local xmlRecycleList = xmlTable["stick_data"]["recycle_list"]
		if xmlStickList ~= nil and type(xmlStickList) == "table" then
			for key, data in pairs(xmlStickList) do
				if type(data) == "table" then
					local item = {}
					item.stick_id = data.stick_id
					item.stick_time = data.stick_time
					item.stick_color = data.stick_color
					item.stick_text = data.stick_text or ""
					local l,t,r,b = string.match(tostring(data.stick_pos), "(%d+),(%d+),(%d+),(%d+)")
					item.pos_left = tonumber(l)
					item.pos_top = tonumber(t)
					item.pos_right = tonumber(r)
					item.pos_bottom = tonumber(b)
					table.insert(helperObj.DataTable["stick_list"], item)
				end
			end
		end
		if xmlRecycleList ~= nil and type(xmlRecycleList) == "table" then
			for key, data in pairs(xmlRecycleList) do
				if type(data) == "table" then
					local item = {}
					item.stick_id = data.stick_id
					item.stick_time = data.stick_time
					item.stick_color = data.stick_color
					item.stick_text = data.stick_text or ""
					local l,t,r,b = string.match(tostring(data.stick_pos), "(%d+),(%d+),(%d+),(%d+)")
					item.pos_left = tonumber(l)
					item.pos_top = tonumber(t)
					item.pos_right = tonumber(r)
					item.pos_bottom = tonumber(b)
					table.insert(helperObj.DataTable["recycle_list"], item)
				end
			end
		end
	end
end

function Close(helperObj)
	SDBPrint("Close Enter")
	-- 将 DataTable 中的内容保存到 xml 中
	local fileSystem = XLGetObject("Sticker.Helper.FileSystem")
	local exeDir = fileSystem:GetCurrentExeDir()
	local xmlDir = fileSystem:PathCombine(exeDir, "Profiles")
	if not fileSystem:IsDirExist(xmlDir) then
		fileSystem:MakeDir(xmlDir)
	end
	local xmlPath = fileSystem:PathCombine(xmlDir, "sticker_data.xml")
	local xmlTable = {}
	xmlTable["stick_data"] = {}
	xmlTable["stick_data"]["stick_list"] = {}
	xmlTable["stick_data"]["recycle_list"] = {}
	for key, value in ipairs(helperObj.DataTable["stick_list"]) do
		local item = {}
		item["item"] = {}
		item["item"]["stick_id"] = value.stick_id
		item["item"]["stick_time"] = value.stick_time
		item["item"]["stick_color"] = value.stick_color
		item["item"]["stick_text"] = value.stick_text
		item["item"]["stick_pos"] = value.pos_left .. "," .. value.pos_top .. "," .. value.pos_right .. "," .. value.pos_bottom
		table.insert(xmlTable["stick_data"]["stick_list"], item)
	end
	for key, value in ipairs(helperObj.DataTable["recycle_list"]) do
		local item = {}
		item["item"] = {}
		item["item"]["stick_id"] = value.stick_id
		item["item"]["stick_time"] = value.stick_time
		item["item"]["stick_color"] = value.stick_color
		item["item"]["stick_text"] = value.stick_text
		item["item"]["stick_pos"] = value.pos_left .. "," .. value.pos_top .. "," .. value.pos_right .. "," .. value.pos_bottom
		table.insert(xmlTable["stick_data"]["recycle_list"], item)
	end
	local xmlHelper = XLGetObject("Sticker.Helper.Xml")
	xmlHelper:SetXml(xmlPath, xmlTable)
end

function AddStick(helperObj, text, l, t, r, b, color)
	SDBPrint("AddStick text = " .. tostring(text) .. ", l = " .. tostring(l) .. ",t = " .. tostring(t) .. ", r = " .. tostring(r) .. ", b = " .. tostring(b) .. ", color = " .. color )

	text = text or ""
	l = l or 0
	t = t or 0
	r = r or 100
	b = b or 100

	local osshell = XLGetObject("Sticker.Helper.OSShell")
	local localTime = osshell:GetLocalTime()
	local stick_id = os.time() .. tostring(localTime.milliseconds)    -- id 要用毫秒表示，防止发生冲突
	local stick_time = string.format("%d,%d,%d;%d:%d:%d:%d", localTime.year, localTime.month, localTime.day, localTime.hour, localTime.min, localTime.second, localTime.milliseconds)
	local item = {}
	item.stick_id = stick_id
	item.stick_color = color
	item.stick_time = stick_time
	item.stick_text = text
	item.pos_left = l
	item.pos_top = t
	item.pos_right = r
	item.pos_bottom = b
	table.insert(helperObj.DataTable["stick_list"], item)
	
	SDBPrint("AddStick stick_id = " .. tostring(stick_id))
	return stick_id
end

function DelStick(helperObj, stick_id)
	SDBPrint("DelStick stick_id = " .. tostring(stick_id))
	
	if stick_id == nil then
		return 
	end
	for index, data in ipairs(helperObj.DataTable["stick_list"]) do
		if data.stick_id == stick_id then
			table.insert(helperObj.DataTable["recycle_list"], data)
			table.remove(helperObj.DataTable["stick_list"], index)
			break
		end
	end
end

function SetStick(helperObj, stick_id, text, l, t, r, b, color)
	SDBPrint("SetStick stick_id = " .. tostring(stick_id) .. ", text = " .. tostring(text) .. ", l = " .. tostring(l) .. ",t = " .. tostring(t) .. ", r = " .. tostring(r) .. ", b = " .. tostring(b) .. ", color = " .. tostring(color) )
	
	if stick_id == nil then
		return
	end
	for index, data in ipairs(helperObj.DataTable["stick_list"]) do
		if data.stick_id == stick_id then
			data.stick_text = text or data.stick_text
			data.stick_color = color or data.stick_color
			data.pos_left = l or data.pos_left
			data.pos_top = t or data.pos_top
			data.pos_right = r or data.pos_right
			data.pos_bottom = b or data.pos_bottom
			break
		end
	end
end

function GetStick(helperObj, stick_id)
	SDBPrint("GetStick stick_id = " .. tostring(stick_id))
	if stick_id == nil then
		return
	end
	for index, data in ipairs(helperObj.DataTable["stick_list"]) do
		if data.stick_id == stick_id then
			local item = {}
			item.id = stick_id
			item.time = data.stick_time
			item.color = data.stick_color
			item.text = data.stick_text
			item.left = data.pos_left
			item.top = data.pos_top
			item.right = data.pos_right
			item.bottom = data.pos_bottom
			return item
		end
	end
end

function GetAllStick(helperObj)
	SDBPrint("GetAllStick Enter")
	local sticklist = {}
	for index, data in ipairs(helperObj.DataTable["stick_list"]) do
		table.insert(sticklist, data.stick_id)
	end
	return sticklist
end

function RegisterObject()
	local helperObj = {}
	helperObj.Open = Open
	helperObj.Close = Close
	helperObj.AddStick = AddStick
	helperObj.DelStick = DelStick
	helperObj.SetStick = SetStick
	helperObj.GetStick = GetStick
	helperObj.GetAllStick = GetAllStick
	
	-- 使用 DataTable 表缓存数据，Open 时初始化 DataTable 表，Close 时将表中数据保存到文件
	-- DataTable 表结构：
	-- DataTable["stick_list"]
	-- DataTable["stick_list"][1]["stick_id"]
	-- DataTable["stick_list"][1]["stick_time"]
	-- DataTable["stick_list"][1]["stick_color"]
	-- DataTable["stick_list"][1]["stick_text"]
	-- DataTable["stick_list"][1]["pos_top"]
	-- DataTable["stick_list"][1]["pos_left"]
	-- DataTable["stick_list"][1]["pos_right"]
	-- DataTable["stick_list"][1]["pos_bottom"]
	-- DataTable["recycle_list"]
	-- DataTable["recycle_list"][1]["stick_id"]
	-- DataTable["recycle_list"][1]["stick_time"]
	-- DataTable["recycle_list"][1]["stick_color"]
	-- DataTable["recycle_list"][1]["stick_text"]
	-- DataTable["recycle_list"][1]["pos_top"]
	-- DataTable["recycle_list"][1]["pos_left"]
	-- DataTable["recycle_list"][1]["pos_right"]
	-- DataTable["recycle_list"][1]["pos_bottom"]
	helperObj.DataTable = {}
	helperObj.DataTable["stick_list"] = {}
	helperObj.DataTable["recycle_list"] = {}

	XLSetGlobal("Sticker.StickerDataBase", helperObj)
end