
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
	helperObj.AddStick = AddStick
	helperObj.DelStick = DelStick
	helperObj.SetStick = SetStick
	helperObj.GetStick = GetStick
	helperObj.GetAllStick = GetAllStick
	
	XLSetGlobal("Sticker.StickerDataHelper", helperObj)
end