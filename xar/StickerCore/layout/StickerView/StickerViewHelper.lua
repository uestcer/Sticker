local function SVHPrint(str) XLPrint("[Sticker][StickViewHelper] " .. tostring(str)) end


function AddStick(helperObj,id,text,color,l,t,r,b)
	if id == nil or text == nil or color == nil or l == nil or t == nil or r == nil or b == nil then
		SVHPrint("AddStick 参数不合法 id="..tostring(id)..",color="..tostring(color)..",l="..tostring(l)..",t="..tostring(t)..",r="..tostring(r)..",b="..tostring(b))
		return
	end
	local hostwndId = "StickMainWnd.Instance." .. tostring(id)
	local objtreeId = "StickMainWndTree.Instance." .. tostring(id)

	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd(hostwndId)
	if hostwnd then
		return
	end
	local templateManager = XLGetObject("Xunlei.UIEngine.TemplateManager")
	local wndTemplate = templateManager:GetTemplate("StickMainWnd", "HostWndTemplate")
	if not wndTemplate then
		return
	end
	local hostwnd = wndTemplate:CreateInstance(hostwndId)
	if not hostwnd then
		return
	end
	local objtreeTemplate = templateManager:GetTemplate("StickMainWndTree", "ObjectTreeTemplate")
	if not objtreeTemplate then
		return
	end
	local objtree = objtreeTemplate:CreateInstance(objtreeId)
	if not objtree then
		return
	end
	hostwnd:BindUIObjectTree(objtree)
	hostwnd:Create()
	
	local ctrlObj = objtree:GetRootObject()
	ctrlObj:SetStick(id, text, color, title)
	hostwnd:Move(l,t,r-l,b-t)
	local function FireViewEvent(eventName, dataTable)
		for i=1,table.maxn(helperObj.CookieTable) do
			local callback = helperObj.CookieTable[i]
			if callback then
				callback(eventName, dataTable)
			end
		end
	end
	ctrlObj:AttachListener("OnAddBtnClick", true, function(obj, eventName, stick_id)
		FireViewEvent("OnAddBtnClick", {["id"] = stick_id})
	end)
	ctrlObj:AttachListener("OnDelBtnClick", true, function(obj, eventName, stick_id)
		FireViewEvent("OnDelBtnClick", {["id"] = stick_id})
	end)
	ctrlObj:AttachListener("OnColorChange", true, function(obj, eventName, stick_id, newColor)
		FireViewEvent("OnColorChange", {["id"] = stick_id, ["newColor"] = newColor})
	end)
	ctrlObj:AttachListener("OnTextChange", true, function(obj, eventName, stick_id, newText)
		FireViewEvent("OnTextChange", {["id"] = stick_id, ["newText"] = newText})
	end)
	hostwnd:AttachListener("OnMove", true, function(targetHostWnd, eventName)
		local objtree = targetHostWnd:GetBindUIObjectTree()
		local ctrlObj = objtree:GetRootObject()
		local stickData = ctrlObj:GetStick()
		local stick_id = stickData["id"]
		if stick_id == nil then
			return
		end
		local l,t,r,b = targetHostWnd:GetWindowRect()
		FireViewEvent("OnPosChange", {["id"] = stick_id, ["l"] = l, ["t"] = t, ["r"] = r, ["b"] = b})
	end)
	
	helperObj.StickIdTable[id] = true
	return true
end

function DelStick(helperObj,id)
	if id == nil then
		SVHPrint("DelStick 参数不合法：id="..tostring(id))
		return
	end
	local hostwndId = "StickMainWnd.Instance." .. tostring(id)
	
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd(hostwndId)
	if hostwnd == nil then
		SVHPrint("DelStick 未找到HostWnd： hostwndId="..tostring(hostwndId))
		return
	end
	local objtree = hostwnd:UnbindUIObjectTree()  
	local objtreeManager = XLGetObject("Xunlei.UIEngine.TreeManager")
	objtreeManager:DestroyTree(objtree)
	hostwndManager:RemoveHostWnd(hostwndId)
	
	helperObj.StickIdTable[id] = nil
end

function SetStick(helperObj,id,text,color,l,t,r,b)
	if id == nil then
		return
	end
	local hostwndId = "StickMainWnd.Instance." .. tostring(id)
	
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd(hostwndId)
	if hostwnd == nil then
		return
	end
	local objtree = hostwnd:GetBindUIObjectTree()
	if objtree == nil then
		return
	end
	local ctrlObj = objtree:GetRootObject()
	ctrlObj:SetStick(id,text,color,title)
	if l and t and r and b then
		hostwnd:Move(l,t,r-l,b-t)
	end
end

function GetStick(helperObj,id)
	if id == nil then
		return
	end
	local hostwndId = "StickMainWnd.Instance." .. tostring(id)
	
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd(hostwndId)
	if hostwnd == nil then
		return
	end
	local objtree = hostwnd:GetBindUIObjectTree()
	if objtree == nil then
		return
	end
	local ctrlObj = objtree:GetRootObject()
	local stickData = ctrlObj:GetStick()
	if stickData == nil then
		return
	end
	local data = {}
	data.id = stickData.id
	data.text = stickData.text
	data.title = stickData.title
	data.color = stickData.color
	local l,t,r,b = hostwnd:GetWindowRect()
	data.l = l
	data.t = t 
	data.r = r 
	data.b = b 
	return data
end

function GetAllStick(helperObj)
	local stickIdArray = {}
	for id, isExist in pairs(helperObj.StickIdTable) do
		if isExist then
			table.insert(stickIdArray, id)
		end
	end
	return stickIdArray
end

function BringStickToTop(helperObj, id)
	if id == nil then
		return
	end
	local hostwndId = "StickMainWnd.Instance." .. tostring(id)
	
	local hostwndManager = XLGetObject("Xunlei.UIEngine.HostWndManager")
	local hostwnd = hostwndManager:GetHostWnd(hostwndId)
	if hostwnd == nil then
		return
	end
	hostwnd:BringWindowToTop()
end

function AttachListener(helperObj, callback)
	if callback == nil then
		return
	end
	for i=1, table.maxn(helperObj.CookieTable) + 1 do
		if helperObj.CookieTable[i] == nil then
			helperObj.CookieTable[i] = callback
			return i
		end
	end
end

function DetachListener(helperObj, cookie)
	helperObj.CookieTable[cookie] = nil
end

function RegisterObject()
	local helperObj = {}
	helperObj.AddStick = AddStick
	helperObj.DelStick = DelStick
	helperObj.SetStick = SetStick
	helperObj.GetStick = GetStick
	helperObj.GetAllStick = GetAllStick
	helperObj.BringStickToTop = BringStickToTop
	helperObj.AttachListener = AttachListener
	helperObj.DetachListener = DetachListener
	
	helperObj.StickIdTable = {}
	helperObj.CookieTable = {}
	
	XLSetGlobal("Sticker.StickerViewHelper", helperObj)
end