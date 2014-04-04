
function init(self)
	print("INIT")
	self:SubscribeEvent("KeyDown")
	self:SubscribeEvent("KeyUp")
	self:SubscribeEvent("SceneLoad")


	print("Comp count " .. self:GetComponentCount())
	print("WINDOW " .. game:GetWindowWidth() .. " X " .. game:GetWindowHeight())

	local scene = game:GetActiveScene()
	print("SCENE NAME " .. scene:GetName())
	local ents = scene:GetEntities()

	for index, value in ipairs(ents) do
		print("Entity " .. value:GetID() .. " inside (" .. scene:GetName() .. ") = " .. tostring(scene:Contains(value)))
	end

	RegisterAudio("forest.ogg", "SpookyShit")
end

local r, g, b = 1,1,1

function update(self, delta)

	if(IsKeyPressed("Left")) then
		game:MoveCamera(0,0,-delta*10)
	end
	if(IsKeyPressed("Right")) then
		game:MoveCamera(0,0,delta*10)
	end
	if(IsKeyPressed("Up")) then
		game:MoveCamera(0, delta*10, 0)
	end
	if(IsKeyPressed("Down")) then
		game:MoveCamera(0, -delta*10, 0)
	end


	if(IsKeyPressed("T")) then
		print("FPS " .. 1.0/delta)
		print(game:GetWindowWidth() .. " X " .. game:GetWindowHeight())
	end

	--local x, y = GetMousePos()

	--print("Comp count " .. self:GetComponentCount())

	--local comp = self:GetComponent("MeshComponent")
	--comp:SetValue("color", 1, 0, 0)

	--[[if (IsMousePressed("left")) then
		r = r - 10*delta
	elseif (IsMousePressed("right")) then
		r = r + 10*delta
	elseif (IsMousePressed("middle")) then
		print(1.0/delta)
	end




	if(IsKeyPressed("B")) then
		SubscribeEvent("KeyDown")
	end
	if(HasComponent("MeshComponent")) then
		local comp = GetComponent("MeshComponent")
		SetComponentValue(comp, "color", r, g, b)
	end
	if(HasComponent("MeshComponent") and IsKeyPressed("L")) then
		local comp = GetComponent("MeshComponent")

		SetComponentValue(comp, "color", 1, 1, 1)


	end]]

end

function events(self, eventName, ...)
	print(self:GetID() .. " " .. eventName)
	if(eventName == "KeyDown") then
		local keyName, keyRepeat = ...
		if(keyName == "K" and not keyRepeat) then
			self:Kill()
		end

		if(keyName == "P" and not keyRepeat) then
			PlaySound("SpookyShit")
		elseif(keyName == "O" and not keyRepeat) then

		end
	end
	--[[print("NAME " .. self:GetComponent("MeshComponent"):GetName())

	if(eventName == "KeyDown") then
		local keyName, keyRepeat = ...

		if(keyName == "H" and not keyRepeat) then
			self:KillEntity()
		elseif(keyName == "C" and not keyRepeat) then
			game:ChangeScene("HUE")
		elseif(keyName == "V" and not keyRepeat) then
			game:ChangeScene("H")
		end
	end]]
end

function killed(self)
	print("Killed entity " .. self:GetID())
end

