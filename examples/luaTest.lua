
print("HUR DUR")
print(Testeru())
print("PST HUR DUR")
print(type(Testeru))

print(type(SWAG))
SWAG:Add(55)
SWAG:Print()
SWAG:Add("DASDAD")
SWAG:Print()
SWAG:Add()
SWAG:Add()
SWAG:Add()
SWAG:Print()

print("ADD STATUS: " .. tostring(SWAG.Add))

local swag = SWAG:Clone()
swag:Add()
swag:Print()

for key, val in pairs(leTestTable) do
	print("Key: " .. key .. " Val: " .. tostring(val))
	for subKey, subVal in pairs(val) do
		print("SubKey: " ..  subKey .. " SubVal: " .. subVal)
	end
end

print("FINALLY: " .. leTestTable.NESTED["A testing value :O"])
print("FINALLY: " .. leTestTable.NESTED.ast)
print(os.date())

GLOBAL_VAR_TEST = "Some silly text"