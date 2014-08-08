local fileSystem = XLGetObject("Sticker.Helper.FileSystem")
local path = fileSystem:GetUpperDir(__document)
XLLoadModule(path .. [[\StickerData\StickerDataHelper.lua]])["RegisterObject"]()
XLLoadModule(path .. [[\StickerView\StickerViewHelper.lua]])["RegisterObject"]()


local templateManager = XLGetObject("Xunlei.UIEngine.TemplateManager")
local frameHostWndTemplate = templateManager:GetTemplate("StickMainWnd", "HostWndTemplate")
if not frameHostWndTemplate then
	return
end
local frameHostWnd = frameHostWndTemplate:CreateInstance("MainFrame")
if not frameHostWnd then
	return
end
local objtreeTemplate = templateManager:GetTemplate("StickMainWndTree", "ObjectTreeTemplate")
if not objtreeTemplate then
	return
end
local objtree = objtreeTemplate:CreateInstance("MainObjectTree")
if not objtree then
	return
end
frameHostWnd:BindUIObjectTree(objtree)
frameHostWnd:Create()

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