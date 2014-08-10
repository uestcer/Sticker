
--------------------- 外部接口 ---------------------
function GetText(self)
	local attr = self:GetAttribute()
	return attr.text
end

function SetText(self, text)
	local attr = self:GetAttribute()
	attr.text = text or ""
	local textObj = self:GetControlObject("text")
	textObj:SetText(text or "")
end

function SetImage(self, normal, down, hover, disable)
	local attr = self:GetAttribute()
	attr.image_normal = normal or ""
	attr.image_down = down or ""
	attr.image_hover = down or ""
	attr.image_disable = disable or ""
	
	__SetState(attr.NowState, true)
end

function SetColor(self, normal, down, hover, disable)
	local attr = self:GetAttribute()
	attr.text_color_normal = normal or ""
	attr.text_color_down = down or ""
	attr.text_color_hover = hover or ""
	attr.text_color_disable = disable or ""
	
	__SetState(attr.NowState, true)
end

function SetFont(self, font)
	local attr = self:GetAttribute()
	attr.text_font = font or ""
	local textObj = self:GetControlObject("text")
	textObj:SetTextFontResID(font or "")
end

function SetTextVAlign(self, valign)
	local attr = self:GetAttribute()
	attr.text_valign = valign or ""
	local textObj = self:GetControlObject("text")
	local width,height = textObj:GetTextExtent()
	local l,t,r,b = self:GetObjPos()
	if valign == "center" then
		local l,_,r,_ = textObj:GetObjPos()
		textObj:SetObjPos(l, (b-t-height)/2, r, (b-t-height)/2+height)
	elseif valign == "top" then
		local l,_,r,_ = textObj:GetObjPos()
		textObj:SetObjPos(l,t,r,t+height)
	elseif valign == "bottom" then
		local l,_,r,_ = textObj:GetObjPos()
		textObj:SetObjPos(l,b-height,r,b)
	end
end

function SetTextHAlign(self, halign)
	local attr = self:GetAttribute()
	attr.text_halign = halign or ""
	local textObj = self:GetControlObject("text")
	local width,height = textObj:GetTextExtent()
	local l,t,r,b = self:GetObjPos()
	if halign == "center" then
		local _,t,_,b = textObj:GetObjPos()
		textObj:SetObjPos((r-l-width)/2, t, (r-l-width)/2 + width, b)
	elseif halign == "left" then
		local _,t,_,b = textObj:GetObjPos()
		textObj:SetObjPos(l,t,l+width,b)
	elseif halign == "right" then
		local _,t,_,b = textObj:GetObjPos()
		textObj:SetObjPos(r-width, t, r, b)
	end
end

function SetTextPos(self, left, top, right, bottom)
	local attr = self:GetAttribute()
	attr.text_left = left or 0
	attr.text_top = top or 0
	attr.text_right = right or 0
	attr.text_bottom = bottom or 0
	local textObj = self:GetControlObject("text")
	textObj:SetObjPos(attr.text_left, attr.text_top, attr.text_right, attr.text_bottom)
end

-------------------- 事件处理函数 ---------------------
function OnInitControl(self)
	local attr = self:GetAttribute()
	local bkgObj = self:GetControlObject("bkg")
	local textObj = self:GetControlObject("text")
	__SetState(self, "normal", true)
	textObj:SetText(attr.text or "")
	textObj:SetTextFontResID(attr.text_font or "")
	if attr.text_left ~= nil or attr.text_top ~= nil or attr.text_right ~= nil or attr.text_bottom ~= nil then
		local l = attr.text_left or 0
		local t = attr.text_top or 0
		local r = attr.text_right or 0
		local b = attr.text_bottom or 0
		textObj:SetObjPos(l,t,r,b)
	else
		if attr.text_valign then
			SetTextVAlign(self, attr.text_valign)
		elseif attr.text_halign then
			SetTextHAlign(self, attr.text_halign)
		end
	end
end

function OnLButtonDown(self, x, y)
	self:SetCaptureMouse(true)
	__SetState(self, "down")
end

function OnLButtonUp(self)
	self:SetCaptureMouse(false)
	__SetState(self, "normal")
	
	self:FireExtEvent("OnClick")
end

function OnMouseMove(self, x, y)
	local state = __GetState(self)
	if state == "normal" then
		__SetState(self, "hover")
	end
end

function OnMouseLeave(self, x, y)
	local state = __GetState(self)
	if state == "hover" then
		__SetState(self, "normal")
	end
end

function OnEnableChange(self, bEnable)
	local state = __GetState(self)
	if state == "notmal" and not bEnable then
		__SetState(self, "disable")
	elseif state == "disable" and bEnable then
		__SetState(self, "normal")
	end
end

-------------------- 私有函数 --------------------

-- 按钮的状态：normal,down,hover,disable
function __GetState(self)
	local attr = self:GetAttribute()
	return attr.NowState
end

function __SetState(self, newState, force)
	local attr = self:GetAttribute()
	if newState == attr.NowState and not force then
		return
	end
	local bkgObj = self:GetControlObject("bkg")
	local textObj = self:GetControlObject("text")
	if newState == "normal" then
		bkgObj:SetTextureID(attr.image_normal)
		textObj:SetTextColorResID(attr.text_color_normal)
	elseif newState == "down" then
		bkgObj:SetTextureID(attr.image_down)
		textObj:SetTextColorResID(attr.text_color_down)
	elseif newState == "hover" then
		bkgObj:SetTextureID(attr.image_hover)
		textObj:SetTextColorResID(attr.text_color_hover)
	elseif newState == "disable" then
		bkgObj:SetTextureID(attr.image_disable)
		textObj:SetTextColorResID(attr.text_color_disable)
	end
	
	attr.NowState = newState
end
