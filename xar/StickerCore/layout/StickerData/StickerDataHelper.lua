
function AddStick(helperObj, text, l, t, r, b, color)
	return helperObj.Database:AddStick(text,l,t,r,b,color)
end

function DelStick(helperObj, stick_id)
	helperObj.Database:DelStick(stick_id)
end

function SetStick(helperObj, stick_id, text, l,t,r,b, color)
	helperObj.Database:SetStick(stick_id,text,l,t,r,b,color)
end

function GetStick(helperObj, stick_id)
	return helperObj.Database:GetStick(stick_id)
end

function GetAllStick(helperObj)
	return helperObj.Database:GetAllStick()
end

function RegisterObject()
	local helperObj = {}
	helperObj.AddStick = AddStick
	helperObj.DelStick = DelStick
	helperObj.SetStick = SetStick
	helperObj.GetStick = GetStick
	helperObj.GetAllStick = GetAllStick
	
	if XLGetGlobal("Sticker.StickerDataBase") == nil then
		local fileSystem = XLGetObject("Sticker.Helper.FileSystem")
		local path = fileSystem:PathCombine(__document, "..\\StickerDataBase.lua")
		XLLoadModule(path)["RegisterObject"]()
	end
	-- 加载并打开 Database
	helperObj.Database = XLGetGlobal("Sticker.StickerDataBase")
	helperObj.Database:Open()
	
	-- 退出时关闭 Database
	local stickerCore = XLGetObject("Sticker.Core")
	stickerCore:AttachListener(function()
		helperObj.Database:Close()
	end)
	
	XLSetGlobal("Sticker.StickerDataHelper", helperObj)
end