
function onStart()
  transform.rz = 90
end


function onUpdate(dt)

  local entity = getEntity("camera")
  local entity_transform = getTransform(entity)

  if input.isDown("A") then
    entity_transform.z = entity_transform.z + 10 * dt
  end

  if input.isDown("S") then
    transform.y = transform.y - 3 * dt
  end

  if input.isDown("W") then
    transform.y = transform.y + 3 * dt
  end

end