
function AddStick(helperObj,id,text,time,color,l,t,r,b)

end

function DelStick(helperObj,id)

end

function SetStick(helperObj,id,text,time,color,l,t,r,b)

end

function GetStick(helperObj,id)
	local item = {}
	return item
end

function GetAllStick(helperObj)

end

function AttachListener(helperObj, callback)

end

function DetachListener(helperObj, cookie)

end

function RegisterObject()
	local helperObj = {}
	helperObj.AddStick = AddStick
	helperObj.DelStick = DelStick
	helperObj.SetStick = SetStick
	helperObj.GetStick = GetStick
	helperObj.GetAllStick = GetAllStick
	helperObj.AttachListener = AttachListener
	helperObj.DetachListener = DetachListener
	
	XLSetGlobal("Sticker.StickerViewHelper", helperObj)
end