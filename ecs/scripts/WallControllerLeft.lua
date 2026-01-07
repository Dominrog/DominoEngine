
function onStart()

end


function onUpdate(dt)

  if input.isDown("W") then
    transform.y = transform.y - 3 * dt
  end

  if input.isDown("S") then
    transform.y = transform.y + 3 * dt
  end
end