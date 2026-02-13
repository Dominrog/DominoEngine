
function onStart()
  transform.rz = 90
end


function onUpdate(dt)

  if input.isDown("A") then
    transform.y = transform.y - 3 * dt
  end

  if input.isDown("D") then
    transform.y = transform.y + 3 * dt
  end

end