
function StickCtrl_SetStick(ctrlObj)

end

function StickCtrl_GetStick(ctrlObj)

end

function StickCtrl_OnInitControl(ctrlObj)

end

function StickCtrl_OnDestroy(ctrlObj)

end

function AddButton_OnClick(btnObj)
	local ctrlObj = btnObj:GetOwnerControl()
	ctrlObj:FireExtEvent("OnAddBtnClick")
end

function DelButton_OnClick(btnObj)
	local ctrlObj = btnObj:GetOwnerControl()
	ctrlObj:FireExtEvent("OnDelBtnClick")
end