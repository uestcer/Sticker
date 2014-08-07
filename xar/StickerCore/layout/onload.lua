local fileSystem = XLGetObject("Sticker.Helper.FileSystem")
local path = fileSystem:GetUpperDir(__document)
XLPrint("[Dongyu] path = " .. tostring(path))
XLLoadModule(path .. [[\StickerData\StickerDataBase.lua]])["RegisterObject"]()
XLLoadModule(path .. [[\StickerData\StickerDataHelper.lua]])["RegisterObject"]()
XLLoadModule(path .. [[\StickerView\StickerViewHelper.lua]])["RegisterObject"]()

local database = XLGetGlobal("Sticker.StickerDataBase")
database:Open()
local stick_id = database:AddStick("自己添加的Stick", 111,222,333,444,"redandblue")
local sticklist = database:GetAllStick()
for i,stick_id in ipairs(sticklist) do
	local stick = database:GetStick(stick_id)
	if stick_id == "stick_id1" then
		database:DelStick(stick_id)
	else
		database:SetStick(stick_id, "修改后的Stick", 555, 666, 777, 888, "SetStickColor")
	end
end
-- database:Close()

local path = fileSystem:PathCombine(__document, [[..\..\res\default\StikyNot.ico]])
local notifyHelper = XLGetObject("Sticker.Helper.NotifyIcon")
local uID = notifyHelper:AddIcon(path)
notifyHelper:Attach(function(uID, message)
	if message == "WM_LBUTTONDBLCLK" then
		frameHostWnd:BringWindowToTop()
	elseif message == "WM_RBUTTONDBLCLK" then
		local stickerCore = XLGetObject("Sticker.Core")
		stickerCore:ExitSticker()
	end
end)
local stickerCore = XLGetObject("Sticker.Core")
stickerCore:AttachListener(function()
	notifyHelper:DelIcon(uID)
end)