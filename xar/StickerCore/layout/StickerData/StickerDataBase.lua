local function SDBPrint(str) XLPrint("[Sticker][StickerDataBase] " .. tostring(str)) end

function Open(helperObj)
	local fileSystem = XLGetObject("Sticker.Helper.FileSystem")
	local exeDir = fileSystem:GetCurrentExeDir()
	local xmlPath = fileSystem:PathCombine(exeDir, "Profiles\\sticker_data.xml")
	if not fileSystem:IsFileExist(xmlPath) then
		SDBPrint("Open Failed: sticker_data.xml not exist!")
		return
	end
	
	-- 解析 xml 使用 xml 中的数据初始化 DataTable
	local xmlHelper = XLGetObject("Sticker.Helper.Xml")
	local xmlTable = xmlHelper:GetXml(xmlPath)
	if xmlTable ~= nil and type(xmlTable) == "table" and xmlTable["stick_data"] ~= nil and type(xmlTable["stick_data"]) == "table" then
		if xmlTable["stick_data"]["stick_list"] ~= nil and type(xmlTable["stick_data"]["stick_list"]) == "table" then
			
		end
	end
end

function Close()

end

function AddStick()
end

function DelStick()
end

function SetStick()
end

function GetStick()
end

function GetAllStick()
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

	XLSetGlobal("Sticker.StickerDataHelper", helperObj)
end