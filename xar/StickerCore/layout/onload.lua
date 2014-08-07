local fileSystem = XLGetObject("Sticker.Helper.FileSystem")
local path = fileSystem:GetUpperDir(__document)
XLPrint("[Dongyu] path = " .. tostring(path))
XLLoadModule(path .. [[\StickerData\StickerDataHelper.lua]])["RegisterObject"]()
XLLoadModule(path .. [[\StickerView\StickerViewHelper.lua]])["RegisterObject"]()


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