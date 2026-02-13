
function onStart()
  --transform.ry = 45
  --transform.rz = 45
end

local move = 4
local wasColliding = false

function onUpdate(dt)

  if box.colliding and not wasColliding then
  	--print("Collision")
  	--move = -move
  	--transform.x = transform.x + move * dt
  else
    --transform.x = transform.x + move * dt
  end
  --print(transform.x)
end
