function OnInitControl(ctrlObj)
end

function MainWndHostWnd_OnSize(hostWnd, type, width, height)
	local objTree = hostWnd:GetBindUIObjectTree()
	local ctrlObj = objTree:GetUIObject("ctrl")
	ctrlObj:SetObjPos(0,0,width,height)
end

function CloseBtn_OnLButtonDown(self)
	local stickerCore = XLGetObject("Sticker.Core")
	stickerCore:ExitSticker()
	-- os.exit()
end


