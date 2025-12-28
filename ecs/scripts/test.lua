

function onStart()
  transform.sz = 10
  transform.sx = 10
  transform.sy = 10
end


function onUpdate(dt)
  transform.z = transform.z - 0.5 * dt
end