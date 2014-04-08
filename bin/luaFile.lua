
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

	if(IsKeyPressed("Left") or IsKeyPressed("A")) then
		game:MoveCamera(0,0,-delta*10)
	end
	if(IsKeyPressed("Right") or IsKeyPressed("D")) then
		game:MoveCamera(0,0,delta*10)
	end
	if(IsKeyPressed("Up") or IsKeyPressed("W")) then
		game:MoveCamera(0, delta*10, 0)
	end
	if(IsKeyPressed("Down") or IsKeyPressed("S")) then
		game:MoveCamera(0, -delta*10, 0)
	end


	if(IsKeyPressed("T")) then
		print("FPS " .. 1.0/delta)
		print(game:GetWindowWidth() .. " X " .. game:GetWindowHeight())
	end

end

function events(self, eventName, ...)
	print(self:GetID() .. " " .. eventName)
	if(eventName == "KeyDown") then
		local keyName, keyRepeat = ...
		if(keyName == "K" and not keyRepeat) then
			self:Kill()
		end

		if(keyName == "P" and not keyRepeat) then
			PlaySound("Swag")
		elseif(keyName == "O" and not keyRepeat) then
			local comp = self:GetComponent("TransformComponent")
			local x, y, z = comp:GetValue("position")
			print("Heru X= "..x.. " Y= "..y.." Z= "..z)
		end
	end
end

function killed(self)
	print("Killed entity " .. self:GetID())
end

