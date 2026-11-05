function onStart()
  transform.rz = 90
end


function onUpdate(dt)

  local vx = 0
  local vy = 0

  if input.isDown("A") then
    vx = vx - 3
  end

  if input.isDown("D") then
    vx = vx + 3
  end

  if input.isDown("S") then
    vy = vy - 3
  end

  if input.isDown("W") then
    vy = vy + 3
  end

  setVelocity(self, vx, vy, 0)

end