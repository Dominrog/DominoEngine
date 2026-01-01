function onStart()
  
end

function onUpdate(dt)
  if link.target then
    local target = getTransform(link.target)

    if target then
      transform.x = target.x
      transform.y = target.y
      transform.z = target.z

      camera.yaw = -target.ry + 90
      camera.pitch = -target.rx
    end
  end
end
