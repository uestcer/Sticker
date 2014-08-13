local function SOPrint(str) XLPrint("[Sticker][onload] " .. tostring(str)) end

local fileSystem = XLGetObject("Sticker.Helper.FileSystem")
local path = fileSystem:GetUpperDir(__document)
XLLoadModule(path .. [[\StickerData\StickerDataHelper.lua]])["RegisterObject"]()
XLLoadModule(path .. [[\StickerView\StickerViewHelper.lua]])["RegisterObject"]()

-- 添加系统托盘图标
local path = fileSystem:PathCombine(__document, [[..\..\res\default\StikyNot.ico]])
local notifyHelper = XLGetObject("Sticker.Helper.NotifyIcon")
local uID = notifyHelper:AddIcon(path)
notifyHelper:Attach(function(uID, message)
	if message == "WM_LBUTTONDBLCLK" then
		local viewHelper = XLGetGlobal("Sticker.StickerViewHelper")
		local stickIdArray = viewHelper:GetAllStick()
		for i,stickId in ipairs(stickIdArray) do
			viewHelper:BringStickToTop(stickId)
		end
	elseif message == "WM_RBUTTONDBLCLK" then
		local stickerCore = XLGetObject("Sticker.Core")
		stickerCore:ExitSticker()
	elseif message == "WM_LBUTTONUP" then
		
	end
end)

local stickerCore = XLGetObject("Sticker.Core")
stickerCore:AttachListener(function()
	notifyHelper:DelIcon(uID)
end)

-- 如果 没有 Stick ,创建一个
local dataHelper = XLGetGlobal("Sticker.StickerDataHelper")
local viewHelper = XLGetGlobal("Sticker.StickerViewHelper")
local stickIdArray = dataHelper:GetAllStick()
if #stickIdArray == 0 then
	local osshell = XLGetObject("Sticker.Helper.OSShell")
	local screenWidth,screenHeight = osshell:GetScreenSize()
	screenWidth = screenWidth or 1080
	screenHeight = screenHeight or 720
	
	local text = " "
	local l = screenWidth - 188 - 100
	local t = 100
	local r = l + 188
	local b = t + 177
	local defaultcolor = "defaultcolor"
	dataHelper:AddStick(text,l,t,r,b,defaultcolor)
end

-- 读取 Stick 数据，显示到界面
local stickIdArray = dataHelper:GetAllStick()
for i, stickId in ipairs(stickIdArray) do
	local stickData = dataHelper:GetStick(stickId)
	SOPrint(stickData.id)
	if stickData then
		viewHelper:AddStick(stickData.id, stickData.text, stickData.color, stickData.left, stickData.top, stickData.right, stickData.bottom)
	end
end

-- Attach 界面事件
viewHelper:AttachListener(function(eventName, viewData)
	if viewData == nil then
		return
	end
	SOPrint("AttachListener eventName = " .. tostring(eventName) .. ",viewData = " .. tostring(viewData.id))
	if eventName == "OnAddBtnClick" then
		local stickData = dataHelper:GetStick(viewData.id)
		
		local text = ""
		local l = stickData.left - 188 - 10
		local t = stickData.top
		local r = l + 188
		local b = t + 177
		local defaultcolor = "defaultcolor"
		local newStickId = dataHelper:AddStick(text,l,t,r,b,defaultcolor)

		viewHelper:AddStick(newStickId, text, defaultcolor, l,t,r,b)
	elseif eventName == "OnDelBtnClick" then
		dataHelper:DelStick(viewData.id)
		viewHelper:DelStick(viewData.id)
	elseif eventName == "OnColorChange" then
	elseif eventName == "OnTextChange" then
		dataHelper:SetStick(viewData.id, viewData.newText)
	elseif eventName == "OnPosChange" then
		dataHelper:SetStick(viewData.id, nil,viewData.l,viewData.t,viewData.r,viewData.b)
	end
end)