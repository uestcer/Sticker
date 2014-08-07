
function AddStick(helperObj,id,text,time,color,l,t,r,b)

end

function DelStick(helperObj,id)

end

function SetStick(helperObj,sid,text,time,color,l,t,r,b)

end

function GetStick(helperObj,id)

end

function GetAllStick(helperObj)

end

function RegisterObject()
	local helperObj = {}
	helperObj.AddStick = AddStick
	helperObj.DelStick = DelStick
	helperObj.SetStick = SetStick
	helperObj.GetStick = GetStick
	helperObj.GetAllStick = GetAllStick
	
	XLSetGlobal("Sticker.StickerViewHelper", helperObj)
end