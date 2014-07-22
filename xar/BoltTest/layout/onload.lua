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
	
	local xmlObj = XLGetObject("Sticker.Helper.Xml")
	local xmlTable = {}
	xmlTable["data"] = {}
	xmlTable["data"]["item1"] = "haha"
	xmlTable["data"]["item2"] = {}
	xmlTable["data"]["item2"]["item21"] = "1"
	xmlTable["data"]["item2"]["item22"] = "hahahahahahahaha"
	local item = {}
	item["item"] = {["arritem1"] = "hahahaha", ["arritem2"] = "asljdfk", ["arritem3"] = "fadsfasdfas"}
	table.insert(xmlTable["data"], item)
	local item = {}
	item["item"] = {["arritem1"] = "2hahahaha", ["arritem2"] = "2asljdfk", ["arritem3"] = "2fadsfasdfas"}
	table.insert(xmlTable["data"], item)
	xmlObj:SetXml([[C:\Users\Dongyu\Desktop\135213341.xml]], xmlTable)
	local dataTable = xmlObj:GetXml([[C:\Users\Dongyu\Desktop\135213341.xml]])
	print_lua_table(dataTable)
	
	return true
end

-- 打印table
function print_lua_table(lua_table, indent)
    indent = indent or 0
    for k, v in pairs(lua_table) do
        if type(k) == "string" then
            k = string.format("%q", k)
        end
        local szSuffix = ""
        if type(v) == "table" then
            szSuffix = "{"
        end
        local szPrefix = string.rep("    ", indent)
        formatting = szPrefix.."["..k.."]".." = "..szSuffix
        if type(v) == "table" then
            XLPrint("[Dongyu] " .. formatting)
            print_lua_table(v, indent + 1)
            XLPrint("[Dongyu] "..szPrefix.."},")
        else
            local szValue = ""
            if type(v) == "string" then
                szValue = string.format("%q", v)
            else
                szValue = tostring(v)
            end
            XLPrint("[Dongyu] " .. formatting..szValue..",")
        end
    end
end

local ret = Main()
if not ret then
	os.exit()
end