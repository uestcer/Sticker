function Main()
	local templateManager = XLGetObject("Xunlei.UIEngine.TemplateManager")
	local frameHostWndTemplate = templateManager:GetTemplate("MainWndHostWnd", "HostWndTemplate")
	if not frameHostWndTemplate then
		return
	end
	local frameHostWnd = frameHostWndTemplate:CreateInstance("MainFrame")
	if not frameHostWnd then
		return
	end
	local objtreeTemplate = templateManager:GetTemplate("MainWndTree", "ObjectTreeTemplate")
	if not objtreeTemplate then
		return
	end
	local objtree = objtreeTemplate:CreateInstance("MainObjectTree")
	if not objtree then
		return
	end
	frameHostWnd:BindUIObjectTree(objtree)
	frameHostWnd:Create()
	
	return true
end

local ret = Main()
if not ret then
	os.exit()
end
