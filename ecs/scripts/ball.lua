
function onStart()

end

local move = 10
local wasColliding = false

function onUpdate(dt)

  if box.colliding and not wasColliding then
  	print("Collision")
  	move = -move
  	transform.x = transform.x + move * dt
  else
    transform.x = transform.x + move * dt
  end
end
