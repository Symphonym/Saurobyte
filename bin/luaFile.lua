
function init()
	print("INIT")
	SubscribeEvent("KeyDown")
end

function update(delta)
	local x, y = GetMousePos()

	if (IsMousePressed("left")) then
		--print("LEFTERU X " .. x .. "  Y " .. y)
	elseif (IsMousePressed("right")) then
		--print("RIGHTERU X " .. x .. "  Y " .. y)
	elseif (IsMousePressed("middle")) then
		--print("MIDDERU X " .. x .. "  Y " .. y)
	end

	if(IsKeyPressed("B")) then
		SubscribeEvent("KeyDown")
	end


	--[[if(HasComponent("Compiz")) then
		local comp = GetComponent("Compiz")
		print("COMP COUNT " .. GetComponentCount())
		SetComponentValue(comp, "Variable", 1337, 337)
		print(tostring(GetComponentValue(comp, "Variable")));


	end

	RemoveComponent("Compiz")
	print("COMP COUNT " .. GetComponentCount())
	ChangeScene("H")]]
end

function events(eventName, ...)
	if(eventName == "KeyDown") then
		local keyName, keyRepeat = ...

		print("KEY " .. keyName .. " REPEAT " .. tostring(keyRepeat))

		if(keyName == "H" and not keyRepeat) then
			print("UNSUBSCRIBE")
			UnsubscribeEvent("KeyDown")
		end
	end
end

