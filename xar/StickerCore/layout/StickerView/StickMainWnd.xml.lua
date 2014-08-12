
-- SetStick 不一定要和StickDataHelper中的数据结构保持对应关系，所以不需要有 time,l,t,r,b 这些参数
function StickCtrl_SetStick(ctrlObj,id,text,color,title)
	local attr = ctrlObj:GetAttribute()
	attr.stickData = attr.stickData or {}
	if id then
		attr.stickData["id"] = id
	end
	if text then
		attr.stickData["text"] = text
		local editObj = ctrlObj:GetControlObject("text.edit")
		editObj:SetText(tostring(text))
	end
	if color then
		attr.stickData["color"] = color
	end
	if title then
		attr.stickData["title"] = title
	end
end

function StickCtrl_GetStick(ctrlObj)
	local attr = ctrlObj:GetAttribute()
	return attr.stickData
end

function StickCtrl_OnInitControl(ctrlObj)
	local attr = ctrlObj:GetAttribute()
	attr.stickData = {}
end

function StickCtrl_OnDestroy(ctrlObj)

end

function AddButton_OnClick(btnObj)
	local ctrlObj = btnObj:GetOwnerControl()
	local attr = ctrlObj:GetAttribute()
	ctrlObj:FireExtEvent("OnAddBtnClick", attr.stickData["id"])
end

function DelButton_OnClick(btnObj)
	local ctrlObj = btnObj:GetOwnerControl()
	local attr = ctrlObj:GetAttribute()
	ctrlObj:FireExtEvent("OnDelBtnClick", attr.stickData["id"])
end

function TextEdit_OnChange(editObj)
	local ctrlObj = editObj:GetOwnerControl()
	local attr = ctrlObj:GetAttribute()
	local text = editObj:GetText()
	if text then
		attr.stickData["text"] = text
	end
	ctrlObj:FireExtEvent("OnTextChange", attr.stickData["id"], attr.stickData["text"])
end

function TextEdit_OnFocusChange(editObj, bFocus, lastFocusObj)
	local ctrlObj = editObj:GetOwnerControl()
	local addBtn = ctrlObj:GetControlObject("add.button")
	local delBtn = ctrlObj:GetControlObject("del.button")
	if bFocus then
		if not addBtn:GetVisible() then
			addBtn:SetVisible(true)
			addBtn:SetChildrenVisible(true)
		end
		if not delBtn:GetVisible() then
			delBtn:SetVisible(true)
			delBtn:SetChildrenVisible(true)
		end
	else
		local objTree = ctrlObj:GetOwner()
		local hostwnd = objTree:GetBindHostWnd()
		local osshell = XLGetObject("Sticker.Helper.OSShell")
		local x,y = osshell:GetCursorPos()
		x,y = hostwnd:ScreenPtToHostWndPt(x,y)
		local captionObj = ctrlObj:GetControlObject("caption.caption")
		local l,t,r,b = captionObj:GetAbsPos()
		if x<l or x>r or y<t or y>b then
			if addBtn:GetVisible() then
				addBtn:SetVisible(false)
				addBtn:SetChildrenVisible(false)
			end
			if delBtn:GetVisible() then
				delBtn:SetVisible(false)
				delBtn:SetChildrenVisible(false)
			end
		end
	end
end

function Caption_OnMouseMove(captionObj, x, y, flags)
	local ctrlObj = captionObj:GetOwnerControl()
	local addBtn = ctrlObj:GetControlObject("add.button")
	local delBtn = ctrlObj:GetControlObject("del.button")
	if not addBtn:GetVisible() then
		addBtn:SetVisible(true)
		addBtn:SetChildrenVisible(true)
	end
	if not delBtn:GetVisible() then
		delBtn:SetVisible(true)
		delBtn:SetChildrenVisible(true)
	end
end

function Caption_OnMouseLeave(captionObj, x, y)
	local ctrlObj = captionObj:GetOwnerControl()
	local addBtn = ctrlObj:GetControlObject("add.button")
	local delBtn = ctrlObj:GetControlObject("del.button")
	local editObj = ctrlObj:GetControlObject("text.edit")
	if not editObj:GetFocus() then
		if addBtn:GetVisible() then
			addBtn:SetVisible(false)
			addBtn:SetChildrenVisible(false)
		end
		if delBtn:GetVisible() then
			delBtn:SetVisible(false)
			delBtn:SetChildrenVisible(false)
		end
	end
end

function AddBtn_OnMouseMove(btnObj)
	btnObj:RouteToFather()
end

function AddBtn_OnMouseLeave(btnObj)
	btnObj:RouteToFather()
end

function DelBtn_OnMouseMove(btnObj)
	btnObj:RouteToFather()
end

function DelBtn_OnMouseLeave(btnObj)
	btnObj:RouteToFather()
end